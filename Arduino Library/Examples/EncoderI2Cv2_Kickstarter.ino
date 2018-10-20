#include <Wire.h>
#include "i2cEncoderLibV2.h"

/*In this example, i want my variable counter between -10 and 10.
  When it reaches the limit the LED will blink red in case of minimum and will blink green when it reaches the maximum.
  The INT pin is readed in polling mode

  Connections with Arduino UNO:
  - -> GND
  + -> 5V
  SDA -> A4
  SCL -> A5
  INT -> 12
*/


#define ENCODER_N 5
const int IntPin = 12;
i2cEncoderLibV2 encoder[] = { i2cEncoderLibV2(0b0100000), i2cEncoderLibV2(0b1100000), i2cEncoderLibV2(0b1000000), i2cEncoderLibV2(0b0010000), i2cEncoderLibV2(0b1010000)}; //Class initialization with the I2C addresses

int32_t counter[] = {0, 0, 0, 0, 0};
int32_t maxvalue[] = {20, 20, 20, 20.20};
int32_t minvalue[] = { -20, -20, -20, -20, -20};
int32_t econfig[] = {
  (INT_DATA | WRAP_ENABLE | DIRE_LEFT | IPUP_ENABLE | RMOD_X1 | STD_ENCODER),
  (INT_DATA | WRAP_ENABLE | DIRE_LEFT | IPUP_ENABLE | RMOD_X1 | RGB_ENCODER),
  (INT_DATA | WRAP_ENABLE | DIRE_LEFT | IPUP_ENABLE | RMOD_X1 | STD_ENCODER),
  (INT_DATA | WRAP_ENABLE | DIRE_LEFT | IPUP_ENABLE | RMOD_X1 | RGB_ENCODER),
  (INT_DATA | WRAP_ENABLE | DIRE_LEFT | IPUP_ENABLE | RMOD_X1 | STD_ENCODER),
};

uint8_t encoder_status, i;
void setup(void)
{
  pinMode(IntPin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Serial.print("Encoder Test!!\n");
  for (i = 0; i < ENCODER_N; i++) {
    Serial.println(econfig[i], HEX);
    encoder[i].begin(econfig[i]);
    encoder[i].writeGP1conf(GP_AN | GP_PUP_DI | GP_INT_DI);
    encoder[i].writeGP2conf(GP_AN | GP_PUP_DI | GP_INT_DI);
    encoder[i].writeGP3conf(GP_AN | GP_PUP_DI | GP_INT_DI);
    encoder[i].writeCounter(counter[i]);
    encoder[i].writeMax(maxvalue[i]);
    encoder[i].writeMin(minvalue[i]);
    encoder[i].writeLEDR(0x00);
    encoder[i].writeLEDG(0x00);
    encoder[i].writeLEDB(0x00);
    encoder[i].writeInterruptConfig(0xff);
    encoder[i].updateStatus();
  }
  for (i = 0; i < ENCODER_N; i++) {

    encoder[i].writeLEDR(0xFF);
    encoder[i].writeLEDG(0x00);
    encoder[i].writeLEDB(0x00);
    delay(200);
    encoder[i].writeLEDR(0x00);
    encoder[i].writeLEDG(0xFF);
    encoder[i].writeLEDB(0x00);
    delay(200);
    encoder[i].writeLEDR(0x00);
    encoder[i].writeLEDG(0x00);
    encoder[i].writeLEDB(0xFF);
    delay(200);
    encoder[i].writeLEDR(0x00);
    encoder[i].writeLEDG(0x00);
    encoder[i].writeLEDB(0x00);
  }
}

void loop() {




  if (digitalRead(IntPin) == LOW) {

    digitalWrite(LED_BUILTIN, HIGH);


    for (i = 0; i < ENCODER_N; i++) { //Use a for loop for read all the 4 encoders
      if (digitalRead(IntPin) == HIGH)
        break;

      if (encoder[i].updateStatus()) {

        if (encoder[i].readStatus(S_RINC)) {
          encoder[i].writeLEDR(0xff);
        }

        if (encoder[i].readStatus(S_RDEC)) {
          encoder[i].writeLEDG(0xff);
        }

        if (encoder[i].readStatus(S_PUSHP)) {
          encoder[i].writeLEDB(0xff);
          Serial.print("E");
          Serial.print(i);
          Serial.print(" Push\n");
        }

        if (encoder[i].readStatus(S_PUSHR)) {
          encoder[i].writeLEDB(0xff);
          Serial.print("E");
          Serial.print(i);
          Serial.print(" Released\n");
        }

        if (encoder[i].readStatus(S_RMAX)) {
          encoder[i].writeLEDB(0xff);
          Serial.print("E");
          Serial.print(i);
          Serial.print(" Max\n");
        }

        if (encoder[i].readStatus(S_RMIN)) {
          encoder[i].writeLEDB(0xff);
          Serial.print("E");
          Serial.print(i);
          Serial.print(" Min\n");
        }

        counter[i] = encoder[i].readCounterByte();
        encoder[i].writeLEDR(0x00);
        encoder[i].writeLEDG(0x00);
        encoder[i].writeLEDB(0x00);

        for (i = 0; i < ENCODER_N; i++) { //print the final value
          Serial.print(counter[i], DEC);
          Serial.print("\t");
        }
        Serial.println();

        digitalWrite(LED_BUILTIN, LOW);
      }
    }
  }
}

