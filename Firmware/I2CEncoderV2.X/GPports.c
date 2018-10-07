#include "i2c_register.h"
#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/adc.h"
#include "main.h"
#include "GPports.h"
#include "DataVariable.h"


#define GP_PORTS_TIME 10
uint8_t gp_timer;
uint8_t temp_gp1;
uint8_t temp_gp2;
uint8_t temp_gp3;

uint8_t gp1_fsm = GP_CHECKMODE;
uint8_t gp2_fsm = GP_CHECKMODE;
uint8_t gp3_fsm = GP_CHECKMODE;

uint8_t gp_fade_cnt = 0;

/*
 * @brief Update the PWM value of the GP1
 */
void GPort_GP1_PWM(uint8_t duty) {
    PWM5DCH = (duty & 0xFCU) >> 2U;
    PWM5DCL = (duty & 0x03U) << 6U;

    if (duty == 0) {
        GP1_SetDigitalInput();
    } else {
        GP1_SetDigitalOutput();
    }
}

/*
 * @brief Update the PWM value of the GP2
 */
void GPort_GP2_PWM(uint8_t duty) {

    CCPR4H = 0x00;
    CCPR4L = 0xFFU - duty;

    if (duty == 0) {
        GP2_SetDigitalInput();
    } else {
        GP2_SetDigitalOutput();
    }
}

/*
 * @brief Update the PWM value of the GP3
 */
void GPort_GP3_PWM(uint8_t duty) {

    CCPR1H = 0x00;
    CCPR1L = 0xFFU - duty;

    if (duty == 0) {
        PWM_LR_GP3_SetDigitalInput();
    } else {
        PWM_LR_GP3_SetDigitalOutput();
    }
}

/*
 * @brief FSM of the fade process on the port GP1
 */

void FADE_GP1(void) {

    if ((GP1CONF & GPMODE) != GP_PWM)
        return;

    if (FADEGP == 0) {
        if (temp_gp1 != GP1REG) {
            temp_gp1 = GP1REG;
            GPort_GP1_PWM(temp_gp1);
        }
        return;
    }

    if (gp_fade_cnt >= FADEGP) {
        if (temp_gp1 != GP1REG) {

            if (temp_gp1 < GP1REG) {
                temp_gp1++;
            }
            if (temp_gp1 > GP1REG) {
                temp_gp1--;
            }

            GPort_GP1_PWM(temp_gp1);
            if (temp_gp1 == GP1REG) {
                FadeProcessClear(F_FGP1);
            } else {
                FadeProcessSet(F_FGP1);
            }
        }
    }

}

/*
 * @brief FSM of the fade process on the port GP2
 */
void FADE_GP2(void) {
    
    if ((GP2CONF & GPMODE) != GP_PWM)
        return;

    if (FADEGP == 0) {
        if (temp_gp2 != GP2REG) {
            temp_gp2 = GP2REG;
            GPort_GP2_PWM(temp_gp2);
        }
        return;
    }

    if (gp_fade_cnt >= FADEGP) {
        if (temp_gp2 != GP2REG) {

            if (temp_gp2 < GP2REG) {
                temp_gp2++;
            }
            if (temp_gp2 > GP2REG) {
                temp_gp2--;
            }

            GPort_GP2_PWM(temp_gp2);
            if (temp_gp2 == GP2REG) {
                FadeProcessClear(F_FGP2);
            } else {
                FadeProcessSet(F_FGP2);
            }
        }
    }

}

/*
 * @brief FSM of the fade process on the port GP3
 */

void FADE_GP3(void) {
    
    if ((GP3CONF & GPMODE) != GP_PWM)
        return;

    if (FADEGP == 0) {
        if (temp_gp3 != GP3REG) {
            temp_gp3 = GP3REG;
            GPort_GP3_PWM(temp_gp3);
        }
        return;
    }

    if (gp_fade_cnt >= FADEGP) {
        if (temp_gp3 != GP3REG) {

            if (temp_gp3 < GP3REG) {
                temp_gp3++;
            }
            if (temp_gp3 > GP3REG) {
                temp_gp3--;
            }

            GPort_GP3_PWM(temp_gp3);
            if (temp_gp3 == GP3REG) {
                FadeProcessClear(F_FGP3);
            } else {
                FadeProcessSet(F_FGP3);
            }
        }
    }

}

