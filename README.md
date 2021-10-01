This component provides a sensor component for [ESPHome](https://esphome.io) for the [Grove multichannel gas sensor V2](https://wiki.seeedstudio.com/Grove-Multichannel-Gas-Sensor-V2/). It has the ability to detect a variety of gases: Carbon monoxide (CO), Nitrogen dioxide (NO2), Ethyl alcohol(C2H5CH), Volatile Organic Compounds (VOC). According to the specifications it is rather a qualitive than a quantitive sensor. But this is enough for fun projects like building an [artificial nose](https://blog.benjamin-cabe.com/2021/08/03/how-i-built-a-connected-artificial-nose).

## Installation

In order to connect the Grove multi gas sensor to an ESP32 I recommend [this cable](https://www.seeedstudio.com/Grove-4-pin-Female-Jumper-to-Grove-4-pin-Conversion-Cable-5-PCs-per-PAck.html). You connect the black female jumper to ground, the red one to 3.3V, the yellow one to pin 25, and the white one to pin 26. Then you plug the gove connector into the sensor.

To configure the sensor for ESPHome you do not need to download or copy anything manually, you can just refer to this external component from the yaml file like this:

```yaml
external_components:
  - source: github://wifwucite/esphome-grove-multi-gas-v2

i2c:
  sda: GPIO26 # white grove cable‚
  scl: GPIO25 # yellow grove cable

sensor:
  - platform: grove_multigas_v2
    NO2:
      name: "NO2"
    C2H5OH:
      name: "ethanol"
    VOC:
      name: "VOC"
    CO:
      name: "CO"
    update_interval: 25ms # default is 500ms
```
This is it, happy smelling!