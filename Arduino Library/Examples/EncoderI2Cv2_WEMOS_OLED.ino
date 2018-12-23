#include<Wire.h>
#include <ESP8266WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "i2cEncoderLibV2.h"

/*This is a basic example for using the I2C Encoder V2 connected to a ESP8266 WEMOS D1 with the OLED shield
  The counter is set to work between +10 to -10, at every encoder click the counter value is printed on the OLED display.
  It's also printet when the push button is pressed.
  When the encoder is turned left the RED led turn ON
  When the encoder is turned right the BLUE led turn ON
  When the encoder is pushed, the GREEN led turn ON

  Connections with Arduino UNO:
  - -> GND
  + -> 3.3V
  SDA -> D2
  SCL -> D1
  INT -> D0
*/


const int IntPin = 16; /* Definition of the interrupt pin*/
//Class initialization with the I2C addresses
i2cEncoderLibV2 Encoder(0x61); /* For make the address 0x61 only the jumpers A0, A5 and A6 are soldered.*/

#define OLED_RESET -1
Adafruit_SSD1306 display(OLED_RESET);

void setup(void)
{
  pinMode(IntPin, INPUT);
  /*
      INT_DATA= The register are considered integer.
      WRAP_DISABLE= The WRAP option is disabled
      DIRE_LEFT= Encoder left direction increase the value
      IPUP_ENABLE= INT pin have the pull-up enabled.
      RMOD_X1= Encoder configured as X1.
      RGB_ENCODER= type of encoder is RGB, change to STD_ENCODER in case you are using a normal rotary encoder.
  */

  Encoder.begin(INT_DATA | WRAP_DISABLE | DIRE_LEFT | IPUP_ENABLE | RMOD_X1 | RGB_ENCODER);
  //  Encoder.begin(INT_DATA | WRAP_DISABLE | DIRE_LEFT | IPUP_ENABLE | RMOD_X1 | STD_ENCODER); // try also this!
  //  Encoder.begin(INT_DATA | WRAP_ENABLE | DIRE_LEFT | IPUP_ENABLE | RMOD_X1 | RGB_ENCODER);  // try also this!

  Encoder.writeCounter((int32_t)0); /* Reset the counter value */
  Encoder.writeMax((int32_t)10); /* Set the maximum threshold*/
  Encoder.writeMin((int32_t) - 10); /* Set the minimum threshold */
  Encoder.writeStep((int32_t)1); /* Set the step to 1*/
  Encoder.writeInterruptConfig(INT2 | RMIN | RMAX | RDEC | RINC | PUSHR ); /* Enable all the interrupt */
  Encoder.writeAntibouncingPeriod(20);  /* Set an anti-bouncing of 300ms */
  Encoder.writeDoublePushPeriod(0);  /*Set a period for the double push of 500ms */

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(0);
  display.display();
}

void loop() {
  uint8_t enc_cnt;
  if (digitalRead(IntPin) == LOW) {
    if ( Encoder.updateStatus()) {

      display.clearDisplay();

      if ( Encoder.readStatus(RINC)) {
        display.setCursor(0, 0);
        display.setTextSize(3);
        display.print( Encoder.readCounterByte());
        display.setCursor(0, 25);
        display.setTextSize(1);
        display.print("Increment");
        Encoder.writeLEDB(0);
        Encoder.writeLEDG(0);
        Encoder.writeLEDR(255);

        /* Write here your code */

      }
      if ( Encoder.readStatus(RDEC)) {
        display.setCursor(0, 0);
        display.setTextSize(3);
        display.print( Encoder.readCounterByte());
        display.setCursor(0, 25);
        display.setTextSize(1);
        display.print("Decrement");
        Encoder.writeLEDR(0);
        Encoder.writeLEDG(0);
        Encoder.writeLEDB(255);

        /* Write here your code */

      }

      if ( Encoder.readStatus(RMAX)) {
        display.setCursor(0, 0);
        display.setTextSize(3);
        display.println( Encoder.readCounterByte());
        display.setCursor(0, 35);
        display.setTextSize(1);
        display.print("Maximum");
        /* Write here your code */

      }

      if ( Encoder.readStatus(RMIN)) {
        display.setCursor(0, 0);
        display.setTextSize(3);
        display.print( Encoder.readCounterByte());
        display.setCursor(0, 35);
        display.setTextSize(1);
        display.print("Minimum");


        /* Write here your code */

      }

      if ( Encoder.readStatus(PUSHR)) {
        
        display.setCursor(0, 0);
        display.setTextSize(3);
        display.print( Encoder.readCounterByte());
        display.setCursor(0, 25);
        display.setTextSize(1);
        display.print("Button pressed!");

        /* Write here your code */
      }
      display.display();
    }
  }
}
