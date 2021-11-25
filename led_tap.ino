#include <FastLED.h>
#include <DMXSerial.h>

#define LED_PIN 6
#define CLOCK_PIN 4
#define COLOR_ORDER BGR
#define CHIPSET     APA102
#define NUM_LEDS 150
#define BRIGHTNESS  255
#define DMX_CHANNEL 1
#define TIMEOUT 5000 //DMX timeout in milliseconds
#define PUSHBUTTON 5
#define STRIPES 5

uint8_t water = 0; //unsigned 8 bit variable to store the amount of water
unsigned long lastPacket = 0;

CRGB leds[NUM_LEDS];
int led_order[STRIPES][2] = 
{
  {29, -30},
  {30,  30},
  {89, -30},
  {90,  30},
  {149,-30}
};

void setup() {
  delay(200); // sanity delay
  FastLED.addLeds<CHIPSET, LED_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );
  pinMode(PUSHBUTTON, INPUT_PULLUP);
  randomSeed(analogRead(0));
  DMXSerial.init(DMXReceiver);
}

void loop() {
  // put your main code here, to run repeatedly:
  lastPacket = DMXSerial.noDataSince();
  if (lastPacket < TIMEOUT) {
    water = (uint8_t) DMXSerial.read(DMX_CHANNEL);
  } else {
    water = 0;
  }
  if (digitalRead(PUSHBUTTON) == LOW) {
    water = 200;
  }
  fall();
  //generate upper led values
  for (int i = 0; i < STRIPES; i++) {
    generate(led_order[i][0], water);
  }
  FastLED.show();
  delay(10);
}

void generate(int lednum, uint8_t amount) {
  if (random(5, 3500) < amount) {
    uint8_t blue = (uint8_t) random(127, 256);
    uint8_t green = (uint8_t) random(blue*0.7);
    uint8_t red = (uint8_t) random(green*0.6);
    leds[lednum] = CRGB(red, green, blue);
  } else {
    leds[lednum].nscale8(200);
  }
}

void fall(void) {
  for (int s = 0; s < STRIPES; s++) {
    if (led_order[s][1] > 0) {
      for (int i = led_order[s][0] + led_order[s][1] - 1; i > led_order[s][0]; i--) {
        leds[i] = leds[i-1];
      } 
    } else {
      for (int i = led_order[s][0] + led_order[s][1] + 1; i < led_order[s][0]; i++) {
        leds[i] = leds[i+1];
      } 
    }
  }
}