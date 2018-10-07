

#include "i2c_register.h"
#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/i2c1.h"
#include "main.h"
#include "GPports.h"
#include "DataVariable.h"
#include "Encoder.h"


volatile uint8_t EncoderReg = 0;
volatile bool intclear = false;
volatile bool int2clear = false;

/**
 * @brief Function that set the I2C address according to the A0-A3 jumper setting
 */

void I2C1_AddressSet(void) {
#ifdef CUSTOM_ADD
    i2c_address = CUSTOM_ADD;

#else

    i2c_address = I2C1_SLAVE_ADDRESS;

    if (!ADD0_GetValue())
        i2c_address |= 0x01;

    if (!ADD1_GetValue())
        i2c_address |= 0x02;

    if (!ADD2_GetValue())
        i2c_address |= 0x04;

    if (!ADD3_GetValue())
        i2c_address |= 0x08;

    if (!ADD4_GetValue())
        i2c_address |= 0x10;

    if (!ADD5_GetValue())
        i2c_address |= 0x20;

    if (!ADD6_GetValue())
        i2c_address |= 0x40;
#endif
    i2c_address = (uint8_t) (i2c_address << 1);
}

/**
 * @brief Callback for managing the I2C request from the master
 * @param i2c_bus_state 
 */
void I2C1_StatusCallback(I2C1_SLAVE_DRIVER_STATUS i2c_bus_state) {
    static uint8_t slaveWriteType = SLAVE_NORMAL_DATA;

    switch (i2c_bus_state) {
        case I2C1_SLAVE_WRITE_REQUEST:
            // the master will be sending the eeprom address next
            slaveWriteType = SLAVE_DATA_ADDRESS;
            break;


        case I2C1_SLAVE_WRITE_COMPLETED:

            switch (slaveWriteType) {
                case SLAVE_DATA_ADDRESS:
                    EncoderReg = I2C1_slaveWriteData;
                    break;

                case SLAVE_NORMAL_DATA:
                default:
                    RegisterWrite(EncoderReg, I2C1_slaveWriteData);
                    EncoderReg++;
                    break;

            } // end switch(slaveWriteType)

            slaveWriteType = SLAVE_NORMAL_DATA;
            break;

        case I2C1_SLAVE_READ_REQUEST:
            SSP1BUF = RegisterRead(EncoderReg);
            EncoderReg++;
            break;

        case I2C1_SLAVE_READ_COMPLETED:
            if (intclear == true) {
                StatusClear();
                ClearInterrupt();
                intclear = false;
            }

            if (int2clear == true) {
                Int2StatusClear();
                int2clear = false;
            }

            if (int2clear == false && intclear == false) {

            }
            break;

        default:
            break;

    } // end switch(i2c_bus_state)

}

/**
 * @brief Function that write inside on the Encoder register 
 * @param add Address of the internal register
 * @param data Data to be written inside of the register
 */
