#include "i2c_register.h"
#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/adc.h"
#include "main.h"
#include "GPports.h"
#include "DataVariable.h"
#include "Encoder.h"
#include "PWM.h"


uint8_t pb_fsm = ENCODER_WAITPUSH;
int8_t pb_cnt = 0;
uint16_t double_push_cnt = 0;
uint16_t double_push_delay = 0;
uint8_t fade_cnt = 0;
uint8_t temp_red = 0;
uint8_t temp_green = 0;
uint8_t temp_blu = 0;


volatile uint8_t X1_p; // X1/X2 mode increment
volatile uint8_t X1_n; // X1/X2 mode decrement
volatile uint8_t X2_n; // X2 mode decrement
volatile uint8_t X2_p; // X2 mode increment



volatile uint8_t enc_status;

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

void Encoder_AB_Interrupt(void) {

    enc_status = (enc_status << 2);
    enc_status = enc_status | ((uint8_t) PORTC & 0x03);
    enc_status = enc_status & 0x0f;

    if ((enc_status == X1_p) || (enc_status == X2_p)) {

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
    } else {

        if ((enc_status == X1_n) || (enc_status == X2_n) ) {

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
        }
    }

    SetInterrupt();
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
            PWM_GP3_RLED(temp_red);
        }

        if (temp_green != GLED) {
            temp_green = GLED;
            PWM_GLED(temp_green);
        }

        if (temp_blu != BLED) {
            temp_blu = BLED;
            PWM_BLED(temp_blu);
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

            PWM_GP3_RLED(temp_red);
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

            PWM_GLED(temp_green);
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

            PWM_BLED(temp_blu);
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
                double_push_delay = 0;
                pb_fsm = ENCODER_DOUBLEDELAY;
                EncoderStatusUpdate(S_PUSHD);
                SetInterrupt();
            }
            break;

        case ENCODER_DOUBLEDELAY:
            double_push_cnt = 0;
            if (double_push_delay > DoublePush) {
                pb_fsm = ENCODER_WAITPUSH;
            } else {
                double_push_delay++;
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
    FADE_LEDS();
    Encoder_PushButton_FSM();

}