/*
 * @brief Configure the board in the RGB LED mode
 */
void RGB_EncoderMode(void) {

    PWM_LB_SWC_SetDigitalMode();
    PWM_LB_SWC_SetPushPull();
    PWM_LB_SWC_ResetPullup();
    PWM_LB_SWC_SetDigitalOutput();

    PWM_LG_SW_SetDigitalMode();
    PWM_LG_SW_SetPushPull();
    PWM_LG_SW_ResetPullup();
    PWM_LG_SW_SetDigitalOutput();

    PWM_LR_GP3_SetDigitalMode();
    PWM_LR_GP3_SetPushPull();
    PWM_LR_GP3_SetDigitalOutput();

    UNLOCK_PPS;
    RC5PPSbits.RC5PPS = 0x0C; //RC5->CCP1:CCP1; 
    RC4PPSbits.RC4PPS = 0x0D; //RC4->CCP2:CCP2;   
    RC3PPSbits.RC3PPS = 0x0E; //RC3->CCP3:CCP3; 
    LOCK_PPS;

    SW_RGB_SetDigitalMode();
    SW_RGB_SetDigitalInput();

    PWM1_Initialize();
    PWM2_Initialize();
    PWM3_Initialize();
}

/*
 * @brief Configure the board in the standard rotary encoder
 */
void STD_EncoderMode(void) {

    PWM_LG_SW_SetDigitalMode();
    PWM_LG_SW_SetPullup();
    PWM_LG_SW_SetDigitalInput();

    PWM_LB_SWC_SetDigitalMode();
    PWM_LB_SWC_SetPushPull();
    PWM_LB_SWC_SetDigitalOutput();
    PWM_LB_SWC_SetLow();

    CCP2CON = 0x00;
    CCP3CON = 0x00;

    SW_RGB_SetDigitalMode();
    SW_RGB_SetDigitalInput();
}

/**
 * @brief Function for configure the GP1 port
 */
void Config_GP1(void) {

    PWM5CON = 0;
    UNLOCK_PPS;
    RC7PPSbits.RC7PPS = 0x00;
    LOCK_PPS;

    switch (GP1CONF & GPMODE) {
        case GP_PWM:
            GP1_SetDigitalMode();
            GP1_SetHigh();
            GP1_SetDigitalOutput();
            UNLOCK_PPS;
            RC7PPSbits.RC7PPS = 0x02; //RC7->PWM5:PWM5;  
            LOCK_PPS;
            PWM5_Initialize();
            GPort_GP1_PWM(0);
            break;

        case GP_OUT:
            GP1_SetDigitalMode();
            GP1_SetDigitalOutput();
            GP1_SetPushPull();
            break;

        case GP_ADC:
            GP1_SetDigitalInput();
            GP1_SetAnalogMode();
            break;
        case GP_IN:
            GP1_SetDigitalMode();
            GP1_SetDigitalInput();
            break;
    }

    switch (GP1CONF & GPPULLUP) {
        case GP_PULLDI:
            GP1_ResetPullup();
            break;
        case GP_PULLEN:
            GP1_SetPullup();

            break;
    }

}

/**
 * @brief Function for configure the GP2 port
 */
