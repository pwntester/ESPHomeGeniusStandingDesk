#include "calibration_button.h"
#include "esphome/core/log.h"
#include "../genius_desk.h"

namespace esphome {
    namespace calibration_button {

        static const char *const TAG = "Desk.calibrationButton";

        void CalibrationButton::dump_config() {
            LOG_BUTTON("", "Calibration Button", this);
        }

        void CalibrationButton::press_action() {
            desk->calibrate();
        }

    } // namespace calibration_button
} // namespace esphome

