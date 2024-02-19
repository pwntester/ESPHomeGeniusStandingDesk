#pragma once

#include "esphome/core/component.h"
#include "esphome/components/button/button.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
    namespace genius_desk {
        class GeniusDesk;
    }
    namespace calibration_button {

        class CalibrationButton : public button::Button, public Component {
        public:
            void dump_config() override;
            void set_desk(genius_desk::GeniusDesk *desk) {
                this->desk = desk;
            };

        protected:
            void press_action() override;
            genius_desk::GeniusDesk *desk;
        };

    } // namespace calibration_button
} // namespace esphome

