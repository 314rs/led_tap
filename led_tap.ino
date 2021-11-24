#include <FastLED.h>
#include <DMXSerial.h>

#define LED_BUILDIN 13
#define LED_PIN 6
#define CLOCK_PIN 4
#define COLOR_ORDER BGR
#define CHIPSET     APA102
#define NUM_LEDS 160
#define BRIGHTNESS  255
#define FRAMES_PER_SECOND 30
#define DMX_CHANNEL 1
#define TIMEOUT 5000 //DMX timeout in milliseconds
#define pushbutton 5
#define STRIPES 5

uint8_t water = 0; //unsigned 8 bit variable to store the amount of water
unsigned long lastPacket = 0;

CRGB leds[NUM_LEDS];
int led_order[5][2] = 
{
  {31, -1},
  {32,  1},
  {95, -1},
  {96,  1},
  {159,-1}
};


void setup() {
  delay(200); // sanity delay
  FastLED.addLeds<CHIPSET, LED_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );
  pinMode(pushbutton, INPUT_PULLUP);
  randomSeed(analogRead(0));
  DMXSerial.init(DMXReceiver);
}

void loop() {
  // put your main code here, to run repeatedly:
  lastPacket = DMXSerial.noDataSince();
  if (lastPacket < TIMEOUT) {
    water = (uint8_t) DMXSerial.read(DMX_CHANNEL);
  }  
  if (digitalRead(pushbutton) == LOW) {
    water = 200;
  }
  fall();
  generate(0, water);
  generate(63, water);
  generate(64, water);
  generate(127, water);
  generate(128, water);


  FastLED.show();
  delay(10);
  water = 0;
}


void generate(int lednum, uint8_t amount) {
  if (random(5, 3500) < amount) {
    uint8_t blue = (uint8_t) random(127, 256);
    uint8_t green = (uint8_t) random(blue*0.7);
    uint8_t red = (uint8_t) random(green*0.7);
    leds[lednum] = CRGB(red, green, blue);
  } else {
    leds[lednum].nscale8(200);
  }
}

void fall(void) {
  for (int s = 0; s < STRIPES; s++) {
    for (int i = 0; i < 31; i++) {
      leds[led_order[s][0]+i*led_order[s][1]] = leds[led_order[s][0]+(i+1)*led_order[s][1]];
    }
  }
}
  /* }
  for (int i = 31; i > 0; i--) {
    leds[i] = leds[i-1];
  }
  for (int i = 32; i < 64; i++) {
    leds[i] = leds[i+1];
  }
  for (int i = 95; i > 64; i--) {
    leds[i] = leds[i-1];
  }
  for (int i = 96; i < 128; i++) {
    leds[i] = leds[i+1];
  }
  for (int i = 159; i > 128; i--) {
    leds[i] = leds[i-1];
  }
 */