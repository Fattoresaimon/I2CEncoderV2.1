
#include "DataVariable.h"
#include "mcc_generated_files/mcc.h"
#include "main.h"
#include "GPports.h"
#include <stdbool.h>





volatile uint8_t i2c_address;
/** Internal register **/

/** Configuration **/
volatile uint8_t GCONF = 0; //address 0x00
/** GPIO_CONF **/
volatile uint8_t GP1CONF = 0; //address 0x01
volatile uint8_t GP2CONF = 0; //address 0x02
volatile uint8_t GP3CONF = 0; //address 0x03

/** INTCONF_CONF **/
volatile uint8_t INTCONF = 0; //address 0x04

/** Status **/
volatile uint8_t ESTATUS; //address 0x05
volatile uint8_t I2STATUS; //address 0x06
volatile uint8_t FSTATUS; //address 0x07

/** Data Value **/
volatile union Data_v CVAL; //address 0x08 - 0x0B
volatile union Data_v CMAX; //address 0x0C- 0x0F
volatile union Data_v CMIN; //address 0x10 - 0x13
volatile union Data_v ISTEP; //address 0x14 - 0x17

/** LED Variable **/
volatile uint8_t RLED = 0; //address 0x18
volatile uint8_t GLED = 0; //address 0x19
volatile uint8_t BLED = 0; //address 0x1A

/** GPx register **/
volatile uint8_t GP1REG = 0; //address 0x1B
volatile uint8_t GP2REG = 0; //address 0x1C
volatile uint8_t GP3REG = 0; //address 0x1D

/** Timing register **/
volatile uint8_t ANTBOUNC; //address 0x1E
volatile uint8_t DPPERIOD; //address 0x1F
volatile uint8_t FADERGB; //address 0x20
volatile uint8_t FADEGP; //address 0x21

/* Gamma register */
volatile uint8_t GAMMAGP1; //address 
volatile uint8_t GAMMAGP2; //address 
volatile uint8_t GAMMAGP3_RLED; //address 
volatile uint8_t GAMMAGLED; //address 
volatile uint8_t GAMMABLED; //address 

volatile uint8_t GCONF2; //address 


/** First Configuration bit **/
volatile bool C_RESET, C_MBANK, C_ETYPE, C_RMOD, C_IPUD, C_DIRE, C_WRAPE, C_DTYPE;

/** Second configuration bit **/
volatile bool S_CKSRC, S_RELATIVE;


volatile uint16_t DEBOUNCE;
volatile uint16_t DoublePush;

void DataInitSet(void) {
    GCONF = 0;
    GP1CONF = 0;
    GP3CONF = 0;
    INTCONF = 0;
    ESTATUS = 0;
    I2STATUS = 0;
    CVAL.val = 0;
    CMAX.val = 0;
    CMIN.val = 0;
    ISTEP.val = 0;
    RLED = 0;
    GLED = 0;
    BLED = 0;
    GP1REG = 0;
    GP2REG = 0;
    GP3REG = 0;
    C_RESET = false;
    C_MBANK = false;
    C_ETYPE = false;
    C_RMOD = false;
    C_IPUD = false;
    C_DIRE = false;
    C_WRAPE = false;
    C_DTYPE = false;
    S_CKSRC = false;
    S_RELATIVE = false;

    ANTBOUNC = 25;
    DPPERIOD = 0;
    FADERGB = 0;
    FADEGP = 0;
    DEBOUNCE = 250;
    DoublePush = 0;
}

/**
 * @brief Function that split the configuration byte in the single bit
 */
void FirstDataSplit(void) {

    C_DTYPE = GCONF & 0x01U;
    C_WRAPE = (bool) (GCONF >> 1) & 0x01U;
    C_DIRE = (bool) (GCONF >> 2) & 0x01U;
    C_IPUD = (bool) (GCONF >> 3) & 0x01U;
    C_RMOD = (bool) (GCONF >> 4) & 0x01U;
    C_ETYPE = (bool) (GCONF >> 5) & 0x01U;
    C_MBANK = (bool) (GCONF >> 6) & 0x01U;
    C_RESET = (bool) (GCONF >> 7) & 0x01U;

}

void SecondDataSplit(void) {

    S_CKSRC = GCONF2 & 0x01U;
    S_RELATIVE = (bool) (GCONF2 >> 1) & 0x01U;
}

/**
 * @brief Function for update the status variable 
 * @param stat Status of the encoder from ::ENCODER_STATUS_CASE
 */
void EncoderStatusUpdate(ENCODER_STATUS_CASE stat) {
    ESTATUS |= (uint8_t) stat;
}

/**
 * @brief Function for update the status variable of the GP pins
 * @param stat Status of the encoder from GP_STATUS_CASE
 */
void Int2StatusUpdate(INT2_STATUS_CASE stat) {
    I2STATUS |= (uint8_t) stat;
    EncoderStatusUpdate(S_INT2);
}

/**
 * @brief Function for update the status of the fading process
 * @param stat Status of the fade process from FADE_STATUS_CASE
 */
void FadeProcessSet(FADE_STATUS_CASE stat) {
    FSTATUS |= (uint8_t) stat;
}

/**
 * @brief Function for clear a bit of the fading process
 * @param stat Status of the fade process from FADE_STATUS_CASE
 */
void FadeProcessClear(FADE_STATUS_CASE stat) {
    FSTATUS &= ~(uint8_t) stat;
    Int2StatusUpdate(E_GPFADE);

    SetInterrupt();
}
