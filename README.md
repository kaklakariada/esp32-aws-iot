# ESP32 AWS IoT sensor

This project uses an ESP32 to publish temperature, pressure and humidity to AWS IoT via MQTT.

## Hardware

* ESP32 Dev kit
* BME280 sensor

### Wiring

| ESP32 | BME280 |
|-------|--------|
| 3V3   | VCC    |
| GND   | GND    |
| G22   | SCL    |
| G21   | SDA    |

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