void Config_GP2(void) {

    CCP4CON = 0;
    UNLOCK_PPS;
    RC6PPSbits.RC6PPS = 0x00;
    LOCK_PPS;

    switch (GP2CONF & GPMODE) {
        case GP_PWM:
            GP2_SetDigitalMode();
            GP2_SetHigh();
            GP2_SetDigitalOutput();
            UNLOCK_PPS;
            RC6PPSbits.RC6PPS = 0x0F; //RC6->CCP4:CCP4;
            LOCK_PPS;
            PWM4_Initialize();
            GPort_GP2_PWM(0);
            break;

        case GP_OUT:
            GP2_SetDigitalMode();
            GP2_SetDigitalOutput();
            GP2_SetPushPull();
            break;

        case GP_ADC:
            GP2_SetDigitalInput();
            GP2_SetAnalogMode();
            break;

        case GP_IN:
            GP2_SetDigitalMode();
            GP2_SetDigitalInput();
            break;
    }

    switch (GP2CONF & GPPULLUP) {
        case GP_PULLDI:
            GP2_ResetPullup();
            break;
        case GP_PULLEN:
            GP2_SetPullup();

            break;
    }

}

/**
 * @brief Function for configure the GP3 port
 */
void Config_GP3(void) {
    CCP1CON = 0;
    UNLOCK_PPS;
    RC5PPSbits.RC5PPS = 0x00;
    LOCK_PPS;

    switch (GP3CONF & GPMODE) {
        case GP_PWM:
            PWM_LR_GP3_SetDigitalMode();
            PWM_LR_GP3_SetHigh();
            PWM_LR_GP3_SetDigitalOutput();
            UNLOCK_PPS;
            RC5PPSbits.RC5PPS = 0x0C; //RC5->CCP1:CCP1;  
            LOCK_PPS;
            PWM1_Initialize();
            GPort_GP3_PWM(0);
            break;

        case GP_OUT:
            PWM_LR_GP3_SetDigitalMode();
            PWM_LR_GP3_SetDigitalOutput();
            PWM_LR_GP3_SetPushPull();
            break;

        case GP_ADC:
            PWM_LR_GP3_SetDigitalInput();
            PWM_LR_GP3_SetAnalogMode();
            break;

        case GP_IN:
            PWM_LR_GP3_SetDigitalMode();
            PWM_LR_GP3_SetDigitalInput();
            break;
    }

    switch (GP3CONF & GPPULLUP) {
        case GP_PULLDI:
            PWM_LR_GP3_ResetPullup();
            break;
        case GP_PULLEN:
            PWM_LR_GP3_SetPullup();
            break;
    }

}

/**
 * @brief Set the logic output for the GP1 pin
 */
void UpdateOut_GP1(void) {
    if ((GP1CONF & GPMODE) == GP_OUT) {
        if (GP1REG == 0) {
            GP1_LAT = 0;
        } else {
            GP1_LAT = 1;
            GP1REG = 1;
        }
        return;
    }
}

/**
 * @brief Set the logic output for the GP2 pin
 */
void UpdateOut_GP2(void) {
    if ((GP2CONF & GPMODE) == GP_OUT) {
        if (GP2REG == 0) {
            GP2_LAT = 0;
        } else {

            GP2_LAT = 1;
            GP2REG = 1;
        }
        return;
    }
}

/**
 * @brief Set the logic output for the GP3 pin
 */
void UpdateOut_GP3(void) {
    if (C_ETYPE == RGB_ENCODER)
        return;

    if ((GP3CONF & GPMODE) == GP_OUT) {
        if (GP3REG == 0) {
            PWM_LR_GP3_LAT = 0;
        } else {
            PWM_LR_GP3_LAT = 1;
            GP3REG = 1;
        }
    }
}

/**
 * @brief FSM for manage the ADC and the Input pin of GP1. it's called every 10ms
 */
