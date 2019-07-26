/* 
 * File:   GPports.h
 * Author: Saimon
 *
 * Created on 8 aprile 2018, 22.13
 */

#ifndef GPPORTS_H
#define	GPPORTS_H

#ifdef	__cplusplus
extern "C" {
#endif


#define GPIOMASKPOSINT 0x08
#define GPIOMASKNEGINT 0x10

    typedef enum {
        GP_CHECKMODE,
        GP_ADC_MODE,
        GP_WAITRISE,
        GP_WAITFALL,
    } GP_STATUS;

    void RGB_EncoderMode(void);
    void STD_EncoderMode(void);

    void Config_GP1(void);
    void Config_GP2(void);
    void Config_GP3(void);

    void UpdateOut_GP1(void);
    void UpdateOut_GP2(void);
    void UpdateOut_GP3(void);



    void GP1_FSM(void);
    void GP2_FSM(void);
    void GP3_FSM(void);
    void FADE_GPPort1(void);
    void FADE_GPPort2(void);
    void FADE_GPPort3(void);
    void GP_Ports_FSM(void);
#ifdef	__cplusplus
}
#endif

#endif	/* GPPORTS_H */

