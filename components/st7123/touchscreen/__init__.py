import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c, touchscreen
from esphome.const import CONF_ID, CONF_INTERRUPT_PIN
from esphome import pins
from .. import st7123_ns

DEPENDENCIES = ["i2c"]

ST7123Touchscreen = st7123_ns.class_(
    "ST7123Touchscreen",
    touchscreen.Touchscreen,
    i2c.I2CDevice,
)

CONFIG_SCHEMA = touchscreen.TOUCHSCREEN_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(ST7123Touchscreen),
        cv.Required(CONF_INTERRUPT_PIN): pins.gpio_input_pin_schema,
    }
).extend(i2c.i2c_device_schema(0x24))


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await touchscreen.register_touchscreen(var, config)
    await i2c.register_i2c_device(var, config)

    interrupt_pin = await cg.gpio_pin_expression(config[CONF_INTERRUPT_PIN])
    cg.add(var.set_interrupt_pin(interrupt_pin))
