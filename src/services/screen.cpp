#include <services/screen.h>
#include <services/utils.h>
#include <services/libs.h>

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define US_XPOS 0
#define US_YPOS 0

#define BS_XPOS 0
#define BS_YPOS 16

#define US_HEIGHT 16
#define US_WIDTH 128

#define BS_HEIGHT 48
#define BS_WIDTH 128

#define US_FONT_HEIGHT 14
#define BS_FONT_HEIGHT 14

// assume 4x6 font, define width and height
#define U8LOG_WIDTH 32
#define U8LOG_HEIGHT 10

using namespace std;

U8G2_SSD1306_128X64_NONAME_F_SW_I2C screen(U8G2_R0, /* clock=*/SCL, /* data=*/SDA, /* reset=*/U8X8_PIN_NONE); // All Boards without Reset of the Display

const uint8_t *US_FONT = u8g2_font_cu12_tr;
const uint8_t *BS_FONT = u8g2_font_logisoso38_tf;
const uint8_t *CLOCK_FONT = u8g2_font_7_Seg_33x19_mn;
const uint16_t CLOCK_FONT_ENCODING_START = 48;

volatile int menuState = 1;

array<string, 3> menuItems = {"CLOCK", "TEMPERATURE", "STOCKS"};

uint16_t getClockFontEncoding(uint16_t input)
{
  return uint16_t(CLOCK_FONT_ENCODING_START + input);
}

void collect_digits(vector<int> &digits, unsigned int num)
{
  if (num > 9)
  {
    collect_digits(digits, num / 10);
  }
  // digits.push_back(num % 10);
  digits.push_back(num % 10);
}

// STATE METHODS
volatile void draw_upper_menu(int nextMenu)
{
  screen.clearBuffer();
  int labelsMargin = 6;
  screen.setFont(US_FONT);
  u8g2_uint_t absoluteMarginLeft = 0;
  for (size_t i = 0; i < menuItems.size(); i++)
  {
    string label = slice(menuItems[i], 0, 2);
    // absoluteMarginLeft = u8g2Instance.drawStr((US_XPOS + absoluteMarginLeft), US_YPOS, label.c_str()) + absoluteMarginLeft + labelsMargin;
    if (i == nextMenu)
    {
      screen.setFontMode(0);
      screen.setDrawColor(0);
      u8g2_uint_t stringWidth = screen.drawStr((US_XPOS + absoluteMarginLeft), US_YPOS, label.c_str());
      // u8g2Instance.drawBox(absoluteMarginLeft, US_YPOS, stringWidth, US_HEIGHT);
      absoluteMarginLeft = stringWidth + absoluteMarginLeft + labelsMargin;
    }
    else
    {
      screen.setFontMode(1);
      screen.setDrawColor(1);
      u8g2_uint_t stringWidth = screen.drawStr((US_XPOS + absoluteMarginLeft), US_YPOS, label.c_str());
      absoluteMarginLeft = stringWidth + absoluteMarginLeft + labelsMargin;
    }
  }
  screen.sendBuffer();
  // u8g2Instance.drawStr(US_XPOS, US_FONT_HEIGHT, "fuck");
}

void draw_upper_text(string input, int x = 0, int y = 0)
{
  // u8g2Instance.clearBuffer();
  screen.setDrawColor(1);
  screen.setDrawColor(1);
  screen.setFont(US_FONT);
  screen.drawStr(US_XPOS + x, US_YPOS + y, input.c_str());
  screen.updateDisplayArea(US_XPOS, US_YPOS, US_WIDTH, US_WIDTH);
}

void draw_lower_text(string input, int x = 0, int y = 0)
{
  screen.setDrawColor(1);
  screen.setDrawColor(1);
  // u8g2Instance.clearBuffer();
  screen.setFont(BS_FONT);
  screen.drawUTF8(BS_XPOS + x, BS_YPOS + y, input.c_str());
  screen.sendBuffer();
  // u8g2Instance.updateDisplayArea(BS_XPOS, BS_YPOS, BS_WIDTH, BS_WIDTH);
}

void draw_te_hu()
{
  Serial.println("Rendered temperature");
  screen.setFont(BS_FONT);
  u8g2_uint_t yStringPos = BS_YPOS + 4;
  u8g2_uint_t xStringPos = 0;

  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature))
  {
    Serial.println(F("Error reading temperature!"));
  }
  else
  {
    String temperature = String(
        round(event.temperature));
    String finaltemp = temperature + "°C";
    xStringPos = screen.drawUTF8(BS_XPOS, yStringPos, finaltemp.c_str()) + xStringPos;
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity))
  {
    Serial.println(F("Error reading humidity!"));
  }
  else
  {
    String humidity = String(
        round(event.temperature));
    String finalhum = humidity + " %";

    screen.setFont(US_FONT);
    screen.drawUTF8(xStringPos, yStringPos, finalhum.c_str());
  }
}

void draw_time()
{
  screen.setDrawColor(1);
  screen.setDrawColor(1);
  // u8g2Instance.setClipWindow(BS_XPOS, BS_YPOS, BS_WIDTH - 1, BS_HEIGHT - 1);
  screen.updateDisplayArea(screen.getBufferTileWidth() - 1, screen.getBufferTileHeight() - 1, BS_WIDTH, BS_HEIGHT);
  screen.setFont(CLOCK_FONT);
  int columnOffsetMargin = 8;
  string timestring = string(ntpClient.getFormattedTime().c_str());

  vector<int> hoursDigits = {};
  collect_digits(hoursDigits, ntpClient.getHours());
  fill_to(hoursDigits, 0, 2);

  vector<int> minutesDigits = {};
  collect_digits(minutesDigits, ntpClient.getMinutes());
  fill_to(minutesDigits, 0, 2);

  vector<int> secondsDigits = {};
  collect_digits(secondsDigits, ntpClient.getSeconds());
  fill_to(secondsDigits, 0, 2);

  u8g2_uint_t xStringPos = 0;

  // HOURS
  for (int i = 0; i < hoursDigits.size(); i++)
  {
    int encoding = getClockFontEncoding(hoursDigits[i]);
    xStringPos = screen.drawGlyph(xStringPos, BS_YPOS, encoding) + xStringPos;
  }

  // MINUTES
  screen.drawUTF8(xStringPos, BS_YPOS, ":");
  xStringPos = columnOffsetMargin + xStringPos;

  for (int i = 0; i < minutesDigits.size(); i++)
  {
    int encoding = getClockFontEncoding(minutesDigits[i]);
    xStringPos = screen.drawGlyph(xStringPos, BS_YPOS, encoding) + xStringPos;
  }
  // SECONDS
  // u8g2Instance.drawUTF8(xStringPos, BS_YPOS, ":");
  // xStringPos = columnOffsetMargin + xStringPos;
  // for (int i = 0; i < secondsDigits.size(); i++)
  // {
  //   int encoding = getClockFontEncoding(secondsDigits[i]);
  //   xStringPos = u8g2InstancedrawGlyph(xStringPos, BS_YPOS, encoding) + xStringPos;
  // }
  // u8g2InstanceupdateDisplay();
}

void u8g2_prepare(void)
{
  screen.begin();
  // u8g2.setFont(u8g2_font_tom_thumb_4x6_mf);	// set the font for the terminal window
  // u8g2.setFontRefHeightExtendedText();
  // u8g2.setDrawColor(1);
  screen.setFontPosTop();
  screen.setFontDirection(0);
}