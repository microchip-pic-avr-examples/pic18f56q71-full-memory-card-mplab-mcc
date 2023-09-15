#include "demo.h"
#include "./FatFs/ff.h"
#include "./FatFs/diskio.h"
#include "memoryCard.h"
#include "mcc_generated_files/system/system.h"
#include "demoFileAssets.h"

#include <stdint.h>
#include <xc.h>
#include <string.h>

static uint16_t logCount = 0;

//DIA Values
static int16_t gain, offset;

//High Range DIA
//Gain
#define GAIN_HIGH  0x2C002B
#define GAIN_LOW   0x2C002A

//Offset
#define OFFSET_HIGH  0x2C002F
#define OFFSET_LOW  0x2C002E

////Low Range DIA
////Gain
//#define GAIN_HIGH  0x2C0025
//#define GAIN_LOW   0x2C0024
//
////Offset
//#define OFFSET_HIGH  0x2C0029
//#define OFFSET_LOW  0x2C0028

//Initialize the demo
void Demo_initialize(void)
{
    gain = (CONFIGURATION_Read(GAIN_HIGH) << 8) | CONFIGURATION_Read(GAIN_LOW);
    offset = (CONFIGURATION_Read(OFFSET_HIGH) << 8) | CONFIGURATION_Read(OFFSET_LOW);
    printf("Gain = 0x%x, Offset = 0x%x\r\n", gain, offset);
}

//Start logging
void Demo_startLogging(void)
{    
    //Reset Accumulator
    ADC_ClearAccumulator();

    //Set ADC count to 0
    ADCNT = 0;
    
    //Re-enable falling edge interrupts
    IOCANbits.IOCAN0 = 1;
    
    //Restart the timer
    Timer4_Write(0);
    Timer4_Start();
}
    
//Stop logging
void Demo_stopLogging(void)
{
    //Disable falling edge interrupts
    IOCANbits.IOCAN0 = 0;
    
    Timer4_Stop();
}

void Demo_createInfoFile(void)
{
    FIL file;
    FRESULT result;
    
    printf("Looking for information file...\r\n");
    
    //Try and create a new demo file
    //If it exists, this will fail
    //Note - file path must be 11 chars or less w/o LFN
    //Note - FatFs will make the name uppercase, e.g.: INFO.HTM
    result = f_open(&file, INFO_FILE_NAME, FA_CREATE_NEW | FA_WRITE);
    if (result == FR_OK)
    {
        //Need to write text
        printf("Creating information file...\r\n");
        
        uint16_t bw = 0; //Bytes written
        
        char* txtPtr = demoInfo;
        uint16_t txtLen;
        bool isDone = false;
        
        do
        {
            txtLen = strlen(txtPtr);
            
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
            if (result != FR_OK)
            {
                printf("[ERROR] An error occurred during write, code %d\r\n", result);
                isDone = true;
            }
            
            //Advance Iterator
            txtPtr += bw;
        } while (!isDone);
    }
    else if (result == FR_EXIST)
    {
        printf("Demo info file already exists!\r\n");
    }
    else
    {
        printf("[ERROR] Unable to create/open information file, code %d\r\n", result);
    }
    
    //Close the file
    f_close(&file);
}

void Demo_createLogFile(void)
{
    FIL file;
    FRESULT result;
    result = f_open(&file, LOG_FILE_NAME, FA_CREATE_NEW | FA_WRITE);
    
    const char* header = "Sample Number, Die Temperature (C)\r\n";
    uint16_t bw = 0;
    
    if (result == FR_OK)
    {
        printf("Created new log file...\r\n");
        result = f_write(&file, header, strlen(header), &bw);
        if (result != FR_OK)
        {
            printf("[ERROR] An error occurred during write, code %d\r\n", result);
        }
        else
        {
            printf("Data written to file\r\n");
        }
        
    }
    else if (result == FR_EXIST)
    {
        printf("Log file already exists!\r\n");
    }
    else
    {
        printf("[ERROR] Unable to create log file, code %d\r\n", result);
    }
    
    //Close file
    f_close(&file);
}

void Demo_logTemperature(void)
{
    FIL file;
    FRESULT result;
    char text[64];
    uint16_t bw = 0;
    
    printf("Logging temperature...\r\n");
    
    result = f_open(&file, LOG_FILE_NAME, FA_OPEN_APPEND | FA_WRITE);
    
    if (result == FR_OK)
    {
        uint16_t rawADC = ADC_GetFilterValue();
        
        //Calculate the result
        int24_t tempResult = (int24_t)rawADC * gain;
        tempResult /= 256;
        tempResult += offset;
        tempResult /= 10;
        
        //Create the new string
        sprintf(text, "%d, %d\r\n", logCount, tempResult);
#ifdef PRINT_TEMP_RESULTS
        printf("[LOGGED DATA] %s", text);
#endif
        
        //Write the string
        result = f_write(&file, text, strlen(text), &bw);
        if (result != FR_OK)
        {
            printf("[ERROR] Unable to write log file, code %d\r\n", result);
        }
        logCount++;
    }
    else
    {
        printf("[ERROR] Unable to open log file, code %d\r\n", result);
    }
    
    f_close(&file);
}

