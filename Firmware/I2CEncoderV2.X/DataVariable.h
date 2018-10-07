/* 
 * File:   DataVariable.h
 * Author: Saimon
 *
 * Created on 11 settembre 2017, 21.20
 */

#ifndef DATAVARIABLE_H
#define	DATAVARIABLE_H

#include <stdint.h>
#include <stdbool.h>




extern volatile uint8_t i2c_address;

extern volatile uint8_t I2C1_slaveWriteData;

extern volatile bool DataType;
#define INT_DATA_TYPE false
#define FLOAT_DATA_TYPE true

#define RGB_ENCODER true
#define STD_ENCODER false

union Data_v {
    double fval;
    int32_t val;
    uint8_t bval[4];

};

/** Internal register **/
/** Configuration **/
extern volatile uint8_t GCONF; //address 0x00
/** GPIO_CONF **/
extern volatile uint8_t GP1CONF; //address 0x01
extern volatile uint8_t GP2CONF; //address 0x02
extern volatile uint8_t GP3CONF; //address 0x03

/** INTCONF_CONF **/
extern volatile uint8_t INTCONF; //address 0x04

/** Status **/
extern volatile uint8_t ESTATUS; //address 0x05
extern volatile uint8_t I2STATUS; //address 0x06
extern volatile uint8_t FSTATUS; //address 0x07

/** Data Value **/
extern volatile union Data_v CVAL; //address 0x08 - 0x0B
extern volatile union Data_v CMAX; //address 0x0C- 0x0F
extern volatile union Data_v CMIN; //address 0x10 - 0x13
extern volatile union Data_v ISTEP; //address 0x14 - 0x17

/** LED Variable **/
extern volatile uint8_t RLED; //address 0x18
extern volatile uint8_t GLED; //address 0x19
extern volatile uint8_t BLED; //address 0x1A

/** GPx register **/
extern volatile uint8_t GP1REG; //address 0x1B
extern volatile uint8_t GP2REG; //address 0x1C
extern volatile uint8_t GP3REG; //address 0x1D

/** Timing register **/
extern volatile uint8_t ANTBOUNC; //address 0x1E
extern volatile uint8_t DPPERIOD; //address 0x1F
extern volatile uint8_t FADERGB; //address 0x20
extern volatile uint8_t FADEGP; //address 0x21


/** General configuration bit **/
extern volatile bool C_RESET, C_MBANK, C_ETYPE, C_RMOD, C_IPUD, C_DIRE, C_WRAPE, C_DTYPE;

extern volatile uint16_t DEBOUNCE;
extern volatile uint16_t DoublePush;



/* GP register setting */
#define GPMODE 0x03
#define GPPULLUP 0x04
#define GPINNT 0x18
/* GP MODE configuration */
#define GP_PWM  0x00
#define GP_OUT  0x01
#define GP_ADC  0x02
#define GP_IN   0x03
/* GP pull-up configuration */
#define GP_PULLDI   0x00
#define GP_PULLEN   0x04
/* GP Interrupt configuration */
#define GP_NOINT    0x00
#define GP_POSED    0x08
#define GP_NEGED    0x10
#define GP_BOTHED   0x18


typedef enum {
    S_PUSHR = 0x01,
    S_PUSHP = 0x02,
    S_PUSHD = 0x04,
    S_RINC = 0x08,
    S_RDEC = 0x10,
    S_RMAX = 0x20,
    S_RMIN = 0x40,
    S_INT2 = 0x80,
} ENCODER_STATUS_CASE;

typedef enum {
    E_GP1POS = 0x01,
    E_GP1NEG = 0x02,
    E_GP2POS = 0x04,
    E_GP2NEG = 0x08,
    E_GP3POS = 0x10,
    E_GP3NEG = 0x20,
    E_GPFADE = 0x40,
} INT2_STATUS_CASE;

typedef enum {
    F_FER = 0x01,
    F_FEG = 0x02,
    F_FEB = 0x04,
    F_FGP1 = 0x08,
    F_FGP2 = 0x10,
    F_FGP3 = 0x20,
} FADE_STATUS_CASE;



void DataInitSet(void);
void GeneralDataSplit(void);
void EncoderStatusUpdate(ENCODER_STATUS_CASE stat);
void Int2StatusUpdate(INT2_STATUS_CASE stat);
void FadeProcessSet(FADE_STATUS_CASE stat);
void StatusClear(void);
void Int2StatusClear(void);
void FadeProcessClear(FADE_STATUS_CASE stat);

#endif	/* DATAVARIABLE_H */