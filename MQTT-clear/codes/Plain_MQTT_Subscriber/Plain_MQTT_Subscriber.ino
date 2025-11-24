#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ====== Wi-Fi ======
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// ====== Broker MQTT ======
const char* mqtt_server = "test.mosquitto.org";
const int mqtt_port = 1883;
const char* mqtt_topic = "esp32/dht22";

// ====== LCD ======
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ====== MQTT client ======
WiFiClient espClient;
PubSubClient client(espClient);

// ====== Callback ======
void callback(char* topic, byte* payload, unsigned int length) {
  payload[length] = '\0'; // terminer la chaîne
  String msg = String((char*)payload);

  int commaIndex = msg.indexOf(',');
  String temp = msg.substring(0, commaIndex);
  String hum = msg.substring(commaIndex + 1);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: " + temp + " C");
  lcd.setCursor(0, 1);
  lcd.print("Hum: " + hum + " %");

  Serial.println("Message reçu: " + msg);
}

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
    if (client.connect("ESP32_LCD_Sub")) {
      Serial.println("Connecté au broker !");
      client.subscribe(mqtt_topic);
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
  lcd.init();
  lcd.backlight();
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

// ====== Loop ======
void loop() {
  if (!client.connected()) reconnect();
  client.loop();
}
