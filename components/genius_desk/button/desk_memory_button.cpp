#include "desk_memory_button.h"
#include "esphome/core/log.h"
#include "../genius_desk.h"

namespace esphome {
    namespace memory_button {

        static const char *const TAG = "Desk.memoryButton";

        void MemoryButton::dump_config() {
            LOG_BUTTON("", "Memory Button", this);
        }

        void MemoryButton::press_action() {
            switch (this->memory_id)
            {
            case 0:
                desk->set_target_height(80);
                break;
            case 1:
                desk->set_target_height(93);
                break;
            case 2:
                desk->set_target_height(106);
                break;
            case 3:
                desk->set_target_height(120);
                break;
            default:
                break;
            }
        }

    } // namespace memory_button
} // namespace esphome
