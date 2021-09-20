/* 
 * File:   main.h
 * Author: Saimon
 *
 * Created on 10 settembre 2017, 11.48
 */

#ifndef MAIN_H
#define	MAIN_H


#define ADC_SAMPLE 4000

#define LOCK_PPS     PPSLOCK = 0x55;    PPSLOCK = 0xAA;     PPSLOCKbits.PPSLOCKED = 0x01 // lock PPS
#define UNLOCK_PPS   PPSLOCK = 0x55;    PPSLOCK = 0xAA;     PPSLOCKbits.PPSLOCKED = 0x00 // unlock PPS    

void TimerInt(void);
void SetInterrupt(void);
void ClearInterrupt(void);

#endif	/* MAIN_H */

