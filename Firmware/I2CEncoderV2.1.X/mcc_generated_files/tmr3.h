/**
  TMR3 Generated Driver API Header File

  @Company
    Microchip Technology Inc.

  @File Name
    tmr3.h

  @Summary
    This is the generated header file for the TMR3 driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for TMR3.
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

#ifndef TMR3_H
#define TMR3_H

/**
  Section: Included Files
*/

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif

#define TMR3_INTERRUPT_TICKER_FACTOR    1

/**
  Section: TMR3 APIs
*/

/**
  @Summary
    Initializes the TMR3

  @Description
    This routine initializes the TMR3.
    This routine must be called before any other TMR3 routine is called.
    This routine should only be called once during system initialization.

  @Preconditions
    None

  @Param
    None

  @Returns
    None

  @Comment
    

  @Example
    <code>
    main()
    {
        // Initialize TMR3 module
        TMR3_Initialize();

        // Do something else...
    }
    </code>
*/
void TMR3_Initialize(void);

/**
  @Summary
    This function starts the TMR3.

  @Description
    This function starts the TMR3 operation.
    This function must be called after the initialization of TMR3.

  @Preconditions
    Initialize  the TMR3 before calling this function.

  @Param
    None

  @Returns
    None

  @Example
    <code>
    // Initialize TMR3 module

    // Start TMR3
    TMR3_StartTimer();

    // Do something else...
    </code>
*/
void TMR3_StartTimer(void);

/**
  @Summary
    This function stops the TMR3.

  @Description
    This function stops the TMR3 operation.
    This function must be called after the start of TMR3.

  @Preconditions
    Initialize  the TMR3 before calling this function.

  @Param
    None

  @Returns
    None

  @Example
    <code>
    // Initialize TMR3 module

    // Start TMR3
    TMR3_StartTimer();

    // Do something else...

    // Stop TMR3;
    TMR3_StopTimer();
    </code>
*/
void TMR3_StopTimer(void);

/**
  @Summary
    Reads the TMR3 register.

  @Description
    This function reads the TMR3 register value and return it.

  @Preconditions
    Initialize  the TMR3 before calling this function.

  @Param
    None

  @Returns
    This function returns the current value of TMR3 register.

  @Example
    <code>
    // Initialize TMR3 module

    // Start TMR3
    TMR3_StartTimer();

    // Read the current value of TMR3
    if(0 == TMR3_ReadTimer())
    {
        // Do something else...

        // Reload the TMR value
        TMR3_Reload();
    }
    </code>
*/
uint16_t TMR3_ReadTimer(void);

/**
  @Summary
    Writes the TMR3 register.

  @Description
    This function writes the TMR3 register.
    This function must be called after the initialization of TMR3.

  @Preconditions
    Initialize  the TMR3 before calling this function.

  @Param
    timerVal - Value to write into TMR3 register.

  @Returns
    None

  @Example
    <code>
    #define PERIOD 0x80
    #define ZERO   0x00

    while(1)
    {
        // Read the TMR3 register
        if(ZERO == TMR3_ReadTimer())
        {
            // Do something else...

            // Write the TMR3 register
            TMR3_WriteTimer(PERIOD);
        }

        // Do something else...
    }
    </code>
*/
void TMR3_WriteTimer(uint16_t timerVal);

/**
  @Summary
    Reload the TMR3 register.

  @Description
    This function reloads the TMR3 register.
    This function must be called to write initial value into TMR3 register.

  @Preconditions
    Initialize  the TMR3 before calling this function.

  @Param
    None

  @Returns
    None

  @Example
    <code>
    while(1)
    {
        if(TMR3IF)
        {
            // Do something else...

            // clear the TMR3 interrupt flag
            TMR3IF = 0;

            // Reload the initial value of TMR3
            TMR3_Reload();
        }
    }
    </code>
*/
void TMR3_Reload(void);

/**
  @Summary
    Starts the single pulse acquisition in TMR3 gate operation.

  @Description
    This function starts the single pulse acquisition in TMR3 gate operation.
    This function must be used when the TMR3 gate is enabled.

  @Preconditions
    Initialize  the TMR3 with gate enable before calling this function.

  @Param
    None

  @Returns
    None

  @Example
    <code>
    uint16_t xVal;
    uint16_t yVal;

    // enable TMR3 singlepulse mode
    TMR3_StartSinglePulseAcquistion();

    // check TMR3 gate status
    if(TMR3_CheckGateValueStatus()== 0)
        xVal = TMR3_ReadTimer();

    // wait untill gate interrupt occured
    while(TMR3GIF == 0)
    {
    }

    yVal = TMR3_ReadTimer();
    </code>
*/
void TMR3_StartSinglePulseAcquisition(void);

/**
  @Summary
    Check the current state of Timer1 gate.

  @Description
    This function reads the TMR3 gate value and return it.
    This function must be used when the TMR3 gate is enabled.

  @Preconditions
    Initialize  the TMR3 with gate enable before calling this function.

  @Param
    None

  @Returns
    None

  @Example
    <code>
    uint16_t xVal;
    uint16_t yVal;

    // enable TMR3 singlepulse mode
    TMR3_StartSinglePulseAcquistion();

    // check TMR3 gate status
    if(TMR3_CheckGateValueStatus()== 0)
        xVal = TMR3_ReadTimer();

    // wait untill gate interrupt occured
    while(TMR3IF == 0)
    {
    }

    yVal = TMR3_ReadTimer();
    </code>
*/
uint8_t TMR3_CheckGateValueStatus(void);

/**
  @Summary
    Timer Interrupt Service Routine

  @Description
    Timer Interrupt Service Routine is called by the Interrupt Manager.

  @Preconditions
    Initialize  the TMR3 module with interrupt before calling this ISR.

  @Param
    None

  @Returns
    None
*/
void TMR3_ISR(void);
/**
  @Summary
    CallBack function.

  @Description
    This routine is called by the Interrupt Manager.

  @Preconditions
    Initialize  the TMR3 module with interrupt before calling this function.

  @Param
    None

  @Returns
    None
*/
void TMR3_CallBack(void);

/**
  @Summary
    Set Timer Interrupt Handler

  @Description
    This sets the function to be called during the ISR

  @Preconditions
    Initialize  the TMR3 module with interrupt before calling this.

  @Param
    Address of function to be set

  @Returns
    None
*/
 void TMR3_SetInterruptHandler(void (* InterruptHandler)(void));

/**
  @Summary
    Timer Interrupt Handler

  @Description
    This is a function pointer to the function that will be called during the ISR

  @Preconditions
    Initialize  the TMR3 module with interrupt before calling this isr.

  @Param
    None

  @Returns
    None
*/
extern void (*TMR3_InterruptHandler)(void);

/**
  @Summary
    Default Timer Interrupt Handler

  @Description
    This is the default Interrupt Handler function

  @Preconditions
    Initialize  the TMR3 module with interrupt before calling this isr.

  @Param
    None

  @Returns
    None
*/
void TMR3_DefaultInterruptHandler(void);

#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif

#endif // TMR3_H
/**
 End of File
*/
