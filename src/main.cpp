#include <services/libs.h>

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

#include <services/utils.h>
#include <services/screen.h>
#include <U8g2lib.h>

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#include <string>
#include <array>
#include <vector>

// INTERRUPT PINS
#define NEXTMENU_PIN D4

// DHT SENSOR ####
#define DHT_PIN D7
#define DHTTYPE DHT11

// WIFI ####
#define LGD_HOME_SSID "Livebox-CE89"
#define LGD_HOME_PASS "29CF5F85C6AA2A81C7A76699C0"

#define TLS_HOME_SSID "SFR_39A8"
#define TLS_HOME_PASS "3xvf28dfrk3vth42rsya"

using namespace std;

// // const uint8_t *US_FONT = u8g2_font_cu12_tr;
// const uint8_t *US_FONT = u8g2_font_cu12_tr;
// const uint8_t *BS_FONT = u8g2_font_logisoso38_tf;
// const uint8_t *CLOCK_FONT = u8g2_font_7_Seg_33x19_mn;
// const uint16_t CLOCK_FONT_ENCODING_START = 48;

// U8G2LOG u8g2log
// DATA METHODS

volatile void draw_state(int stateInput)
{
  screen.clearBuffer();
  draw_upper_menu(stateInput);
  switch (stateInput)
  {
  // CLOCK
  case 0:
    draw_time();
    break;
  case 1:
    draw_te_hu();
    /* code */
    break;
  case 2:
    /* code */
    break;
  default:
    break;
  }
  screen.sendBuffer();
}

// SCREEN SETUP ####


uint32_t delayMS;

// INTERRUPTS HANDLERS
// ICACHE_RAM void detectNextButton() {
ICACHE_RAM_ATTR void detectNextButton()
{
  // Serial.print("handled btn press");

  if (menuState < menuItems.size())
  {
    menuState = menuState + 1;
  }
  else
  {
    menuState = 0;
  }

  draw_upper_menu(menuState);
  draw_state(menuState);
}

void setup(void)
{
  // SERIAL ####
  if (Serial)
  {
    Serial.begin(9600);
  }

  // DHT ####
  dht.begin();
  Serial.println(F("DHTxx Unified Sensor Example"));
  // Print temperature sensor details.
  dht.temperature().getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.println(F("Temperature Sensor"));
  Serial.print(F("Sensor Type: "));
  Serial.println(sensor.name);
  Serial.print(F("Driver Ver:  "));
  Serial.println(sensor.version);
  Serial.print(F("Unique ID:   "));
  Serial.println(sensor.sensor_id);
  Serial.print(F("Max Value:   "));
  Serial.print(sensor.max_value);
  Serial.println(F("°C"));
  Serial.print(F("Min Value:   "));
  Serial.print(sensor.min_value);
  Serial.println(F("°C"));
  Serial.print(F("Resolution:  "));
  Serial.print(sensor.resolution);
  Serial.println(F("°C"));
  Serial.println(F("------------------------------------"));
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println(F("Humidity Sensor"));
  Serial.print(F("Sensor Type: "));
  Serial.println(sensor.name);
  Serial.print(F("Driver Ver:  "));
  Serial.println(sensor.version);
  Serial.print(F("Unique ID:   "));
  Serial.println(sensor.sensor_id);
  Serial.print(F("Max Value:   "));
  Serial.print(sensor.max_value);
  Serial.println(F("%"));
  Serial.print(F("Min Value:   "));
  Serial.print(sensor.min_value);
  Serial.println(F("%"));
  Serial.print(F("Resolution:  "));
  Serial.print(sensor.resolution);
  Serial.println(F("%"));
  Serial.println(F("------------------------------------"));

  // SCREEN ####
  u8g2_prepare();
  screen.clearBuffer();

  // PINS ####
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(NEXTMENU_PIN, INPUT);

  // WIFI ####
  WiFi.begin(TLS_HOME_SSID, TLS_HOME_PASS);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  // NTP ####
  int hours = 2;
  int timeOffset = 60 * 60 * hours;
  ntpClient.update();
  ntpClient.setTimeOffset(timeOffset);

  // INTERRUPTS SETUP
  attachInterrupt(digitalPinToInterrupt(NEXTMENU_PIN), detectNextButton, FALLING);

  // MENU
  draw_upper_menu(menuState);
}

void blink()
{
  digitalWrite(LED_BUILTIN, HIGH);
  delay(300);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop(void)
{
  draw_state(menuState);
  blink();
  delay(10000);
}