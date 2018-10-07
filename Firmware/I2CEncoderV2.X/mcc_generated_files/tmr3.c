/**
  TMR3 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    tmr3.c

  @Summary
    This is the generated driver implementation file for the TMR3 driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This source file provides APIs for TMR3.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.65.2
        Device            :  PIC16F18345
        Driver Version    :  2.01
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.45
        MPLAB 	          :  MPLAB X 4.15
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

/**
  Section: Included Files
*/

#include <xc.h>
#include "tmr3.h"

/**
  Section: Global Variables Definitions
*/
volatile uint16_t timer3ReloadVal;
void (*TMR3_InterruptHandler)(void);

/**
  Section: TMR3 APIs
*/

void TMR3_Initialize(void)
{
    //Set the Timer to the options selected in the GUI

    //T3GSS T3G_pin; TMR3GE disabled; T3GTM disabled; T3GPOL low; T3GGO_nDONE done; T3GSPM disabled; 
    T3GCON = 0x00;

    //TMR3H 224; 
    TMR3H = 0xE0;

    //TMR3L 192; 
    TMR3L = 0xC0;

    // Load the TMR value to reload variable
    timer3ReloadVal=(uint16_t)((TMR3H << 8) | TMR3L);

    // Clearing IF flag before enabling the interrupt.
    PIR3bits.TMR3IF = 0;

    // Enabling TMR3 interrupt.
    PIE3bits.TMR3IE = 1;

    // Set Default Interrupt Handler
    TMR3_SetInterruptHandler(TMR3_DefaultInterruptHandler);

    // T3CKPS 1:1; T3SOSC T3CKI_enabled; T3SYNC synchronize; TMR3CS FOSC/4; TMR3ON enabled; 
    T3CON = 0x01;
}

void TMR3_StartTimer(void)
{
    // Start the Timer by writing to TMRxON bit
    T3CONbits.TMR3ON = 1;
}

void TMR3_StopTimer(void)
{
    // Stop the Timer by writing to TMRxON bit
    T3CONbits.TMR3ON = 0;
}

uint16_t TMR3_ReadTimer(void)
{
    uint16_t readVal;
    uint8_t readValHigh;
    uint8_t readValLow;
    
	
    readValLow = TMR3L;
    readValHigh = TMR3H;
    
    readVal = ((uint16_t)readValHigh << 8) | readValLow;

    return readVal;
}

void TMR3_WriteTimer(uint16_t timerVal)
{
    if (T3CONbits.T3SYNC == 1)
    {
        // Stop the Timer by writing to TMRxON bit
        T3CONbits.TMR3ON = 0;

        // Write to the Timer3 register
        TMR3H = (timerVal >> 8);
        TMR3L = timerVal;

        // Start the Timer after writing to the register
        T3CONbits.TMR3ON =1;
    }
    else
    {
        // Write to the Timer3 register
        TMR3H = (timerVal >> 8);
        TMR3L = timerVal;
    }
}

void TMR3_Reload(void)
{
    TMR3_WriteTimer(timer3ReloadVal);
}

void TMR3_StartSinglePulseAcquisition(void)
{
    T3GCONbits.T3GGO_nDONE = 1;
}

uint8_t TMR3_CheckGateValueStatus(void)
{
    return (T3GCONbits.T3GVAL);
}

void TMR3_ISR(void)
{

    // Clear the TMR3 interrupt flag
    PIR3bits.TMR3IF = 0;
    TMR3_WriteTimer(timer3ReloadVal);

    // ticker function call;
    // ticker is 1 -> Callback function gets called everytime this ISR executes
    TMR3_CallBack();
}

void TMR3_CallBack(void)
{
    // Add your custom callback code here
    if(TMR3_InterruptHandler)
    {
        TMR3_InterruptHandler();
    }
}

void TMR3_SetInterruptHandler(void (* InterruptHandler)(void)){
    TMR3_InterruptHandler = InterruptHandler;
}

void TMR3_DefaultInterruptHandler(void){
    // add your TMR3 interrupt custom code
    // or set custom function using TMR3_SetInterruptHandler()
}

/**
  End of File
*/
