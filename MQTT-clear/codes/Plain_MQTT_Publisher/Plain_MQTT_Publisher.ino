#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

// ====== Wi-Fi ======
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// ====== Broker MQTT ======
const char* mqtt_server = "test.mosquitto.org";
const int mqtt_port = 1883;
const char* mqtt_topic = "esp32/dht22";

// ====== DHT22 ======
#define DHTPIN 13        
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// ====== MQTT client ======
WiFiClient espClient;
PubSubClient client(espClient);

// ====== Wi-Fi ======
void setup_wifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connecté !");
}

// ====== Reconnexion MQTT ======
void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32_DHT_Pub")) {
      Serial.println("Connecté au broker !");
    } else {
      Serial.print("Échec, rc=");
      Serial.print(client.state());
      Serial.println(" Nouvelle tentative dans 5s");
      delay(5000);
    }
  }
}

// ====== Setup ======
void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  dht.begin();
}

// ====== Loop ======
void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  float t = dht.readTemperature();
  float h = dht.readHumidity();

  if (!isnan(t) && !isnan(h)) {
    String payload = "TEM: "+String(t) + "," +"HUM: "+ String(h);
    client.publish(mqtt_topic, payload.c_str());
    Serial.println("Données publiées: " + payload);
  } else {
    Serial.println("Erreur lecture DHT");
  }
  delay(5000);
}
