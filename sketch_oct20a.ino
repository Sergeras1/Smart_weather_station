#include <ESP8266WiFi.h>           // Библиотека WiFi связь 
#include <WiFiUdp.h>               // Udp-Сокет 
#include <string>
#include <iostream>
#include <sstream>
#include "DHT.h"

#define DHTTYPE DHT11

using std::string;
using std::stringstream;
using std::cout;
using std::endl;


//-------Датчик DHT11---------//
// датчик DHT
uint8_t DHTPin = 4; 
               
// инициализация датчика DHT.
DHT dht(DHTPin, DHTTYPE);                



//----------------//
unsigned int localPort = 8888; // Локальный порт прослушевания сети 

const char *ssid     = "21308"; // SSID имя WiFi точки-доступа 
const char* password = "19032004SSS"; // Пароль WiFii  


IPAddress local_ip(192,168,1,100);  // IP-адрес нашой платы статичский 
IPAddress gateway(192,168,1,1);  // IP-адрес шлюза  
IPAddress subnet(255,255,255,0); // Подсеть 

#define PACKET_MAX_SIZE 255         // Масимальный размер пакета-данных 
char packetBuffer[PACKET_MAX_SIZE]; // Буферы для приема и отправки пакета-данных,

WiFiUDP UDP; // UPD-сокет обект 

struct TempHumid                                                                              
{   
    int Temperature;
    int Humidity;
};
TempHumid  _TempHumid;



void setup() 
{
  //Инициализируем датчик DHT11
   pinMode(DHTPin, INPUT);
   dht.begin();

   _TempHumid.Humidity;
   _TempHumid.Temperature; 
    // Запускаем сериал порт на скрости 115200.импульсов\сек 
    Serial.begin(115200);                         
  
    // присваиваем статичесий IP адрес
    WiFi.mode(WIFI_STA); // режим клиента
    WiFi.config(local_ip, gateway, subnet);
    WiFi.begin(ssid, password);
  
   // Ждем потключение к WiFi 
    Serial.println("Connecting to Wifi");
    int count_n = 0;
    while (WiFi.status() != WL_CONNECTED) 
    {   
      delay(500);
      Serial.print(".");
      delay(500);
      if(count_n++ > 10) break;
    }

    // Если не потключились к WiFi , 
    // то создаем точку доступу WiFi в самом устройстве с
    // исходными SSID | Password
    if(WiFi.status() != WL_CONNECTED)
    {
      boolean result =
      WiFi.softAP(ssid, password);                  // Устанавливаем режым точки доступа WiFi
      WiFi.softAPConfig(local_ip, gateway, subnet); // Устанавливаем статические IP-адреса 
      delay(100);                                   // Ждем 100 милисекунд 
  
      // вывод данных о сети 
      IPAddress myIP = WiFi.softAPIP();             // IP-адрес робокара
      Serial.print("AP IP address: ");             
      Serial.println(myIP);
      if(result == true) Serial.println("Ready"); else Serial.println("Failed!");
    }
 
    // вывод информации о сервере    
    Serial.print("Connected! IP address: ");
    Serial.println(WiFi.localIP());
    Serial.printf("UDP server on port %d\n", localPort);
    
    // Начинаем слушать порт 
    UDP.begin(localPort);                      
}

void loop()
{
   _TempHumid.Temperature = dht.readTemperature(); // получаем значение температуры
   _TempHumid.Humidity = dht.readHumidity();       // получаем значение влажности
   
  char TempBuff[256];
  sprintf(TempBuff,"-- Temperature: %d-- \n Humidity: %d", _TempHumid.Temperature, _TempHumid.Humidity);
  Serial.printf( TempBuff );
  char HumBuff[256];
        // Отсылаем данные температуры
        UDP.beginPacket(UDP.remoteIP(), UDP.remotePort());
        UDP.write(TempBuff);
        UDP.endPacket();
        // Отслыаем данные о влажности
       /*UDP.beginPacket(UDP.remoteIP(), UDP.remotePort());
        UDP.write(HumBuff);
        UDP.endPacket();*/ 

    delay(1000); // Ждем 10 милисекунд 
}