#pragma once

#include "esphome/core/component.h"
#include "esphome/components/switch/switch.h"
#include "esphome/components/uart/uart.h"

namespace esphome
{
    namespace genius_desk
    {
        class GeniusDesk;
    }
    namespace desk_switch
    {

        class DeskSwitch : public switch_::Switch, public Component
        {
        public:
            void loop() override;
            void setup() override;
            void write_state(bool state) override;
            void dump_config() override;
            void set_direction(bool direction)
            {
                this->direction = direction;
            };
            void set_uart_device(uart::UARTDevice *uart_device)
            {
                this->uart_device = uart_device;
            };
            void set_desk(genius_desk::GeniusDesk *desk)
            {
                this->desk = desk;
            };

        protected:
            uint32_t last_send = 0;
            bool direction = false;
            uart::UARTDevice *uart_device;
            genius_desk::GeniusDesk *desk;
        };

    } // namespace desk_switch
} // namespace esphome
