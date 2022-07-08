// #ifndef SCREEN_H
#define SCREEN_H

using namespace std;

#include <Arduino.h>
#include <U8g2lib.h>
#include <string>

// U8G2 ####
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

extern U8G2_SSD1306_128X64_NONAME_F_SW_I2C screen;

extern const uint8_t *US_FONT;
extern const uint8_t *BS_FONT;
extern const uint8_t *CLOCK_FONT;
extern const uint16_t CLOCK_FONT_ENCODING_START;

extern volatile int menuState;
extern array<string, 3> menuItems;

volatile void draw_upper_menu(int nextMenu);

void draw_upper_text();
void draw_lower_text();
void draw_te_hu();
void draw_time();
void u8g2_prepare();

// #endif
