#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

#define DHT_PIN D7
#define DHTTYPE DHT11

WiFiUDP ntpUDP;
NTPClient ntpClient(ntpUDP);
sensor_t sensor;
DHT_Unified dht(DHT_PIN, DHTTYPE);