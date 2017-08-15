#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>
#include <JQ6500.h>
#include <Microduino_Key.h>
#include "userDef.h"

#define PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(14, PIN, NEO_GRB + NEO_KHZ800);

uint32_t color[9] =
{
  strip.Color(0, 0, 0), strip.Color(255, 0, 0), strip.Color(255, 165, 0), strip.Color(255, 255, 0), strip.Color(0, 255, 0), strip.Color(0, 127, 255), strip.Color(0, 0, 255), strip.Color(139, 0, 255), strip.Color(255, 255, 255)
};

int sensorPin[7] = {
  A6, 8, 10, 12, A0, A2, 0
};

SoftwareSerial mySerial(2, 3);
JQ6500 AUDIO(&mySerial);

Key *pKeyArr[7];
uint8_t ledState[7] =
{0, 0, 0, 0, 0, 0, 0};

bool isKeyDown[7] =
{0, 0, 0, 0, 0, 0, 0};

void setup() {
  
  for (int i = 0; i < 7; i++)
    pKeyArr[i] = new Key(sensorPin[i], INPUT);

  strip.begin();  //初始化LED
  strip.show(); // Initialize all pixels to 'off'

  AUDIO.init(DEVICE_FLASH, MODE_ONE_STOP, MUSIC_VOL);
  delay(600);

  for (int i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, color[0]);
  }
  strip.show();

  for (int i = 0; i < 7; i++)
  {
    strip.setPixelColor(2 * i, color[i + 1]);
    strip.setPixelColor(2 * i + 1, color[i + 1]);
    strip.show();
    AUDIO.choose(i + 1);
    delay(800);
  }

  for (int i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, color[0]);
  }
  strip.show();

}

void loop() {

  switch (pKeyArr[0]->read(0, 512))
  {
    case SHORT_PRESS:
      ledState[0] = (ledState[0] + 1) % 8;
      strip.setPixelColor(0, color[1+(ledState[0] & 0x8F)]);
      strip.setPixelColor( 1, color[1+(ledState[0] & 0x8F)]);

      strip.show();
      AUDIO.choose(1);
      break;
    case LONG_PRESS:
      if (!isKeyDown[0])
      {
        isKeyDown[0] = true;
        if (ledState[0] & 0x80 )
        {
          ledState[0] &= ~(1 << 7);
          strip.setPixelColor(0, color[1+(ledState[0] & 0x8F)]);
          strip.setPixelColor(1, color[1+(ledState[0] & 0x8F)]);
        }
        else
        {
          ledState[0] |= 1 << 7;
          strip.setPixelColor(0, color[0]);
          strip.setPixelColor(1, color[0]);
        }
        strip.show();
      }
      break;
    default:
      isKeyDown[0] = false;
      break;
  }

  for (int i = 1; i < 7; i++)
  {
    switch (pKeyArr[i]->read()) {
      case SHORT_PRESS:
        ledState[i] = (ledState[i] + 1) % 8;
        strip.setPixelColor(2 * i, color[1+(ledState[i] & 0x8F)]);
        strip.setPixelColor(2 * i + 1, color[1+(ledState[i] & 0x8F)]);
        strip.show();
        AUDIO.choose(i + 1);

        break;
      case LONG_PRESS:

        if (!isKeyDown[i])
        {
          isKeyDown[i] = true;
          if (ledState[i] & 0x80 )
          {
            ledState[i] &= ~(1 << 7);
            strip.setPixelColor(2 * i, color[1+(ledState[i] & 0x8F)]);
            strip.setPixelColor(2 * i + 1, color[1+(ledState[i] & 0x8F)]);
          }
          else
          {
            ledState[i] |= 1 << 7;
            strip.setPixelColor(2 * i, color[0]);
            strip.setPixelColor(2 * i + 1, color[0]);
          }
          strip.show();
        }
        break;
      default:
        isKeyDown[i] = false;
    }
  }

}
