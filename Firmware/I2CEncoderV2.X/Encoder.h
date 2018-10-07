#ifndef ENCODER_H
#define	ENCODER_H

#include <xc.h> // include processor files - each processor file is guarded.  


#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

#define PB_DEBOUNCE 50

    /** FSM for the push button **/
    typedef enum {
        ENCODER_WAITPUSH,
        ENCODER_WAITRELEASE,
        ENCODER_WAITDOUBLEPUSH,
        ENCODER_WAITDOUBLERELEASED,
        ENCODER_TIMEOUT,
        ENCODER_PUSHRESET,
    } ENCODER_SWITCH_STATUS;

    bool EncoderSwitch_GetValue(void);
    bool EncoderSwitch_GetValueFiltered(void);
    void CLC_1_Interrupt(void);
    void CLC_2_Interrupt(void);
    void RGBEncoder_RLED(uint8_t duty);
    void RGBEncoder_GLED(uint8_t duty);
    void RGBEncoder_BLED(uint8_t duty);
    void FADE_LEDS(void);
    void Encoder_PushButton_FSM(void);
    void Encoder_FSM(void);


#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

