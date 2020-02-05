# ESP32 AWS IoT sensor

This project uses an ESP32 to publish temperature, pressure and humidity to AWS IoT via MQTT.

## Hardware

* ESP32 Dev kit
* BME280 sensor
* SDS011 sensor

### Wiring

#### BME280

| ESP32 pins     | BME280 pins |
|----------------|-------------|
| 3V3            | VCC         |
| GND            | GND         |
| G22 (IC2 SCL)  | SCL         |
| G21 (IC2 SDA)  | SDA         |

#### SDS011

| ESP32 pins | SDS011 pins |
|------------|-------------|
| 5V         | 5V          |
| GND        | GND         |
| G16 (RX2)  | RX          |
| G17 (TX2)  | TX          |

## Setup

1. Create a thing with certificate and policy in AWS IoT
1. Install PlatformIO for VSCode
1. Open `device` project in VSCode
1. Copy `device/include/secrets.h.template` to `device/include/secrets.h` and insert the following:
   * `THINGNAME`: choose a name for your device
   * `WIFI_SSID`: name of your Wifi network
   * `WIFI_PASSWORD`: password for your Wifi network
   * `AWS_IOT_ENDPOINT`: get it from AWS IoT settings
   * `AWS_CERT_CA`: get it from https://www.amazontrust.com/repository/AmazonRootCA1.pem
   * `AWS_CERT_CRT`: device certificate
   * `AWS_CERT_PRIVATE`: private key from device certificate
1. Build and upload with PlatformIO
1. Verify that messages are published to AWS IoT

## Example message

```json
{
  "time": 669776,
  "temperature": 21.83,
  "humidity": 36.62695,
  "pressure": 997.652,
  "altitude": 130.6805,
  "hall": 4,
  "sds_p10": 19.2,
  "sds_p25": 13.8
}
```