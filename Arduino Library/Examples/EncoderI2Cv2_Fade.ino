#include <Wire.h>
#include "i2cEncoderLibV2.h"
#include <LiquidCrystal.h>

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
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5
#define ENCODER_N 2

const int IntPin = A3;
//Class initialization with the I2C addresses
i2cEncoderLibV2 STDEncoder(0b0100000);
i2cEncoderLibV2 RGBEncoder(0b1100000);


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
  RGBEncoder.begin(RESET);
  STDEncoder.begin(RESET);
  pinMode(IntPin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  lcd.begin(16, 2);              // start the library
  lcd.setCursor(0, 0);

  Serial.begin(115200);
  Serial.print("Encoder Test!!\n");
  RGBEncoder.begin(INT_DATA | WRAP_DISABLE | DIRE_LEFT | IPUP_ENABLE | RMOD_X1 | RGB_ENCODER);
  STDEncoder.begin(INT_DATA | WRAP_DISABLE | DIRE_LEFT | IPUP_ENABLE | RMOD_X1 | STANDARD_ENCODER);

  RGBEncoder.writeCounter((int32_t)0);
  RGBEncoder.writeMax((int32_t)50);
  RGBEncoder.writeMin((int32_t) - 10);
  RGBEncoder.writeInterruptConfig(0xff);
  RGBEncoder.writeFadeRGB(1);
  RGBEncoder.updateStatus();


  STDEncoder.writeGP1conf(GP_PWM | GP_PUP_DI | GP_INT_DI);
  STDEncoder.writeGP2conf(GP_PWM | GP_PUP_DI | GP_INT_DI);
  STDEncoder.writeGP3conf(GP_PWM | GP_PUP_DI | GP_INT_DI);
  STDEncoder.writeCounter((int32_t)100);
  STDEncoder.writeMax((int32_t)200);
  STDEncoder.writeMin((int32_t)0);
  STDEncoder.writeInterruptConfig(0xff);
  STDEncoder.writeFadeGP(1);
  STDEncoder.updateStatus();

  lcd.setCursor(0, 0);
  lcd.print(" I2C Encoder V2 ");
  lcd.setCursor(0, 1);
  lcd.print("     Test!!     ");

  STDEncoder.writeFadeGP(5);
  RGBEncoder.writeFadeRGB(5);

  STDEncoder.writeGP2(0xff);
  RGBEncoder.writeLEDB(0xff);
  delay(2000);
  RGBEncoder.writeLEDB(0x00);
  STDEncoder.writeGP2(0x00);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("RGB: ");
  lcd.print(RGBEncoder.readCounterInt());
  lcd.print("    ");
  lcd.setCursor(0, 1);
  lcd.print("STD: ");
  lcd.print(STDEncoder.readCounterInt());
  lcd.print("    ");

  STDEncoder.writeFadeGP(3);
  RGBEncoder.writeFadeRGB(3);
}

void loop() {

  if (digitalRead(IntPin) == LOW) {

    if (RGBEncoder.updateStatus()) {
      if (RGBEncoder.readStatus(S_RMAX) || RGBEncoder.readStatus(S_RMIN) ) {
        RGBEncoder.writeLEDR(0xff);
      } else {
        if (RGBEncoder.readStatus(S_RINC) || RGBEncoder.readStatus(S_RDEC)) {
          RGBEncoder.writeLEDG(0xff);

        }
      }

      if (RGBEncoder.readStatus(S_PUSHR)) {
        RGBEncoder.writeLEDB(0xff);
      }

      if (RGBEncoder.readStatus(S_FADE)) {
        RGBEncoder.writeLEDR(0x00);
        RGBEncoder.writeLEDG(0x00);
        RGBEncoder.writeLEDB(0x00);
      }

      lcd.setCursor(0, 0);
      lcd.print("RGB: ");
      lcd.print(RGBEncoder.readCounterInt());
      lcd.print("    ");
    }

    if (STDEncoder.updateStatus()) {
      if (STDEncoder.readStatus(S_RMAX) || STDEncoder.readStatus(S_RMIN) ) {
        STDEncoder.writeGP1(0xff);
      } else {
        if (STDEncoder.readStatus(S_RINC) || STDEncoder.readStatus(S_RDEC)) {
          STDEncoder.writeGP2(0xff);

        }
      }
      if (STDEncoder.readStatus(S_PUSHR)) {
        STDEncoder.writeGP3(0xff);
      }

      if (STDEncoder.readStatus(S_IGP)) {
        STDEncoder.writeGP1(0x00);
        STDEncoder.writeGP2(0x00);
        STDEncoder.writeGP3(0x00);
      }
      lcd.setCursor(0, 1);
      lcd.print("STD: ");
      lcd.print(STDEncoder.readCounterInt());
      lcd.print("    ");

    }
  }
}
