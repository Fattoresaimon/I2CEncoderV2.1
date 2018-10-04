#include <Wire.h>
#include "i2cEncoderLibV2.h"
#include <LiquidCrystal.h>


/*In this example there are 9 I2C Encoder V2 boards with the RGB LED connected in a matrix 3x3
    There is also the Arduino LCD KeyPad Shield attached.

    Every time of one encoder is moved it's value is showed on the display
    In this example the data type are float

  Connections with Arduino UNO:
  - -> GND
  + -> 5V
  SDA -> A4
  SCL -> A5
  INT -> A3
*/

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

#define ENCODER_N 9 //Number limit of the encoder

uint32_t color[ENCODER_N] = {0x04A9FC, 0xFFEF00, 0xE800F9, 0xD2EE841, 0xFD230B, 0x0BFD9C, 0xFD0BBD, 0x3809F2, 0x7EF209   }; //RGB color table used in the initialization

const int IntPin = A3;

//Class initialization with the I2C addresses
i2cEncoderLibV2 RGBEncoder[9] = {
  i2cEncoderLibV2(0x40),
  i2cEncoderLibV2(0x20),
  i2cEncoderLibV2(0x60),
  i2cEncoderLibV2(0x10),
  i2cEncoderLibV2(0x50),
  i2cEncoderLibV2(0x30),
  i2cEncoderLibV2(0x70),
  i2cEncoderLibV2(0x04),
  i2cEncoderLibV2(0x44)
};


// read the buttons
int read_LCD_buttons()
{
  adc_key_in = analogRead(0);      // read the value from the sensor
  // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
  // we add approx 50 to those values and check to see if we are close
  if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
  // For V1.1 us this threshold
  if (adc_key_in < 50)   return btnRIGHT;
  if (adc_key_in < 250)  return btnUP;
  if (adc_key_in < 450)  return btnDOWN;
  if (adc_key_in < 650)  return btnLEFT;
  if (adc_key_in < 850)  return btnSELECT;
  return btnNONE;  // when all others fail, return this...
}



uint8_t encoder_status, i;

void setup(void)
{
  uint8_t enc_cnt;
  //Reset of all the encoder ìs
  for (enc_cnt = 0; enc_cnt < ENCODER_N; enc_cnt++) {
    RGBEncoder[enc_cnt].begin(RESET);
  }

  Wire.setClock(400000);
  pinMode(IntPin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  lcd.begin(16, 2);              // start the library
  lcd.setCursor(0, 0);

  Serial.begin(115200);
  Serial.print("Encoder Test!!\n");
  lcd.clear();

  // Initialization of the encoders
  for (enc_cnt = 0; enc_cnt < ENCODER_N; enc_cnt++) {
    RGBEncoder[enc_cnt].begin(FLOAT_DATA | WRAP_ENABLE | DIRE_LEFT | IPUP_ENABLE | RMOD_X1 | RGB_ENCODER);
    RGBEncoder[enc_cnt].writeCounter((float)0); //Reset of the CVAL register
    RGBEncoder[enc_cnt].writeMax((float)5); //SEt the maximum threshold to 5
    RGBEncoder[enc_cnt].writeMin((float) 0); //Set the minimum threshold to 0
    RGBEncoder[enc_cnt].writeStep((float)0.5); //The step at every encoder click is 0.5
    RGBEncoder[enc_cnt].writeInterruptConfig(0xff);
    RGBEncoder[enc_cnt].writeFadeRGB(2); //Fade enabled with 2ms step
    RGBEncoder[enc_cnt].writeAntibouncingPeriod(25); //250ms of debouncing
    RGBEncoder[enc_cnt].writeDoublePushPeriod(50); //Set the double push period to 500ms
    RGBEncoder[enc_cnt].updateStatus();
  }

  for (enc_cnt = 0; enc_cnt < ENCODER_N; enc_cnt++) {
    RGBEncoder[enc_cnt].writeRGBCode(color[enc_cnt]);  //Update the encoder with the RGB code
  }


  lcd.setCursor(0, 0);
  lcd.print(" I2C Encoder V2 ");
  lcd.setCursor(0, 1);
  lcd.print("     Test!!     ");



}

void loop() {
  uint8_t enc_cnt;
  if (digitalRead(IntPin) == LOW) {
    //Interrupt from the encoders, start to scan the encoder matrix
    for (enc_cnt = 0; enc_cnt < ENCODER_N; enc_cnt++) {
      
      if (digitalRead(IntPin) == HIGH) { //If the interrupt pin return high, exit from the encoder scan
        break;
      }

      if ( RGBEncoder[enc_cnt].updateStatus()) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Encoder N: ");
        lcd.print(enc_cnt);

        if ( RGBEncoder[enc_cnt].readStatus(RINC)) {
          lcd.setCursor(0, 1);
          lcd.print("Increment: ");
          lcd.print( RGBEncoder[enc_cnt].readCounterFloat(), 1);
        }
        if ( RGBEncoder[enc_cnt].readStatus(RDEC)) {
          lcd.setCursor(0, 1);
          lcd.print("Decrement: ");
          lcd.print( RGBEncoder[enc_cnt].readCounterFloat(), 1);
        }

        if ( RGBEncoder[enc_cnt].readStatus(PUSHR)) {
          lcd.setCursor(0, 1);
          lcd.print("Button Released");
        }

        if ( RGBEncoder[enc_cnt].readStatus(PUSHP)) {
          lcd.setCursor(0, 1);
          lcd.print("Button Pressed");
        }

        if ( RGBEncoder[enc_cnt].readStatus(PUSHD)) {
          lcd.setCursor(0, 1);
          lcd.print("Double push");
        }
      }
    }
  }
}
