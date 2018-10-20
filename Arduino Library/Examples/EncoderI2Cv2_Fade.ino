#include <Wire.h>
#include "i2cEncoderLibV2.h"
#include <LiquidCrystal.h>

/*In this example there are 2 I2C Encoder V2 boards
   One with the RGB Encoder and the other with a normal rotary encoder and the RGB LED
   There is also the Arduino LCD KeyPad Shield attached.

   The counter value of the encoder are showed on the LCD
   While the LEDS are turned on/off with the fading effect

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
#define ENCODER_N 2

const int IntPin = A3;

//Class initialization with the I2C addresses
i2cEncoderLibV2 STDEncoder(0b0100000); //Normal rotary encoder address
i2cEncoderLibV2 RGBEncoder(0b1100000); //RGB rotary encoder address


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
  // Reset the two encoder
  RGBEncoder.begin(RESET);
  STDEncoder.begin(RESET);

  // Initialize the GPIO and the display
  pinMode(IntPin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);

  //Initialize the Serial port
  Serial.begin(115200);
  Serial.print("Encoder Test!!\n");

  //Configure the RGB Encoder
  RGBEncoder.begin(INT_DATA | WRAP_DISABLE | DIRE_LEFT | IPUP_ENABLE | RMOD_X1 | RGB_ENCODER);
  RGBEncoder.writeCounter((int32_t) 0);
  RGBEncoder.writeMax((int32_t) 50);
  RGBEncoder.writeMin((int32_t) - 10);
  RGBEncoder.writeStep((int32_t) 1);
  RGBEncoder.writeInterruptConfig(0xff); //Enable all the interrupts
  RGBEncoder.writeFadeRGB(1); //Set the fade with 1ms step
  RGBEncoder.writeAntibouncingPeriod(20);  /* Set an anti-bouncing of 200ms */
  RGBEncoder.updateStatus();

  //Configure the Standard Encoder
  STDEncoder.begin(INT_DATA | WRAP_DISABLE | DIRE_LEFT | IPUP_ENABLE | RMOD_X1 | STD_ENCODER);
  STDEncoder.writeGP1conf(GP_PWM | GP_PULL_DI | GP_INT_DI);  // Configure the GP pins in PWM for drive the RGB LED
  STDEncoder.writeGP2conf(GP_PWM | GP_PULL_DI | GP_INT_DI);  // Configure the GP pins in PWM for drive the RGB LED
  STDEncoder.writeGP3conf(GP_PWM | GP_PULL_DI | GP_INT_DI);  // Configure the GP pins in PWM for drive the RGB LED
  STDEncoder.writeCounter((int32_t) 5);
  STDEncoder.writeMax((int32_t) 10);
  STDEncoder.writeMin((int32_t) 0);
  STDEncoder.writeStep((int32_t) 1);
  STDEncoder.writeInterruptConfig(0xff); //Enable all the interrupts
  STDEncoder.writeFadeRGB(2); //Set the fade with 2ms step
  STDEncoder.writeAntibouncingPeriod(20);  /* Set an anti-bouncing of 200ms */
  STDEncoder.updateStatus();

  //Clear the LCD and print the counter value
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("RGB: ");
  lcd.print(RGBEncoder.readCounterInt());
  lcd.print("    ");
  lcd.setCursor(0, 1);
  lcd.print("STD: ");
  lcd.print(STDEncoder.readCounterInt());
  lcd.print("    ");
}

void loop() {


  if (digitalRead(IntPin) == LOW) { //Check for the interrupt

    if (RGBEncoder.updateStatus()) { //Check if the RGB Encoder is moved
      if (RGBEncoder.readStatus(RMAX) || RGBEncoder.readStatus(RMIN) ) {
        RGBEncoder.writeLEDR(0xff); //If the limits are reached turn on the RED LED
      } else {
        if (RGBEncoder.readStatus(RINC) || RGBEncoder.readStatus(RDEC)) {
          RGBEncoder.writeLEDG(0xff); //If the encoder is moved turn on the Green LED
        }
      }

      if (RGBEncoder.readStatus(PUSHR)) {
        RGBEncoder.writeLEDB(0xff);  //If the encoder is pushed turn on the BLUE LED
      }

      if (RGBEncoder.readInt2(FADE_INT)) { //Check if the fade proccess finished, if yes turn off the LEDs
        RGBEncoder.writeLEDR(0x00);
        RGBEncoder.writeLEDG(0x00);
        RGBEncoder.writeLEDB(0x00);
      }

      //Print the counter value of the RGB Encoder
      lcd.setCursor(0, 0);
      lcd.print("RGB: ");
      lcd.print(RGBEncoder.readCounterInt());
      lcd.print("    ");
    }

    if (STDEncoder.updateStatus()) { //Check if the normal encoder is moved
      if (STDEncoder.readStatus(RMAX) || STDEncoder.readStatus(RMIN) ) {
        STDEncoder.writeGP1(0xff);  //If the limits are reached turn on the RED LED
      } else {
        if (STDEncoder.readStatus(RINC) || STDEncoder.readStatus(RDEC)) {
          STDEncoder.writeGP2(0xff); //If the encoder is moved turn on the Green LED

        }
      }
      if (STDEncoder.readStatus(PUSHR)) {
        STDEncoder.writeGP3(0xff); //If the encoder is pushed turn on the BLUE LED
      }

      if (STDEncoder.readInt2(FADE_INT)) { //Check if the fade proccess finished, if yes turn off the LEDs
        STDEncoder.writeGP1(0x00);
        STDEncoder.writeGP2(0x00);
        STDEncoder.writeGP3(0x00);
      }

      //Print the counter value of the normal encoder
      lcd.setCursor(0, 1);
      lcd.print("STD: ");
      lcd.print(STDEncoder.readCounterInt());
      lcd.print("    ");

    }
  }
}