void RegisterWrite(uint8_t add, uint8_t data) {

    switch (add) {
        case REG_GCONF:
            GCONF = data;
            GeneralDataSplit();
            GeneralDataSet();
            break;

        case REG_GP1CONF:
            GP1CONF = data;
            Config_GP1();
            break;

        case REG_GP2CONF:
            GP2CONF = data;
            Config_GP2();
            break;

        case REG_GP3CONF:
            if (C_ETYPE == STD_ENCODER) {
                GP3CONF = data;
                Config_GP3();
            }
            break;

        case REG_INTCONF:
            INTCONF = data;
            break;

            /*** Counter value reg ***/
        case REG_CVALB4:
            CVAL.bval[BYTE4] = data;
            break;
        case REG_CVALB3:
            CVAL.bval[BYTE3] = data;
            break;
        case REG_CVALB2:
            CVAL.bval[BYTE2] = data;
            break;
        case REG_CVALB1:
            CVAL.bval[BYTE1] = data;
            break;

            /*** Max Value ***/
        case REG_CMAXB4:
            CMAX.bval[BYTE4] = data;
            break;
        case REG_CMAXB3:
            CMAX.bval[BYTE3] = data;
            break;
        case REG_CMAXB2:
            CMAX.bval[BYTE2] = data;
            break;
        case REG_CMAXB1:
            CMAX.bval[BYTE1] = data;
            break;

            /*** Min Value ***/
        case REG_CMINB4:
            CMIN.bval[BYTE4] = data;
            break;
        case REG_CMINB3:
            CMIN.bval[BYTE3] = data;
            break;
        case REG_CMINB2:
            CMIN.bval[BYTE2] = data;
            break;
        case REG_CMINB1:
            CMIN.bval[BYTE1] = data;
            break;

            /*** Increment step  Value ***/
        case REG_ISTEPB4:
            ISTEP.bval[BYTE4] = data;
            break;
        case REG_ISTEPB3:
            ISTEP.bval[BYTE3] = data;
            break;
        case REG_ISTEPB2:
            ISTEP.bval[BYTE2] = data;
            break;
        case REG_ISTEPB1:
            ISTEP.bval[BYTE1] = data;
            break;

            /* LEDs */
        case REG_RLED:
            RLED = data;

            break;

        case REG_GLED:
            GLED = data;
            break;

        case REG_BLED:
            BLED = data;
            break;

            /* GPs */
        case REG_GP1REG:
            GP1REG = data;
            UpdateOut_GP1();
            break;

        case REG_GP2REG:
            GP2REG = data;
            UpdateOut_GP2();
            break;

        case REG_GP3REG:

            if (C_ETYPE == STD_ENCODER) {
                GP3REG = data;
                UpdateOut_GP3();
            }

            break;

            // timing register
        case REG_ANTBOUNC:
            ANTBOUNC = data;
            DEBOUNCE = (uint16_t) ANTBOUNC * 10;
            break;
        case REG_DPPERIOD:
            DPPERIOD = data;
            DoublePush = (uint16_t) DPPERIOD * 10;
            break;

        case REG_FADERGB:
            FADERGB = data;
            break;

        case REG_FADEGP:
            FADEGP = data;
            break;
    }

    if (add >= REG_EEPROMS) {

        if (C_MBANK == 0) {
            eeprom_write((uint8_t) (add - REG_EEPROMS), (uint8_t) data);

        } else {
            eeprom_write((uint8_t) add, (uint8_t) data);
        }
    }
}

/**
 * @brief Function called when there is a Read request from the i2c master
 * @param add Address of the internal register
 * @return Value of the internal register, 0 in case of invalid register
 */

