#include "esphome/core/log.h"
#include "genius_desk.h"
#include "./button/desk_memory_button.h"
#include "./switch/desk_switch.h"

namespace esphome
{
    namespace genius_desk
    {

        static const char *TAG = "genius_desk.sensor";

        void GeniusDesk::setup() {
          this->set_update_interval(this->update_interval);
        }

        void GeniusDesk::loop() {
        }

        void GeniusDesk::set_target_height(float target) {
          ESP_LOGCONFIG(TAG, "Change target height from %f to %f", this->target_height, target);
          this->target_height = target;
        }

        void GeniusDesk::update() {

            // Read UART Bus
            static uint8_t buffer[3] = {0};
            int i = 0;
            while (available())
            {
                buffer[i % 3] = (uint8_t)read();
                i++;
            }

            uint8_t resp = (buffer[0] & this->resp_mask) >> 3;

            // Ignoring psu and pwm values as I dont know what to do with them
            uint8_t psu = (buffer[1] & this->psu_mask);
            uint8_t pwm = (buffer[2] & this->pwm_mask) >> 6;

            // this height only makes sense for certain response codes
            // so its only assigned to the current height for those codes.
            unsigned int raw_height = (256*(unsigned int)buffer[2]) + buffer[1];

            // check desk status
            switch (resp) {
            case 0x00:
              // resp 0x00 = slave is stationary
              height = ((raw_height / 2) * this->pulse_size) / 10;

              // Publish the desk height if it changed
              if (height != last_height) {
                  ESP_LOGCONFIG(TAG, "Desk stopped");
                  ESP_LOGCONFIG(TAG, "New height: %f", height);
                  last_height = height;
                  for (auto *height_sensor : this->height_sensors)
                      height_sensor->publish_state(height);
                  for (auto *moving_sensor : this->moving_sensors)
                      moving_sensor->publish_state(false);
              }
              break;
            case 0x1:
              // resp 0x01 = the slave sends back the program date instead of the height  
              ESP_LOGCONFIG(TAG, "Desk is sending back the program date");
              break;
            case 0x2:
              // resp 0x02 = slave detected an obstacle (anti-collision) while driving up
              ESP_LOGCONFIG(TAG, "Collision detected while driving up");
              break;
            case 0x3:
              // resp 0x03 = slave detected an obstacle (anti-collision) while driving down
              ESP_LOGCONFIG(TAG, "Collision detected while driving down");
              break;
            case 0x4:
              // resp 0x04 = slave is moving
              height = ((raw_height / 2) * this->pulse_size) / 10;

              // Publish the desk height if it changed
              if (height != last_height) {
                  last_height = height;
                  for (auto *height_sensor : this->height_sensors)
                      height_sensor->publish_state(height);
                  for (auto *moving_sensor : this->moving_sensors)
                      moving_sensor->publish_state(true);
              }
              break;
            case 0x5:
              // resp 0x05 = after the altitude reset, the slave responds until it reaches the minimum
              ESP_LOGCONFIG(TAG, "Calibration needed");
              break;
            case 0x6:
              // resp 0x06 = uphill overload
              ESP_LOGCONFIG(TAG, "Uphill overload");
              break;
            case 0x7:
              // resp 0x07 = downhill overload
              ESP_LOGCONFIG(TAG, "Downhill overload");
              break;
            default:
              ESP_LOGCONFIG(TAG, "Received response value: %x", resp);
            }

            // move the desk if needed
            if (
              // target height has been set
              this->target_height != 0 and
              // we are not already at that height
              this->target_height != this->height and
              // target height is between the min/max range
				      this->target_height >= this->min_height and
				      this->target_height <= this->max_height and
              // the current height is within 1 motor pulse of the target height
				      (
					        this->height < this->target_height - 0.5*this->pulse_size or 
					        this->height > this->target_height + 0.5*this->pulse_size
				      ) 
            ) {
              // 0010 0000 - drive to a given altitude
              uint8_t cmd[3] = { 0x20, 0x00, 0x00 };
              float t = (this->target_height * 10) / pulse_size;
              cmd[2] = (int) t / 256;
              cmd[1] = (int) t % 256;
              write_array(cmd, sizeof(cmd));
            } else {
              // default command = stop (do not move)
              write_array({ 0x00, 0x00, 0x00 });
            }
        }

        void GeniusDesk::dump_config()
        {
            ESP_LOGCONFIG(TAG, "Genius Desk");
            for (auto *height_sensor : this->height_sensors)
                LOG_SENSOR("", "Height sensor: ", height_sensor);
            for (auto *moving_sensor : this->moving_sensors)
                LOG_BINARY_SENSOR("", "Is Moving binary sensor: ", moving_sensor);
        }

        void GeniusDesk::add_button(memory_button::MemoryButton *button)
        {
            button->set_uart_device(static_cast<uart::UARTDevice*>(this));
            button->set_desk(static_cast<genius_desk::GeniusDesk *>(this));
        }
        void GeniusDesk::add_switch(desk_switch::DeskSwitch *switch_)
        {
            this->desk_switches.push_back(switch_);
            switch_->set_uart_device(static_cast<uart::UARTDevice*>(this));
            switch_->set_desk(static_cast<genius_desk::GeniusDesk *>(this));
        }
    } // namespace genius_desk
} // namespace esphome
