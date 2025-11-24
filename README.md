# Smart Device Traffic Analysis: MQTT vs MQTTS

##  Overview
This project demonstrates how unencrypted MQTT exposes data and how enabling TLS (MQTTS) secures IoT communication.

##  Setup
We used:
- 2 ESP32 (Publisher + Subscriber)
- DHT22 sensor
- MQTT broker (Mosquitto)
- Wokwi simulation
- Wireshark traffic analysis

##  Structure
- MQTT-clear/ → unencrypted MQTT example
- MQTTS-secure/ → encrypted MQTT over TLS example
- captures/ → pcap files for Wireshark

##  Conclusion
Using MQTT alone exposes all data in plaintext. Using MQTT over TLS ensures encrypted and secure communication.
