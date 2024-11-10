#include <ESP8266WiFi.h>
#include <string>
#include "DHT.h"

#define DHTTYPE DHT11

const char* ssid = "21308";           // Имя вашей Wi-Fi сети
const char* password = "19032004SSS";   // Пароль от вашей Wi-Fi сети

const char* serverIP = "192.168.1.153";       // IP-адрес сервера
const int serverPort = 1111;              // Порт сервера

WiFiClient client;

uint8_t DHTPin = 4; 
               
// инициализация датчика DHT.
DHT dht(DHTPin, DHTTYPE); 

struct TempHumid                                                                              
{   
    int Temperature;
    int Humidity;
};
TempHumid  _TempHumid;

void setup() {
  //Инициализируем датчик DHT11
   pinMode(DHTPin, INPUT);
   dht.begin();

   _TempHumid.Humidity;
   _TempHumid.Temperature; 

    Serial.begin(115200);
    delay(100);

    // Подключение к Wi-Fi
    Serial.print("Подключение к Wi-Fi");
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.println("Подключение к Wi-Fi");

    // Подключение к серверу
    Serial.print("Подключение к серверу...");
    if (client.connect(serverIP, serverPort)) {
        Serial.println("Подключен к серверу.");
    } else {
        Serial.println("Ошибка подключения.");
    }
}

void sendMessage(const String& message) {
    if (client.connected()) {
        int msgSize = message.length();
        
        // Отправка размера сообщения (4 байта)
        client.write((uint8_t*)&msgSize, sizeof(int));
        
        // Отправка самого сообщения
        client.write(message.c_str(), msgSize);
        
        Serial.println("Отправлено: " + message);
    } else {
        Serial.println("Нет подключния к серверу.");
    }
}

void loop() {
   _TempHumid.Temperature = dht.readTemperature(); // получаем значение температуры
   _TempHumid.Humidity = dht.readHumidity();       // получаем значение влажности
   
  char TempBuff[25];
  //char HumBuff[25];
  sprintf(TempBuff,"\n %d *C \n %d %%", _TempHumid.Temperature,  _TempHumid.Humidity);
    // Пример сообщения для отправки
   
    sendMessage(TempBuff);

    delay(2000);  // Задержка перед следующей отправкой
}