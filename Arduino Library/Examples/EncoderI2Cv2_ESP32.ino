#include <Wire.h>
#include "i2cEncoderLibV2.h"

/*This is a basic example for using the I2C Encoder V2
  The counter is set to work between +10 to -10, at every encoder click the counter value is printed on the terminal.
  It's also printet when the push button is pressed, or released or double pushed.
  When the encoder is turned left the RED led turn ON 
  When the encoder is turned right the BLUE led turn ON
  When the encoder is pushed, the GREEN led turn ON

  Connections with ESP32:
  - -> GND
  + -> 3.3V
  SDA -> G21
  SCL -> G22
  INT -> G23
*/

const int IntPin = 23; /* Definition of the interrupt pin*/
//Class initialization with the I2C addresses
i2cEncoderLibV2 Encoder(0x61); /* For make the address 0x61 only the jumpers A0, A5 and A6 are soldered.*/


void setup(void)
{
  pinMode(IntPin, INPUT);

  Serial.begin(115200);
  Serial.println("**** I2C Encoder V2 basic example ****");
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
  Encoder.writeInterruptConfig(0xff); /* Enable all the interrupt */
  Encoder.writeAntibouncingPeriod(30);  /* Set an anti-bouncing of 300ms */
  Encoder.writeDoublePushPeriod(50);  /*Set a period for the double push of 500ms */
}

void loop() {
  uint8_t enc_cnt;
  if (digitalRead(IntPin) == LOW) {
    if ( Encoder.updateStatus()) {
      if ( Encoder.readStatus(RINC)) {
        Serial.print("Increment: ");
        Serial.println( Encoder.readCounterByte());
        Encoder.writeLEDB(0);
        Encoder.writeLEDG(0);
        Encoder.writeLEDR(255);
        /* Write here your code */

      }
      if ( Encoder.readStatus(RDEC)) {
        Serial.print("Decrement: ");
        Serial.println( Encoder.readCounterByte());
        Encoder.writeLEDR(0);
        Encoder.writeLEDG(0);
        Encoder.writeLEDB(255);

        /* Write here your code */

      }

      if ( Encoder.readStatus(RMAX)) {
        Serial.print("Maximum threshold: ");
        Serial.println( Encoder.readCounterByte());

        /* Write here your code */

      }

      if ( Encoder.readStatus(RMIN)) {
        Serial.print("Minimum threshold: ");
        Serial.println( Encoder.readCounterByte());

        /* Write here your code */

      }

      if ( Encoder.readStatus(PUSHR)) {
        Serial.println("Push button Released");

        /* Write here your code */

      }

      if ( Encoder.readStatus(PUSHP)) {
        Serial.println("Push button Pressed");
        Encoder.writeLEDR(0);
        Encoder.writeLEDB(0);
        Encoder.writeLEDG(255);
        /* Write here your code */

      }

      if ( Encoder.readStatus(PUSHD)) {
        Serial.println("Double push!");

        /* Write here your code */

      }
    }
  }
}
