#include <FastLED.h>
#include <DMXSerial.h>

#define LED_PIN     3
#define CLOCK_PIN 4
#define COLOR_ORDER BGR
#define CHIPSET     APA102
#define NUM_LEDS 160
#define BRIGHTNESS  255
#define FRAMES_PER_SECOND 30
//#define SLAVE_CHANNELS   10 

CRGB leds[NUM_LEDS];

#define pushbutton 5
//DMX_Slave dmx_slave ( SLAVE_CHANNELS );
const int ledPin = 13;

void setup() {
   delay(1000); // sanity delay
    // dmx_slave.enable ();
   //  dmx_slave.setStartAddress (1);
//     pinMode ( ledPin, OUTPUT );
    FastLED.addLeds<CHIPSET, LED_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness( BRIGHTNESS );
 pinMode(pushbutton, INPUT_PULLUP);


  DMXSerial.init(DMXReceiver);
 //   DMXSerial.write(1, 1);
}

void loop() {
  // put your main code here, to run repeatedly:
if (digitalRead(pushbutton) == LOW) {
  lauf();
}
// unsigned long lastPacket = DMXSerial.noDataSince();
/*
{
  if ( dmx_slave.getChannelValue (1) > 127 )
   lauf();
    if ( dmx_slave.getChannelValue (1) > 127 )
  digitalWrite (ledPin, HIGH);
     else 
*/
{
if (DMXSerial.read(1) > 127)
   lauf();     
     if (DMXSerial.read(1) > 127)
 digitalWrite (ledPin, HIGH);
 else
 digitalWrite (ledPin, LOW);
}
fadeall();
 FastLED.show(); // laat ook de laatste leds dimmen

}

void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(232); } }

void lauf()  { 
   // First slide the led in one direction
  for(int i = 0; i < NUM_LEDS; i++) {
    // Set the i'th led to white 
    leds[i] = CRGB::White;
    // Show the leds
    FastLED.show(); 
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(5); //Snelheid
}
}