void GP1_FSM(void) {
    switch (gp1_fsm) {
        case GP_CHECKMODE:
            if ((GP1CONF & GPMODE) == GP_ADC) {
                gp1_fsm = GP_ADC_MODE;
                ADC_Initialize();
                return;
            }
            if ((GP1CONF & GPMODE) == GP_IN) {
                gp1_fsm = GP_WAITRISE;
                return;
            }
            break;

        case GP_ADC_MODE:
            GP1REG = (ADC_GetConversion(A_GP1) >> 2);
            break;

        case GP_WAITRISE:
            if (GP1_GetValue() == HIGH) {
                GP1REG = 1;
                Int2StatusUpdate(E_GP1POS);
                if ((GP1CONF & GPIOMASKPOSINT) != 0) {
                    SetInterrupt();
                }
                gp1_fsm = GP_WAITFALL;
            }
            break;

        case GP_WAITFALL:
            if (GP1_GetValue() == LOW) {
                GP1REG = 0;
                Int2StatusUpdate(E_GP1NEG);
                if ((GP1CONF & GPIOMASKNEGINT) != 0) {
                    SetInterrupt();
                }
                gp1_fsm = GP_WAITRISE;
            }
            break;
    }
}

/**
 * @brief FSM for manage the ADC and the Input pin of GP2. it's called every 10ms
 */
void GP2_FSM(void) {

    switch (gp2_fsm) {
        case GP_CHECKMODE:
            if ((GP2CONF & GPMODE) == GP_ADC) {
                gp2_fsm = GP_ADC_MODE;
                ADC_Initialize();
                return;
            }

            if ((GP2CONF & GPMODE) == GP_IN) {
                gp2_fsm = GP_WAITRISE;
                return;
            }
            break;

        case GP_ADC_MODE:
            GP2REG = (ADC_GetConversion(A_GP2) >> 2);
            break;

        case GP_WAITRISE:
            if (GP2_GetValue() == HIGH) {
                GP2REG = 1;
                Int2StatusUpdate(E_GP2POS);
                if ((GP2CONF & GPIOMASKPOSINT) != 0) {
                    SetInterrupt();
                }
                gp2_fsm = GP_WAITFALL;
            }
            break;

        case GP_WAITFALL:
            if (GP2_GetValue() == LOW) {

                GP2REG = 0;
                Int2StatusUpdate(E_GP2NEG);
                if ((GP2CONF & GPIOMASKNEGINT) != 0) {
                    SetInterrupt();
                }
                gp2_fsm = GP_WAITRISE;
            }
            break;
    }

}

/**
 * @brief FSM fror manage the ADC and the Input pin of GP3. it's called every 10ms
 */
void GP3_FSM(void) {

    if (C_ETYPE == RGB_ENCODER)
        return;

    switch (gp3_fsm) {
        case GP_CHECKMODE:
            if ((GP3CONF & GPMODE) == GP_ADC) {
                gp3_fsm = GP_ADC_MODE;
                ADC_Initialize();
                return;
            }

            if ((GP3CONF & GPMODE) == GP_IN) {
                gp3_fsm = GP_WAITRISE;
                return;
            }
            break;

        case GP_ADC_MODE:
            GP3REG = (ADC_GetConversion(A_GP3) >> 2);
            break;

        case GP_WAITRISE:
            if (PWM_LR_GP3_GetValue() == HIGH) {
                GP3REG = 1;
                Int2StatusUpdate(E_GP3POS);
                if ((GP3CONF & GPIOMASKPOSINT) != 0) {
                    SetInterrupt();
                }
                gp3_fsm = GP_WAITFALL;
            }
            break;

        case GP_WAITFALL:
            if (PWM_LR_GP3_GetValue() == LOW) {

                GP3REG = 0;
                Int2StatusUpdate(E_GP3NEG);
                if ((GP3CONF & GPIOMASKNEGINT) != 0) {
                    SetInterrupt();
                }
                gp3_fsm = GP_WAITRISE;
            }
            break;
    }
}

void GP_Ports_FSM(void) {


    FADE_GP1();
    FADE_GP2();
    FADE_GP3();

    if (gp_fade_cnt++ >= FADEGP) {
        gp_fade_cnt = 0;
    }

    if (gp_timer++ > GP_PORTS_TIME) {
        gp_timer = 0;
        GP1_FSM();
        GP2_FSM();
        GP3_FSM();
    }
}