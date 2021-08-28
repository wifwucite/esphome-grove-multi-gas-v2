#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace grove_multigas_v2 {

class GroveMultiGasV2Component : public PollingComponent, public i2c::I2CDevice {
public:
  void set_NO2_sensor(sensor::Sensor* no2_sensor) { this->no2_sensor = no2_sensor; }
  void set_C2H5OH_sensor(sensor::Sensor* c2h5oh_sensor) { this->c2h5oh_sensor = c2h5oh_sensor; }
  void set_VOC_sensor(sensor::Sensor* voc_sensor) { this->voc_sensor = voc_sensor; }
  void set_CO_sensor(sensor::Sensor* co_sensor) { this->co_sensor = co_sensor; }

  float get_setup_priority() const override;
  void setup() override;
  void dump_config() override;
  void update() override;

private:
  bool write_byte(uint8_t byte);
  bool enable_preheating();
  bool read_value32(uint8_t i2c_register, uint32_t& value);
  void update_gas_sensor_value(sensor::Sensor* gas_sensor, uint8_t i2c_register);

private:
  bool has_error{false};
  bool preheated{false};

  sensor::Sensor* no2_sensor{nullptr};
  sensor::Sensor* c2h5oh_sensor{nullptr};
  sensor::Sensor* voc_sensor{nullptr};
  sensor::Sensor* co_sensor{nullptr};
};

}  // namespace grove_multigas_v2
}  // namespace esphome
