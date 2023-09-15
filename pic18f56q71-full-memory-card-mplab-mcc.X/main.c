 /*
 * MAIN Generated Driver File
 * 
 * @file main.c
 * 
 * @defgroup main MAIN
 * 
 * @brief This is the generated driver implementation file for the MAIN driver.
 *
 * @version MAIN Driver Version 1.0.0
*/

/*
� [2023] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/
#include "mcc_generated_files/system/system.h"
#include "spi1_host.h"
#include "memoryCard.h"
#include "unitTests.h"
#include "FatFs/ff.h"
#include "FatFs/diskio.h"
#include "demoFileAssets.h"
#include "demo.h"

#include <stdint.h>
#include <stdbool.h>

#include <assert.h>
#define STOP() do{__conditional_software_breakpoint(0);NOP();}while(0)
#define ASSERT(x) do{__conditional_software_breakpoint(x);NOP();}while(0)

//#define UNIT_TEST_ENABLE

void onCardChange(void)
{
    if (IS_CARD_ATTACHED())
    {
        memCard_attach();
    }
    else
    {
        memCard_detach();
        Demo_stopLogging();
    }
}

static FATFS fs;
static bool logTimer;

void onResultReady(void)
{
    logTimer = true;
}

int main(void)
{
    SYSTEM_Initialize();
    Demo_initialize();
    
    //Start CRC Shifter
    CRC_StartCrc();
    
    //Set Measurement Callback
    ADC_SetContext1ThresholdInterruptHandler(&onResultReady);
    
    //Clear Timer Log
    logTimer = false;
    
    //Init SPI
    SPI1_initPins();
    SPI1_initHost();

    //Interrupt for card insert/remove
    CLC2_CLCI_SetInterruptHandler(&onCardChange);
    
    //Initialize Memory Card
    memCard_initDriver();
    
    // Enable the Global High Interrupts 
    INTERRUPT_GlobalInterruptHighEnable(); 

    // Enable the Global Low Interrupts 
    INTERRUPT_GlobalInterruptLowEnable(); 

    //Isolated unit tests for sub-systems
#ifdef UNIT_TEST_ENABLE
    unitTest_runSequence();
#endif

    bool hasPrinted = false;
    FRESULT result;
    
    while(1)
    {
        if (memCard_getCardStatus() == STATUS_CARD_NOT_INIT)
        {
            //Card is plugged in
            disk_initialize(1);
        }
        else if (memCard_getCardStatus() == STATUS_CARD_READY)
        {
            if (!hasPrinted)
            {
                hasPrinted = true;
                
                //Memory Card - Vol. 1
                result = f_mount(&fs, "1:/", 0);
                
                //Mount the drive
                if (result != FR_OK)
                {
                    printf("[ERROR] Unable to Mount Drive!\r\n");
                }
                else
                {
                    //Start logging
                    Demo_startLogging();
                    
                    printf("Drive mounted\r\n");
                    
                    //Create a file for info about the demo
                    Demo_createInfoFile();
                    
                    //Create the temperature log header
                    Demo_createLogFile();
                }
            }
            else
            {
                if (logTimer)
                {
                    //Clear the flag
                    logTimer = false;
                    
                    //Log the temperature
                    Demo_logTemperature();
                    
                    //Blink the light
                    LED0_Toggle();
                }
            }
        }
        else if (memCard_getCardStatus() == STATUS_CARD_NONE)
        {
            hasPrinted = false;
        }
    }    
}