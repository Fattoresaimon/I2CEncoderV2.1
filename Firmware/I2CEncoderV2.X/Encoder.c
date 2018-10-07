#include "i2c_register.h"
#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/adc.h"
#include "main.h"
#include "GPports.h"
#include "DataVariable.h"
#include "Encoder.h"


uint8_t pb_fsm = ENCODER_WAITPUSH;
int8_t pb_cnt = 0;
uint16_t double_push_cnt = 0;
uint8_t fade_cnt = 0;
uint8_t temp_red = 0;
uint8_t temp_green = 0;
uint8_t temp_blu = 0;

uint16_t en_deb = 0;
uint8_t en_prev = 0;
#define ENC_1 0
#define ENC_2 1

/**
 * @brief function return the status of the encoder switch
 * @return true if pressed, false if released
 */
bool EncoderSwitch_GetValue(void) {
    if (C_ETYPE == RGB_ENCODER) {
        return (SW_RGB_GetValue());
    } else {
        return ((bool)!PWM_LG_SW_GetValue());
    }

}

/**
 * @brief function that make a anti-bouncing of the switch, must be called every 1ms
 * @return true if pressed, false if released
 */
bool EncoderSwitch_GetValueFiltered(void) {
    static bool pb_status = false;

    if (EncoderSwitch_GetValue() == true) {
        if (pb_cnt++ >= PB_DEBOUNCE) {
            pb_cnt = PB_DEBOUNCE;
            pb_status = true;
        }
    } else {
        if (pb_cnt-- <= 0) {
            pb_cnt = 0;
            pb_status = false;
        }
    }

    return pb_status;
}

/**
 * @brief function that is called on one direction of the encoder
 */
void CLC_1_Interrupt(void) {

    if (en_prev == ENC_2)
        if (en_deb < DEBOUNCE)
            return;


    en_deb = 0;
    en_prev = ENC_1;

    if (C_DTYPE == INT_DATA_TYPE) {
        CVAL.val = CVAL.val + ISTEP.val;
        EncoderStatusUpdate(S_RINC);
        if (CVAL.val > CMAX.val) {
            EncoderStatusUpdate(S_RMAX);
            if (C_WRAPE == true) {
                CVAL.val = CMIN.val;
            } else {
                CVAL.val = CMAX.val;
            }
        }
    } else {
        CVAL.fval = CVAL.fval + ISTEP.fval;
        EncoderStatusUpdate(S_RINC);
        if (CVAL.fval > CMAX.fval) {
            EncoderStatusUpdate(S_RMAX);
            if (C_WRAPE == true) {
                CVAL.fval = CMIN.fval;
            } else {
                CVAL.fval = CMAX.fval;
            }
        }
    }


    SetInterrupt();
}

/**
 * @brief function that is called on the other direction of the encoder
 */

void CLC_2_Interrupt(void) {

    if (en_prev == ENC_1)
        if (en_deb < DEBOUNCE)
            return;

    en_deb = 0;
    en_prev = ENC_2;

    if (C_DTYPE == INT_DATA_TYPE) {

        CVAL.val = CVAL.val - ISTEP.val;
        EncoderStatusUpdate(S_RDEC);
        if (CVAL.val < CMIN.val) {
            EncoderStatusUpdate(S_RMIN);
            if (C_WRAPE == true) {
                CVAL.val = CMAX.val;
            } else {
                CVAL.val = CMIN.val;
            }
        }
    } else {
        CVAL.fval = CVAL.fval - ISTEP.fval;
        EncoderStatusUpdate(S_RDEC);
        if (CVAL.fval < CMIN.fval) {
            EncoderStatusUpdate(S_RMIN);
            if (C_WRAPE == true) {
                CVAL.fval = CMAX.fval;
            } else {

                CVAL.fval = CMIN.fval;
            }
        }
    }

    SetInterrupt();
}

/*
 * @brief Update the PWM value of the RED LED of the RGB Encoder
 */
void RGBEncoder_RLED(uint8_t duty) {

    CCPR1H = 0x00;
    CCPR1L = 0xFFU - duty;

    if (duty == 0) {
        PWM_LR_GP3_SetDigitalInput();
    } else {
        PWM_LR_GP3_SetDigitalOutput();
    }

}

/*
 * @brief Update the PWM value of the GREEN LED of the RGB Encoder
 */
void RGBEncoder_GLED(uint8_t duty) {

    CCPR2H = 0;
    CCPR2L = 0xFFU - duty;

    if (duty == 0) {
        PWM_LG_SW_SetDigitalInput();
    } else {
        PWM_LG_SW_SetDigitalOutput();
    }
}

