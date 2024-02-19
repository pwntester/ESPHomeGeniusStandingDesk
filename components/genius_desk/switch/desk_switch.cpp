#include "esphome/core/log.h"
#include "desk_switch.h"
#include "../genius_desk.h"

namespace esphome {
    namespace desk_switch {

        static const char *TAG = "desk.switch";

        void DeskSwitch::write_state(bool state) {
            this->publish_state(state);
            if (state) {
              if (this->direction) {
                desk->set_target_height(desk->get_max_height());
              } else {
                desk->set_target_height(desk->get_min_height());
              }
            } else {
              if (this->direction) {
                desk->set_target_height(desk->get_current_height() + 15*desk->get_pulse_size());
              } else {
                desk->set_target_height(desk->get_current_height() - 15*desk->get_pulse_size());
              }
            }
        }

        void DeskSwitch::dump_config() {
            ESP_LOGCONFIG(TAG, "Desk switch %s", (this->direction ? "up" : "down"));
        }

    } // namespace desk_switch
} // namespace esphome
