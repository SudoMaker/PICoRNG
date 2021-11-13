/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for .
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.7
        Device            :  PIC16LF1454
        Driver Version    :  2.11
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.31 and above
        MPLAB 	          :  MPLAB X 5.45	
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

#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

/**
  Section: Included Files
*/

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set RA0 procedures
#define RA0_GetValue()              PORTAbits.RA0

// get/set RA1 procedures
#define RA1_GetValue()              PORTAbits.RA1

// get/set LED2 aliases
#define LED2_TRIS                 TRISCbits.TRISC2
#define LED2_LAT                  LATCbits.LATC2
#define LED2_PORT                 PORTCbits.RC2
#define LED2_ANS                  ANSELCbits.ANSC2
#define LED2_SetHigh()            do { LATCbits.LATC2 = 1; } while(0)
#define LED2_SetLow()             do { LATCbits.LATC2 = 0; } while(0)
#define LED2_Toggle()             do { LATCbits.LATC2 = ~LATCbits.LATC2; } while(0)
#define LED2_GetValue()           PORTCbits.RC2
#define LED2_SetDigitalInput()    do { TRISCbits.TRISC2 = 1; } while(0)
#define LED2_SetDigitalOutput()   do { TRISCbits.TRISC2 = 0; } while(0)
#define LED2_SetAnalogMode()      do { ANSELCbits.ANSC2 = 1; } while(0)
#define LED2_SetDigitalMode()     do { ANSELCbits.ANSC2 = 0; } while(0)

// get/set LED1 aliases
#define LED1_TRIS                 TRISCbits.TRISC3
#define LED1_LAT                  LATCbits.LATC3
#define LED1_PORT                 PORTCbits.RC3
#define LED1_ANS                  ANSELCbits.ANSC3
#define LED1_SetHigh()            do { LATCbits.LATC3 = 1; } while(0)
#define LED1_SetLow()             do { LATCbits.LATC3 = 0; } while(0)
#define LED1_Toggle()             do { LATCbits.LATC3 = ~LATCbits.LATC3; } while(0)
#define LED1_GetValue()           PORTCbits.RC3
#define LED1_SetDigitalInput()    do { TRISCbits.TRISC3 = 1; } while(0)
#define LED1_SetDigitalOutput()   do { TRISCbits.TRISC3 = 0; } while(0)
#define LED1_SetAnalogMode()      do { ANSELCbits.ANSC3 = 1; } while(0)
#define LED1_SetDigitalMode()     do { ANSELCbits.ANSC3 = 0; } while(0)

// get/set DCDC_SW aliases
#define DCDC_SW_TRIS                 TRISCbits.TRISC4
#define DCDC_SW_LAT                  LATCbits.LATC4
#define DCDC_SW_PORT                 PORTCbits.RC4
#define DCDC_SW_SetHigh()            do { LATCbits.LATC4 = 1; } while(0)
#define DCDC_SW_SetLow()             do { LATCbits.LATC4 = 0; } while(0)
#define DCDC_SW_Toggle()             do { LATCbits.LATC4 = ~LATCbits.LATC4; } while(0)
#define DCDC_SW_GetValue()           PORTCbits.RC4
#define DCDC_SW_SetDigitalInput()    do { TRISCbits.TRISC4 = 1; } while(0)
#define DCDC_SW_SetDigitalOutput()   do { TRISCbits.TRISC4 = 0; } while(0)

// get/set RNG_DATA aliases
#define RNG_DATA_TRIS                 TRISCbits.TRISC5
#define RNG_DATA_LAT                  LATCbits.LATC5
#define RNG_DATA_PORT                 PORTCbits.RC5
#define RNG_DATA_SetHigh()            do { LATCbits.LATC5 = 1; } while(0)
#define RNG_DATA_SetLow()             do { LATCbits.LATC5 = 0; } while(0)
#define RNG_DATA_Toggle()             do { LATCbits.LATC5 = ~LATCbits.LATC5; } while(0)
#define RNG_DATA_GetValue()           PORTCbits.RC5
#define RNG_DATA_SetDigitalInput()    do { TRISCbits.TRISC5 = 1; } while(0)
#define RNG_DATA_SetDigitalOutput()   do { TRISCbits.TRISC5 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/