/*
 * @brief Update the PWM value of the BLUE LED of the RGB Encoder
 */
void RGBEncoder_BLED(uint8_t duty) {

    CCPR3H = 0;
    CCPR3L = 0xFFU - duty;

    if (duty == 0) {
        PWM_LB_SWC_SetDigitalInput();
    } else {
        PWM_LB_SWC_SetDigitalOutput();
    }
}

/*
 * @brief Fade manager of the RGB Encoder. It's called every 1ms in the main loop
 */

void FADE_LEDS(void) {

    if (C_ETYPE == STD_ENCODER)
        return;

    if (FADERGB == 0) {
        if (temp_red != RLED) {
            temp_red = RLED;
            RGBEncoder_RLED(temp_red);
        }

        if (temp_green != GLED) {
            temp_green = GLED;
            RGBEncoder_GLED(temp_green);
        }

        if (temp_blu != BLED) {
            temp_blu = BLED;
            RGBEncoder_BLED(temp_blu);
        }
        return;
    }
    fade_cnt++;
    if (fade_cnt >= FADERGB) {
        fade_cnt = 0;

        if (temp_red != RLED) {

            if (temp_red < RLED) {
                temp_red++;
            }
            if (temp_red > RLED) {
                temp_red--;
            }

            RGBEncoder_RLED(temp_red);
            if (temp_red == RLED) {
                FadeProcessClear(F_FER);
            } else {
                FadeProcessSet(F_FER);
            }
        }

        if (temp_green != GLED) {

            if (temp_green < GLED) {
                temp_green++;
            }
            if (temp_green > GLED) {
                temp_green--;
            }

            RGBEncoder_GLED(temp_green);
            if (temp_green == GLED) {
                FadeProcessClear(F_FEG);
            } else {
                FadeProcessSet(F_FEG);

            }
        }


        if (temp_blu != BLED) {

            if (temp_blu < BLED) {
                temp_blu++;
            }
            if (temp_blu > BLED) {
                temp_blu--;
            }

            RGBEncoder_BLED(temp_blu);
            if (temp_blu == BLED) {
                FadeProcessClear(F_FEB);
            } else {

                FadeProcessSet(F_FEB);
            }
        }
    }
}

/*
 @brief managinf the encoder push button
 */

void Encoder_PushButton_FSM(void) {

    if (double_push_cnt > DoublePush) {
        pb_fsm = ENCODER_TIMEOUT;
    } else {
        double_push_cnt++;
    }

    switch (pb_fsm) {
        case ENCODER_WAITPUSH:
            double_push_cnt = 0;
            if (EncoderSwitch_GetValueFiltered() == true) {
                pb_fsm = ENCODER_WAITRELEASE;
            }
            break;


        case ENCODER_WAITRELEASE:
            if (EncoderSwitch_GetValueFiltered() == false) {
                pb_fsm = ENCODER_WAITDOUBLEPUSH;
            }
            break;

        case ENCODER_WAITDOUBLEPUSH:
            if (EncoderSwitch_GetValueFiltered() == true) {
                pb_fsm = ENCODER_WAITDOUBLERELEASED;
            }
            break;

        case ENCODER_WAITDOUBLERELEASED:
            if (EncoderSwitch_GetValueFiltered() == false) {
                double_push_cnt = 0;
                pb_fsm = ENCODER_WAITPUSH;
                EncoderStatusUpdate(S_PUSHD);
                SetInterrupt();
            }
            break;

        case ENCODER_TIMEOUT:
            double_push_cnt = 0;
            EncoderStatusUpdate(S_PUSHP);
            if (EncoderSwitch_GetValueFiltered() == true) {
                pb_fsm = ENCODER_PUSHRESET;
            } else {
                pb_fsm = ENCODER_WAITPUSH;
                EncoderStatusUpdate(S_PUSHR);
            }
            SetInterrupt();
            break;

        case ENCODER_PUSHRESET:
            double_push_cnt = 0;
            if (EncoderSwitch_GetValueFiltered() == false) {

                pb_fsm = ENCODER_WAITPUSH;
                EncoderStatusUpdate(S_PUSHR);
                SetInterrupt();
            }
            break;
    }
}

/*
 @brief FMS for managing the RGB led fade, the push button and the debounce of the encoder
 */
void Encoder_FSM(void) {
    if (en_deb < DEBOUNCE) {
        en_deb++;
    }
    
    FADE_LEDS();
    Encoder_PushButton_FSM();
}