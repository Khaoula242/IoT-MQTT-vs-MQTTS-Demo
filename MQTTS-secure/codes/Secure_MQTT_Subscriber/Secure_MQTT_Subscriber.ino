#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Wi-Fi
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Broker MQTT TLS
const char* mqtt_server = "test.mosquitto.org";
const int mqtt_port = 8883;
const char* mqtt_topic = "esp32/dht22";

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

WiFiClientSecure espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
  payload[length] = '\0'; // Terminer la chaîne
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
    if (client.connect("ESP32_LCD_Sub")) {
      Serial.println("Connecté au broker !");
      client.subscribe(mqtt_topic);
    } else {
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  setup_wifi();
  espClient.setInsecure();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();
}
