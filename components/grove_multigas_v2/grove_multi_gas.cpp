#include "grove_multi_gas.h"

#include "esphome/core/log.h"

// see https://wiki.seeedstudio.com/Grove-Multichannel-Gas-Sensor-V2
// and https://github.com/Seeed-Studio/Seeed_Arduino_MultiGas

#define GM_RESOLUTION 1023

// commands / registers
#define GM_102B 0x01
#define GM_302B 0x03
#define GM_502B 0x05
#define GM_702B 0x07
#define CHANGE_I2C_ADDR 0x55
#define WARMING_UP 0xFE
#define WARMING_DOWN  0xFF

namespace esphome {
namespace grove_multigas_v2 {

static const char *const TAG = "grove_multigas_v2.sensor";

float GroveMultiGasV2Component::get_setup_priority() const { return setup_priority::DATA; }

void GroveMultiGasV2Component::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Grove MultiGas V2...");
  bool ok = enable_preheating();
  if (!ok) {
    ESP_LOGE(TAG, "could not activate preheating");
    mark_failed();
  }
}

void GroveMultiGasV2Component::dump_config() {
  ESP_LOGCONFIG(TAG, "Grove MultiGas V2:");

  LOG_I2C_DEVICE(this);

  if (is_failed()) {
    ESP_LOGCONFIG(TAG, "  Failed!");
  }

  LOG_SENSOR("  ", "NO2", no2_sensor);
  LOG_SENSOR("  ", "C2H5OH", c2h5oh_sensor);
  LOG_SENSOR("  ", "VOC", voc_sensor);
  LOG_SENSOR("  ", "CO", co_sensor);
}

void GroveMultiGasV2Component::update() {
  update_gas_sensor_value(no2_sensor, GM_102B);
  update_gas_sensor_value(c2h5oh_sensor, GM_302B);
  update_gas_sensor_value(voc_sensor, GM_502B);
  update_gas_sensor_value(co_sensor, GM_702B);
}

bool GroveMultiGasV2Component::enable_preheating() {
  return send_command(WARMING_UP);
}

bool GroveMultiGasV2Component::send_command(uint8_t i2c_command) {
  return write_bytes(i2c_command, nullptr, 0);
}

void GroveMultiGasV2Component::update_gas_sensor_value(sensor::Sensor* gas_sensor, uint8_t i2c_register) {
  if (!gas_sensor) {
    return;
  }

  ESP_LOGV(TAG, "Reading %s gas sensor", gas_sensor->get_name().c_str());
  uint32_t adc_value;
  bool ok = read_value32(i2c_register, adc_value);
  if (ok) {
    if (this->publish_raw_values) {
      gas_sensor->publish_state(adc_value);
    } else {
      float ppm = (adc_value * 3.3) / GM_RESOLUTION;
      gas_sensor->publish_state(ppm);
    }
  } else {
    gas_sensor->publish_state(NAN);
  }
}

bool GroveMultiGasV2Component::read_value32(uint8_t i2c_register, uint32_t& value) {
  value = 0;

  uint8_t buffer[4];
  bool ok = read_register(i2c_register, buffer, sizeof(buffer)) == i2c::ErrorCode::ERROR_OK;
  if (!ok) {
    return false;
  }

  for (int i = 0; i < 4; ++i) {
    value += buffer[i] << (8 * i);
  }
  return true;
}

}  // namespace grove_multigas_v2
}  // namespace esphome
