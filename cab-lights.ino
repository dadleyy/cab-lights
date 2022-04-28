#include <Arduino.h>
#include <Wire.h>

#include "Adafruit_VCNL4010.h"
#include "Adafruit_DotStar.h"
#include "Adafruit_NeoPixel.h"

constexpr const uint8_t dotstar_data = 7;
constexpr const uint8_t dotstar_clock = 8;
constexpr const uint8_t neo_pin = 4;
constexpr const uint8_t red_led = 13;

constexpr const uint8_t frame_delay = 50;
constexpr const uint16_t max_idle_lit_frames = (1000 / frame_delay) * 10;

bool failed = false;

uint16_t lit_frames = 0;
bool lit = false;

Adafruit_DotStar dot(1, dotstar_data, dotstar_clock, DOTSTAR_BRG);
Adafruit_NeoPixel pixel(2, neo_pin, NEO_RGB);
Adafruit_VCNL4010 vcnl;

void setup(void) {
  Serial.begin(9600);
  pinMode(red_led, OUTPUT);

  dot.begin();
  pixel.begin();

  unsigned char i = 0;
  while (i < 10) {
    i += 1;
    digitalWrite(red_led, i % 2 == 0 ? HIGH : LOW);
    dot.clear();
    dot.setPixelColor(0, i % 2 == 0 ? Adafruit_DotStar::Color(255, 0, 0) : Adafruit_DotStar::Color(0, 255, 0));
    dot.show();

    for (unsigned char j = 0; j < 2; j++) {
      pixel.setPixelColor(
        j,
        i % 2 == 0
          ? Adafruit_NeoPixel::Color(255, 0, 0)
          : Adafruit_NeoPixel::Color(0, 255, 0)
      );
      pixel.show();
    }
    delay(500);
  }

  dot.setPixelColor(0, Adafruit_DotStar::Color(0, 0, 0));
  dot.show();
  pixel.fill(Adafruit_NeoPixel::Color(0, 0, 0), 0, 2);
  pixel.show();

  if (!vcnl.begin()) {
    dot.setPixelColor(0, Adafruit_DotStar::Color(255, 0, 0));
    dot.show();
    pixel.fill(Adafruit_NeoPixel::Color(255, 0, 0), 0, 2);
    pixel.show();
    failed = true;
    return;
  }
}

void loop(void) {
  delay(frame_delay);

  if (failed) {
    return;
  }

  uint16_t prox = vcnl.readProximity();

  Serial.print("prox[");
  Serial.print(prox);
  Serial.print("] frame[");
  Serial.print(lit_frames);
  Serial.print("] max[");
  Serial.print(max_idle_lit_frames);
  Serial.println("]");

  bool close = prox >= 2500;

  // Anytime we're in range, reset the amount of frames we've been lit.
  if (close) {
    lit_frames = 0;

    if (!lit) {
      lit = true;
      pixel.fill(Adafruit_NeoPixel::Color(255, 255, 255), 0, 2);
      pixel.show();
    }

    return;
  }

  lit_frames += 1;

  if (lit && lit_frames > max_idle_lit_frames) {
    pixel.fill(Adafruit_NeoPixel::Color(0, 0, 0), 0, 2);
    pixel.show();
    lit = false;
    lit_frames = 0;
  }
}
