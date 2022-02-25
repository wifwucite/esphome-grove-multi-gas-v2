import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c, sensor
from esphome.const import (
    CONF_ID,
    STATE_CLASS_MEASUREMENT,
    CONF_UNIT_OF_MEASUREMENT,
    UNIT_PARTS_PER_MILLION,
    CONF_ACCURACY_DECIMALS,
    ICON_CHEMICAL_WEAPON,
)

CONF_RAW_VALUES = "raw_values"
CONF_NO2 = "NO2"
CONF_C2H5OH = "C2H5OH"
CONF_VOC = "VOC"
CONF_CO = "CO"

DEPENDENCIES = ["i2c"]

multigas_ns = cg.esphome_ns.namespace("grove_multigas_v2")
MultiGasComponent = multigas_ns.class_(
    "GroveMultiGasV2Component", cg.PollingComponent, i2c.I2CDevice
)

CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(MultiGasComponent),

            cv.Optional(CONF_RAW_VALUES, default=False): cv.boolean,

            cv.Optional(CONF_NO2): sensor.sensor_schema(
                unit_of_measurement=UNIT_PARTS_PER_MILLION,
                icon=ICON_CHEMICAL_WEAPON,
                accuracy_decimals=1,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_C2H5OH): sensor.sensor_schema(
                unit_of_measurement=UNIT_PARTS_PER_MILLION,
                icon=ICON_CHEMICAL_WEAPON,
                accuracy_decimals=1,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_VOC): sensor.sensor_schema(
                unit_of_measurement=UNIT_PARTS_PER_MILLION,
                icon=ICON_CHEMICAL_WEAPON,
                accuracy_decimals=1,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_CO): sensor.sensor_schema(
                unit_of_measurement=UNIT_PARTS_PER_MILLION,
                icon=ICON_CHEMICAL_WEAPON,
                accuracy_decimals=1,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
        }
    )
    .extend(cv.polling_component_schema("500ms"))
    .extend(i2c.i2c_device_schema(0x08)),
)


def patch_sensor_config_for_raw_values(config, raw_values):
    if raw_values:
        config = config.copy()
        config[CONF_UNIT_OF_MEASUREMENT] = "adc"
        config[CONF_ACCURACY_DECIMALS] = 0
        return config
    else:
        return config

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    raw_values = config[CONF_RAW_VALUES]
    cg.add(var.set_publish_raw_values(raw_values))

    if CONF_NO2 in config:
        sensor_config = patch_sensor_config_for_raw_values(config[CONF_NO2], raw_values)
        sens = await sensor.new_sensor(sensor_config)
        cg.add(var.set_NO2_sensor(sens))

    if CONF_C2H5OH in config:
        sensor_config = patch_sensor_config_for_raw_values(config[CONF_C2H5OH], raw_values)
        sens = await sensor.new_sensor(sensor_config)
        cg.add(var.set_C2H5OH_sensor(sens))

    if CONF_VOC in config:
        sensor_config = patch_sensor_config_for_raw_values(config[CONF_VOC], raw_values)
        sens = await sensor.new_sensor(sensor_config)
        cg.add(var.set_VOC_sensor(sens))

    if CONF_CO in config:
        sensor_config = patch_sensor_config_for_raw_values(config[CONF_CO], raw_values)
        sens = await sensor.new_sensor(sensor_config)
        cg.add(var.set_CO_sensor(sens))

    # https://platformio.org/lib/show/6306/Grove%20-%20Laser%20PM2.5%20Sensor%20HM3301
    # cg.add_library("6306", "1.0.3")
