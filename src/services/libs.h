#ifndef LIBS_H
#define LIBS_H

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

extern WiFiUDP ntpUDP;
extern NTPClient ntpClient;
extern sensor_t sensor;
extern DHT_Unified dht;

#endif LIBS_H