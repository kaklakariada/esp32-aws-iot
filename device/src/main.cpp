#include "secrets.h"
#include <WiFiClientSecure.h>
#include <MQTTClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define PUBLISH_MEASURMENT_TOPIC   "sensor/measurment"
#define PUBLISH_MESSAGE_TOPIC   "sensor/message"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp32/sub"
#define SEALEVELPRESSURE_HPA (1013.25)

WiFiClientSecure net = WiFiClientSecure();
MQTTClient client = MQTTClient(256);
Adafruit_BME280 bme;

void messageHandler(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
}


void publishJson(const char* topic, StaticJsonDocument<200> doc) {
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer);

  bool success = client.publish(topic, jsonBuffer);
  if(!success) {
    Serial.print("Publish failed: ");
    Serial.println(success);
  }
}

void publishMessage(String type, String message)
{
  StaticJsonDocument<200> doc;
  doc["time"] = millis();
  doc["type"] = type;
  doc["message"] = message;
  publishJson(PUBLISH_MESSAGE_TOPIC, doc);
}

void configureWill() {
  StaticJsonDocument<200> doc;
  doc["type"] = "will";
  doc["message"] = "Thing disconnected";
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer);
  client.setWill(PUBLISH_MESSAGE_TOPIC, jsonBuffer, false, 0);
}

void connectAWS()
{
  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  // Connect to the MQTT broker on the AWS endpoint we defined earlier
  client.begin(AWS_IOT_ENDPOINT, 8883, net);

  configureWill();

  // Create a message handler
  client.onMessage(messageHandler);

  Serial.print("Connecting to AWS IOT as thing ");
  Serial.println(THINGNAME);

  while (!client.connect(THINGNAME)) {
    Serial.print(".");
    delay(100);
  }

  if(!client.connected()){
    Serial.println("AWS IoT Timeout!");
    return;
  }

  // Subscribe to a topic
  Serial.print("Subscribing to topic ");
  Serial.println(AWS_IOT_SUBSCRIBE_TOPIC);

  bool success = client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);

  Serial.print("Subscription success: ");
  Serial.println(success);

  Serial.println("AWS IoT Connected!");
  publishMessage("info", "Connected to AWS IoT");
}

void connectWifi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to Wi-Fi ");
  Serial.print(WIFI_SSID);
  Serial.println("...");

  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connected to Wi-Fi!");
}

void connectBme() {
  bool status = bme.begin(0x76);
  Serial.println("Connecting to BME280...");
  if (!status) {
    publishMessage("error", "Could not find a valid BME280 sensor, check wiring!");
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
  publishMessage("info", "Connected to BME280");
  Serial.println("Connected to BME280");
}

void publishMeasurment()
{
  StaticJsonDocument<200> doc;
  doc["time"] = millis();
  doc["temperature"] = bme.readTemperature();
  doc["pressure"] = bme.readPressure() / 100.0F;
  doc["altitude"] = bme.readAltitude(SEALEVELPRESSURE_HPA);
  doc["humidity"] = bme.readHumidity();
  publishJson(PUBLISH_MEASURMENT_TOPIC, doc);
}

void setup() {
  Serial.begin(115200);
  connectWifi();
  connectAWS();
  connectBme();
}

void loop() {
  publishMeasurment();
  client.loop();
  delay(1000);
}