uint8_t RegisterRead(uint8_t add) {
    uint8_t return_value = 0;

    switch (add) {
        case REG_GCONF:
            return_value = GCONF;
            break;

        case REG_GP1CONF:
            return_value = GP1CONF;
            break;

        case REG_GP2CONF:
            return_value = GP2CONF;
            break;

        case REG_GP3CONF:
            if (C_ETYPE == STD_ENCODER) {
                return_value = GP3CONF;
            } else {
                return_value = 0;
            }
            break;

        case REG_INTCONF:
            return_value = INTCONF;
            break;

        case REG_ESTATUS:
            return_value = ESTATUS;
            intclear = true;
            break;

        case REG_I2STATUS:
            return_value = I2STATUS;
            int2clear = true;
            break;

        case REG_FSTATUS:
            return_value = FSTATUS;
            break;

            /*** Data Value ***/
        case REG_CVALB4:
            return_value = CVAL.bval[BYTE4];
            break;
        case REG_CVALB3:
            return_value = CVAL.bval[BYTE3];
            break;
        case REG_CVALB2:
            return_value = CVAL.bval[BYTE2];
            break;
        case REG_CVALB1:
            return_value = CVAL.bval[BYTE1];
            break;

            /*** Max Value ***/
        case REG_CMAXB4:
            return_value = CMAX.bval[BYTE4];
            break;
        case REG_CMAXB3:
            return_value = CMAX.bval[BYTE3];
            break;
        case REG_CMAXB2:
            return_value = CMAX.bval[BYTE2];
            break;
        case REG_CMAXB1:
            return_value = CMAX.bval[BYTE1];
            break;

            /*** Min Value ***/
        case REG_CMINB4:
            return_value = CMIN.bval[BYTE4];
            break;
        case REG_CMINB3:
            return_value = CMIN.bval[BYTE3];
            break;
        case REG_CMINB2:
            return_value = CMIN.bval[BYTE2];
            break;
        case REG_CMINB1:
            return_value = CMIN.bval[BYTE1];
            break;

            /*** Increment step  Value ***/
        case REG_ISTEPB4:
            return_value = ISTEP.bval[BYTE4];
            break;
        case REG_ISTEPB3:
            return_value = ISTEP.bval[BYTE3];
            break;
        case REG_ISTEPB2:
            return_value = ISTEP.bval[BYTE2];
            break;
        case REG_ISTEPB1:
            return_value = ISTEP.bval[BYTE1];
            break;

            /* LEDs */
        case REG_RLED:
            return_value = RLED;
            break;

        case REG_GLED:
            return_value = GLED;
            break;

        case REG_BLED:
            return_value = BLED;
            break;

        case REG_GP1REG:
            return_value = GP1REG;
            break;

        case REG_GP2REG:
            return_value = GP2REG;
            break;

        case REG_GP3REG:
            if (C_ETYPE == STD_ENCODER) {
                return_value = GP3REG;
            } else {
                return_value = 0;
            }
            break;

        case REG_ANTBOUNC:
            return_value = ANTBOUNC;
            break;

        case REG_DPPERIOD:
            return_value = DPPERIOD;
            break;

        case REG_FADERGB:
            return_value = FADERGB;
            break;

        case REG_FADEGP:
            return_value = FADEGP;
            break;

        default:
            return_value = 0;
            break;

    }
    if (add >= REG_EEPROMS) {
        if (C_MBANK == 0) {
            return_value = eeprom_read((uint8_t) (add - REG_EEPROMS));
        } else {
            return_value = eeprom_read(add);
        }
    }
    return return_value;
}

/**
 * @brief Function that execute the command according on the configuration register
 */
void GeneralDataSet(void) {

    if (C_ETYPE == RGB_ENCODER) {
        RGB_EncoderMode();
    } else {
        STD_EncoderMode();
    }


    if (C_DIRE == true) {
        UNLOCK_PPS;
        CLCIN1PPSbits.CLCIN1PPS = 0x11; //RC1->CLC2:CLCIN1;
        CLCIN0PPSbits.CLCIN0PPS = 0x10; //RC0->CLC2:CLCIN0;
        LOCK_PPS;
    } else {
        UNLOCK_PPS;
        CLCIN1PPSbits.CLCIN1PPS = 0x10; //RC0->CLC2:CLCIN1;
        CLCIN0PPSbits.CLCIN0PPS = 0x11; //RC1->CLC2:CLCIN0;
        LOCK_PPS;
    }

    if (C_IPUD == true) {
        INT_ResetPullup();
        INT_SetHigh();
    } else {
        INT_SetPullup();
        INT_SetHigh();

    }

    if (C_RMOD == true) {
        PIR3bits.CLC1IF = 0;
        PIE3bits.CLC1IE = 0;
        CLC1CON = 0x9D;
        PIE3bits.CLC1IE = 1;

        PIR3bits.CLC2IF = 0;
        PIE3bits.CLC2IE = 0;
        CLC2CON = 0x9D;
        PIE3bits.CLC2IE = 1;

    } else {
        PIR3bits.CLC1IF = 0;
        PIE3bits.CLC1IE = 0;
        CLC1CON = 0x95;
        PIE3bits.CLC1IE = 1;

        PIR3bits.CLC2IF = 0;
        PIE3bits.CLC2IE = 0;
        CLC2CON = 0x95;
        PIE3bits.CLC2IE = 1;
    }


    if (C_DTYPE == INT_DATA_TYPE) {
        ISTEP.val = 1;
    } else {
        ISTEP.fval = 1.0;
    }

    if (C_RESET == true) {
        RESET();
    }
}