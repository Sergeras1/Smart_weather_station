#include <ESP8266WiFi.h>
#include "DHT.h"

#define DHTTYPE DHT11

const char* ssid = "21308";
const char* password = "19032004SSS";

const char* serverIP = "192.168.1.153";
const int serverPort = 1111;

WiFiClient client;

uint8_t DHTPin = 4;
DHT dht(DHTPin, DHTTYPE);

void setup() {
    Serial.begin(115200);
    dht.begin();

    WiFi.begin(ssid, password);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to Wi-Fi");

    connectToServer();
}

void connectToServer() {
    Serial.print("Connecting to server...");
    if (client.connect(serverIP, serverPort)) {
        Serial.println("Connected to server.");
    } else {
        Serial.println("Failed to connect to server.");
    }
}

void sendDataToClient() {
    if (!client.connected()) {
        Serial.println("Client disconnected, reconnecting...");
        connectToServer();
        return;
    }

    float temp = dht.readTemperature();
    float hum = dht.readHumidity();

    if (!isnan(temp) && !isnan(hum)) {
        String message = "T:" + String(temp, 1) + "\nH:" + String(hum, 1) + "\n";
        client.print(message);
        Serial.println("Sent: " + message);
    } else {
        Serial.println("Failed to read from DHT sensor!");
    }
}

void loop() {
    sendDataToClient();
    delay(2000);
}