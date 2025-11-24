#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "DHT.h"

// Wi-Fi
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Broker MQTT TLS
const char* mqtt_server = "test.mosquitto.org";
const int mqtt_port = 8883;
const char* mqtt_topic = "esp32/dht22";

// DHT22
#define DHTPIN 13
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

WiFiClientSecure espClient;
PubSubClient client(espClient);

void setup_wifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connecté !");
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32_DHT_Pub")) {
      Serial.println("Connecté au broker !");
    } else {
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  espClient.setInsecure(); // Pour test
  client.setServer(mqtt_server, mqtt_port);
  dht.begin();
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (!isnan(h) && !isnan(t)) {
    String payload = String(t) + "," + String(h);
    client.publish(mqtt_topic, payload.c_str());
    Serial.println("Données publiées: " + payload);
  } else {
    Serial.println("Erreur lecture DHT");
  }
  delay(5000);
}
