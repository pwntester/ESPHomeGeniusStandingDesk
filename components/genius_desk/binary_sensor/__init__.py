import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor

from .. import CONF_DESK_ID, GeniusDesk

CONF_CALIBRATION = "calibration_needed"
CONF_MOVING = "desk_is_moving"

DEPENDENCIES = ["genius_desk"]

# CONFIG_SCHEMA = binary_sensor.binary_sensor_schema().extend(
#     {cv.GenerateID(CONF_DESK_ID): cv.use_id(GeniusDesk)}
# )
BINARY_SENSOR_CONFIG = (
    binary_sensor.binary_sensor_schema()
    .extend(
        # {cv.GenerateID(): cv.declare_id(DeskSwitch)}
    )
    .extend(cv.COMPONENT_SCHEMA)
)

CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(CONF_DESK_ID): cv.use_id(GeniusDesk),
            cv.Optional(CONF_CALIBRATION): BINARY_SENSOR_CONFIG,
            cv.Optional(CONF_MOVING): BINARY_SENSOR_CONFIG,
        }
    ),
    cv.has_at_least_one_key(CONF_CALIBRATION, CONF_MOVING),
)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_DESK_ID])

    if calibration_config := config.get(CONF_CALIBRATION):
        var = await binary_sensor.new_binary_sensor(calibration_config)
        cg.add(parent.register_calibration_sensor(var))

    if moving_config := config.get(CONF_MOVING):
        var = await binary_sensor.new_binary_sensor(moving_config)
        cg.add(parent.register_moving_sensor(var))
