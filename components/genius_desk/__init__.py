import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.components.uart import CONF_UART_ID, UARTComponent
from esphome.const import CONF_ID

DEPENDENCIES = ["uart"]

MULTI_CONF = True

CONF_DESK_ID = "desk_id"
CONF_MIN_HEIGHT = "min_height"
CONF_MAX_HEIGHT = "max_height"

genius_desk_ns = cg.esphome_ns.namespace("genius_desk")
GeniusDesk = genius_desk_ns.class_("GeniusDesk", cg.Component, uart.UARTDevice)

CONFIG_SCHEMA = uart.UART_DEVICE_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(GeniusDesk),
        cv.Required(CONF_UART_ID): cv.use_id(UARTComponent),
        cv.Required(CONF_MIN_HEIGHT): cv.float_,
        cv.Required(CONF_MAX_HEIGHT): cv.float_,
    }
).extend(cv.polling_component_schema("96ms"))


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

    cg.add(var.set_min_height(config[CONF_MIN_HEIGHT]))
    cg.add(var.set_max_height(config[CONF_MAX_HEIGHT]))
