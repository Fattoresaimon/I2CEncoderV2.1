/**
  Generated Pin Manager File

  Company:
    Microchip Technology Inc.

  File Name:
    pin_manager.c

  Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for pin APIs for all pins selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.65.2
        Device            :  PIC16F18345
        Driver Version    :  2.01
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.45
        MPLAB             :  MPLAB X 4.15

    Copyright (c) 2013 - 2015 released Microchip Technology Inc.  All rights reserved.
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

#include <xc.h>
#include "pin_manager.h"
#include "stdbool.h"





void PIN_MANAGER_Initialize(void)
{
    /**
    LATx registers
    */
    LATA = 0x00;
    LATB = 0x00;
    LATC = 0x00;

    /**
    TRISx registers
    */
    TRISA = 0x37;
    TRISB = 0xD0;
    TRISC = 0xFF;

    /**
    ANSELx registers
    */
    ANSELC = 0x00;
    ANSELB = 0x20;
    ANSELA = 0x00;

    /**
    WPUx registers
    */
    WPUB = 0x10;
    WPUA = 0x3F;
    WPUC = 0x00;

    /**
    ODx registers
    */
    ODCONA = 0x00;
    ODCONB = 0x20;
    ODCONC = 0x00;
	




   
    
	
    SSP1CLKPPSbits.SSP1CLKPPS = 0x0E;   //RB6->MSSP1:SCL1;    
    SSP1DATPPSbits.SSP1DATPPS = 0x0F;   //RB7->MSSP1:SDA1;    
    CLCIN1PPSbits.CLCIN1PPS = 0x11;   //RC1->CLC2:CLCIN1;    
    CLCIN0PPSbits.CLCIN0PPS = 0x10;   //RC0->CLC2:CLCIN0;    
    RB7PPSbits.RB7PPS = 0x19;   //RB7->MSSP1:SDA1;    
    RB6PPSbits.RB6PPS = 0x18;   //RB6->MSSP1:SCL1;    
}
  
void PIN_MANAGER_IOC(void)
{   
}

/**
 End of File
*/