/* 
 * File:   i2c_register.h
 * Author: Saimon
 *
 * Created on September 9, 2017, 6:08 PM
 */


#ifndef I2C_REGISTER_H
#define	I2C_REGISTER_H

#include <stdint.h>

/** Default I2C address **/
#define I2C1_SLAVE_ADDRESS 0x00 
//#define CUSTOM_ADD 0b1000000

/** Internal i2C address definition **/
typedef enum {
    REG_GCONF = 0x00,
    REG_GP1CONF = 0x01,
    REG_GP2CONF = 0x02,
    REG_GP3CONF = 0x03,
    REG_INTCONF = 0x04,
    REG_ESTATUS = 0x05,
    REG_I2STATUS = 0x06,
    REG_FSTATUS = 0x07,
    REG_CVALB4 = 0x08,
    REG_CVALB3 = 0x09,
    REG_CVALB2 = 0x0A,
    REG_CVALB1 = 0x0B,
    REG_CMAXB4 = 0x0C,
    REG_CMAXB3 = 0x0D,
    REG_CMAXB2 = 0x0E,
    REG_CMAXB1 = 0x0F,
    REG_CMINB4 = 0x10,
    REG_CMINB3 = 0x11,
    REG_CMINB2 = 0x12,
    REG_CMINB1 = 0x13,
    REG_ISTEPB4 = 0x14,
    REG_ISTEPB3 = 0x15,
    REG_ISTEPB2 = 0x16,
    REG_ISTEPB1 = 0x17,
    REG_RLED = 0x18,
    REG_GLED = 0x19,
    REG_BLED = 0x1A,
    REG_GP1REG = 0x1B,
    REG_GP2REG = 0x1C,
    REG_GP3REG = 0x1D,
    REG_ANTBOUNC = 0x1E,
    REG_DPPERIOD = 0x1F,
    REG_FADERGB = 0x20,
    REG_FADEGP = 0x21,
            
    REG_GAMRLED = 0x27,
    REG_GAMGLED = 0x28,
    REG_GAMBLED = 0x29,        
    REG_GAMMAGP1 = 0x2A,
    REG_GAMMAGP2 = 0x2B,        
    REG_GAMMAGP3 = 0x2C,        
    REG_GCONF2 = 0x30,
    REG_IDCODE = 0x70,
    REG_VERSION = 0x71,          
    REG_EEPROMS = 0x80,
} I2C1_REGISTER;

/** MSB or LSB order in 16bit variable **/
#define BYTE1 0
#define BYTE2 1
#define BYTE3 2
#define BYTE4 3

typedef enum {
    I2C1_SLAVE_WRITE_REQUEST,
    I2C1_SLAVE_READ_REQUEST,
    I2C1_SLAVE_WRITE_COMPLETED,
    I2C1_SLAVE_READ_COMPLETED,
} I2C1_SLAVE_DRIVER_STATUS;

typedef enum {
    SLAVE_NORMAL_DATA,
    SLAVE_DATA_ADDRESS,
} SLAVE_WRITE_DATA_TYPE;


void I2C1_AddressSet(void);

void I2C1_StatusCallback(I2C1_SLAVE_DRIVER_STATUS i2c_bus_state);

void RegisterWrite(uint8_t add, uint8_t data);
uint8_t RegisterRead(uint8_t add);
void FirstConfig(void);
void SecondConfig(void);

#endif	/* I2C_REGISTER_H */

