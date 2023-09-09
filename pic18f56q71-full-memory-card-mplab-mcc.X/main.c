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
© [2023] Microchip Technology Inc. and its subsidiaries.

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
    }
}

uint16_t getStringLength(const char* str)
{
    uint16_t count = 0;
    while (str[count] != '\0') { count++; }
    
    //Add an extra position to account for '\0'
    count++;
    return count;
}

static FATFS fs;

void createInfoFile(void)
{
    static FIL file;
    FRESULT result;
    
    printf("Looking for information file...\r\n");
    
    //Try and create a new demo file
    //If it exists, this will fail
    //Note - file path must be 11 chars or less w/o LFN
    result = f_open(&file, "1:/test.htm", FA_CREATE_NEW | FA_WRITE);
    if (result == FR_OK)
    {
        //Need to write text
        printf("Creating information file...\r\n");
        
//        const char* infoString = "PIC18F56Q71 - Temperature Logging Demo\r\n"
//        "More Information: <URL>\r\n"
//        "(C) Microchip Technology 2023\r\n";
        uint16_t bw = 0; //Bytes written
        
        char* txtPtr = demoInfo;
        uint16_t txtLen;
        bool isDone = false;
        
        do
        {
            txtLen = getStringLength(txtPtr);
            
            if (txtLen > FAT_BLOCK_SIZE)
            {
                txtLen = FAT_BLOCK_SIZE;
            }
            else
            {
                isDone = true;
            }
            
            //Write Text
            result = f_write(&file, txtPtr, txtLen, &bw);
            
            //Advance Iterator
            txtPtr += bw;
        } while (!isDone);
        
        
    }
    else if (result == FR_EXIST)
    {
        printf("Demo info file already exists...\r\n");
    }
    else
    {
        printf("[ERROR] Unable to create/open information file, code %d\r\n", result);
    }
    
    //Close the file
    f_close(&file);
}


int main(void)
{
    SYSTEM_Initialize();
    CRC_StartCrc();
    
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
    
    //Mounted Drive
    
    FRESULT result;
    uint8_t bRead = 0;
    
    char buffer[256];
    const char* testFile = "test.txt";
    const char* nText = "Marc McComb was here.";
    
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
                if (result != 0x00)
                {
                    printf("[ERROR] Unable to Mount Drive!\r\n");
                }
                else
                {
                    printf("Drive mounted\r\n");
                    
                    //Create a file for info about the demo
                    createInfoFile();
                    
                    //Begin Log!
                    
//                    result = f_open(&file, "1:/test.txt", FA_READ | FA_WRITE);
//                    
//                    if (result == 0)
//                    {
//                        printf("File was successfully opened\r\n");
//                        
//                        result = f_read(&file, &buffer[0], 256, &bRead);
//                        if (result == 0x00)
//                        {
//                            buffer[bRead] = '\0';
//                            printf("%s\r\n", buffer);
//                            
//                            result = f_write(&file, &nText[0], getStringLength(nText), &bRead);
//                            if (result == 0)
//                            {
//                                printf("Data is queued for write\r\n");
//                            }
//                            else
//                            {
//                                printf("Failed to sync data to disk\r\n");
//                            }
//                        }
//                    }
//                    else
//                    {
//                        printf("Unable to Open File - Code %d\r\n", result);
//                    }
//                    
//                    //Close the file
//                    f_close(&file);
                }
            }
            
        }
        else if (memCard_getCardStatus() == STATUS_CARD_NONE)
        {
            hasPrinted = false;
        }
    }    
}