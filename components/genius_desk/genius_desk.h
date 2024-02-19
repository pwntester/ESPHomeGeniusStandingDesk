#pragma once


#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"

namespace esphome {
    namespace desk_switch {
        class DeskSwitch;
    }
    namespace memory_button {
        class MemoryButton;
    }
    namespace calibration_button {
        class CalibrationButton;
    }
    namespace genius_desk {

        class GeniusDesk : public uart::UARTDevice, public PollingComponent {
        public:
            void setup() override;
            void loop() override;
            void update() override;
            void dump_config() override;
            void add_memory_button(memory_button::MemoryButton *button);
            void add_calibration_button(calibration_button::CalibrationButton *button);
            void add_switch(desk_switch::DeskSwitch *switch_);
            void set_target_height(float target);
            void calibrate();
            void register_sensor(sensor::Sensor *obj) {
                this->height_sensors.push_back(obj);
            }
            void register_moving_sensor(binary_sensor::BinarySensor *obj) {
                this->moving_sensors.push_back(obj);
            }
            void register_calibration_sensor(binary_sensor::BinarySensor *obj) {
                this->calibration_sensors.push_back(obj);
            }
            void set_min_height(float min_height) {
                this->min_height = min_height;
            }
            void set_max_height(float max_height) {
                this->max_height = max_height;
            }
            float get_current_height() {
              return this->height;
            }
            float get_min_height() {
              return this->min_height;
            }
            float get_max_height() {
              return this->max_height;
            }
            float get_pulse_size() {
              return this->pulse_size;
            }

        private:
            std::vector<sensor::Sensor *> height_sensors;
            std::vector<binary_sensor::BinarySensor *> moving_sensors;
            std::vector<binary_sensor::BinarySensor *> calibration_sensors;
            std::vector<desk_switch::DeskSwitch *> desk_switches;
            std::vector<calibration_button::CalibrationButton *> calibration_buttons;

            /**
             * @brief The desk min height in CM.
             */
            float min_height = 80.0;

            /**
             * @brief The desk max height in CM.
             */
            float max_height = 120.0;

            /**
             * @brief The Desk current height
             */
            float height = 0.0;

            /**
             * @brief The Desk last known height
             */
            float last_height = 0.0;

            /**
             * @brief The Desk target height 
             */
            float target_height = 0.0;

            /**
             * @brief 1 pulse corresponds to 0.1111 mm
             */
            float pulse_size = 0.1111; 

            /**
             * @brief Response bit mask: 0011 1000
             * Commands send by the power supplies
             */
            uint8_t resp_mask = 0x38;

            /**
             * @brief PSU mask: 0000 0111
             * PSU address (from 1 to 4)
             */
            uint8_t psu_mask  = 0x07;

            /**
             * @brief PWM mask: 1100 0000
             * Whenever the PWM of the power supplies is within the limit
             * For adjustments so all power supplies run equally.
             */
            uint8_t pwm_mask  = 0xC0;

            /**
             * @brief Whether manual calibration is needed.
             */
            bool calibration_needed = false;

            /**
             * @brief Indicates if desk is being calibrated. 
             */
            bool calibrating = false;
        };

    } // namespace genius_desk
} // namespace esphome
