 /**
   CLC1 Generated Driver File
 
   @Company
     Microchip Technology Inc.
 
   @File Name
     clc1.c
 
   @Summary
     This is the generated driver implementation file for the CLC1 driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs
 
   @Description
     This source file provides implementations for driver APIs for CLC1.
     Generation Information :
         Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.65.2
         Device            :  PIC16F18345
         Driver Version    :  2.01
     The generated drivers are tested against the following:
         Compiler          :  XC8 1.45 or later
         MPLAB             :  MPLAB X 4.15
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
#include "clc1.h"
#include "mcc.h"
#include "..\DataVariable.h"

/**
  Section: CLC1 APIs
*/

void CLC1_Initialize(void)
{
    // Set the CLC1 to the options selected in the User Interface

    // LC1G1POL not_inverted; LC1G2POL not_inverted; LC1G3POL inverted; LC1G4POL not_inverted; LC1POL not_inverted; 
    CLC1POL = 0x04;
    // LC1D1S CLCIN0 (CLCIN0PPS); 
    CLC1SEL0 = 0x00;
    // LC1D2S CLCIN1 (CLCIN1PPS); 
    CLC1SEL1 = 0x01;
    // LC1D3S CLCIN1 (CLCIN1PPS); 
    CLC1SEL2 = 0x01;
    // LC1D4S CLCIN1 (CLCIN1PPS); 
    CLC1SEL3 = 0x01;
    // LC1G1D3N disabled; LC1G1D2N disabled; LC1G1D4N disabled; LC1G1D1T enabled; LC1G1D3T disabled; LC1G1D2T disabled; LC1G1D4T disabled; LC1G1D1N disabled; 
    CLC1GLS0 = 0x02;
    // LC1G2D2N disabled; LC1G2D1N disabled; LC1G2D4N disabled; LC1G2D3N disabled; LC1G2D2T enabled; LC1G2D1T disabled; LC1G2D4T enabled; LC1G2D3T enabled; 
    CLC1GLS1 = 0xA8;
    // LC1G3D1N disabled; LC1G3D2N disabled; LC1G3D3N disabled; LC1G3D4N disabled; LC1G3D1T disabled; LC1G3D2T enabled; LC1G3D3T enabled; LC1G3D4T enabled; 
    CLC1GLS2 = 0xA8;
    // LC1G4D1N disabled; LC1G4D2N disabled; LC1G4D3N disabled; LC1G4D4N disabled; LC1G4D1T disabled; LC1G4D2T enabled; LC1G4D3T enabled; LC1G4D4T enabled; 
    CLC1GLS3 = 0xA8;
    // LC1EN enabled; INTN disabled; INTP enabled; MODE 2-input D flip-flop with R; 
    CLC1CON = 0x95;

    // Clear the CLC interrupt flag
    PIR3bits.CLC1IF = 0;
    // Enabling CLC1 interrupt.
    PIE3bits.CLC1IE = 1;
}

void CLC1_ISR(void)
{
	if(C_RMOD == true){
		if ((PORTCbits.RC0 == 0) && (PORTCbits.RC1 == 0))
			CLC_2_Interrupt();
		else
			CLC_1_Interrupt();
	}else{
		CLC_1_Interrupt();
	}

    // Clear the CLC interrupt flag
    PIR3bits.CLC1IF = 0;
}

bool CLC1_OutputStatusGet(void)
{
    return(CLC1CONbits.LC1OUT);
}
/**
 End of File
*/
