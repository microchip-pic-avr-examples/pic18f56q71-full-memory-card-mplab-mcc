#ifndef MEMORYCARD_H
#define	MEMORYCARD_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include <stdint.h>
#include <stdbool.h>

#include "mcc_generated_files/system/pins.h"
#include "mcc_generated_files/clc/clc2.h"

//If defined, all memory card commands are printed to terminal
#define MEM_CARD_DEBUG_ENABLE
    
//If defined, sector requests/file I/O are printed to terminal
#define MEM_CARD_FILE_DEBUG_ENABLE
    
//If defined, the data from the sectors is printed
#define MEM_CARD_SECTOR_DEBUG_ENABLE
    
////If defined, the CRC (from READ DISK) is printed
//#define MEM_CARD_CRC_DEBUG_ENABLE
    
//How many clock sequences to run between each command
#define MEMORY_CARD_IDLE_CLOCK_CYCLES 10
    
//Macro for card insert / detect
#define IS_CARD_ATTACHED() (!CLC2_OutputStatusGet())
    
//Number of bytes to wait for valid response (for R1 commands)
#define R1_TIMEOUT_BYTES 10
        
//Worst case Read Time Delay (ms)
#define DEFAULT_READ_TIMEOUT 250
    
//Worst case Write Time Delay (ms)
#define DEFAULT_WRITE_TIMEOUT 500
    
//How many times will the driver attempt to init the Card (ACMD41 / CMD1)
#define INIT_RETRIES 100
    
//If any stage of init fails, the driver will retry this many times before giving up
#define FULL_RETRIES 5
    
//Check Pattern during configuration (Can be any value)
#define CHECK_PATTERN 0xA5
    
//If set, the SPI is kept at 400 kHz for all communication
//#define DISABLE_SPEED_SWITCH 
    
//If set, read operations will attempt to validate the CRC
//This does not invalidate a read, unless ENFORCE_DATA_CRC is also set
#define CRC_VALIDATE_READ
    
//If set, a read can fail due to bad CRC
#define ENFORCE_DATA_CRC
    
//Set VDD for 2.7V to 3.6V Operation
#define VHS_3V3 0b0001
    
//This is returned if no header errors occurred
#define HEADER_IDLE 0x01
    
//This is returned if no errors occurred AFTER init.
#define HEADER_NO_ERROR 0x00
    
//This is returned for an invalid header
#define HEADER_INVALID 0xFF
    
//SPI Baud Rates (Assume SPI Base = 64 MHz)
    
//8 MHz
#define SPI_FAST_BAUD 3
    
//400 kHz
#define SPI_CMD_BAUD 79
    
//Bad OCR return value
#define CARD_BAD_OCR 0xFFFFFFFF
#define CARD_NO_DATA 0x00000000
    
#define WRITE_SIZE_INVALID 0xFFFF
    
//Number of bytes to transfer
#define FAT_BLOCK_SIZE 512
    
//Block to Byte Shift for FAT file systems
#define FAT_BLOCK_SHIFT 9
    
    typedef union 
    {
        struct {
            unsigned is_idle : 1;
            unsigned erase_reset : 1;
            unsigned illegal_cmd_error : 1;
            unsigned crc_error : 1;
            unsigned erase_seq_error : 1;
            unsigned address_error : 1;
            unsigned param_error : 1;
            unsigned valid_header_n : 1; //This value is 0 if the header is valid
        };
        uint8_t data;
    } command_status_t;
    
        typedef union 
    {
        struct {
            unsigned error : 1;
            unsigned cc_error : 1;
            unsigned card_ecc_error : 1;
            unsigned range_error : 1;
            unsigned locked_error : 1;
            unsigned valid_header_n : 3; //These bits are 0 if valid
        } ErrorToken;
        struct {
            unsigned one : 1; //This bit is always 1
            unsigned status : 3;
            unsigned zero : 1; //This bit is always 1
            unsigned dnc : 3; //Don't care (if all 000s, then this is an error token)
        } DataToken;

        uint8_t data;
    } resp_token_t;
    
    typedef union 
    {
        struct {
            unsigned is_locked : 1;
            unsigned wp_erase_skip : 1;
            unsigned unknown_error : 1;    //Unknown error
            unsigned cc_error : 1; //Controller Error
            unsigned ecc_fail : 1;
            unsigned wp_violation : 1;
            unsigned erase_param : 1;
            unsigned out_of_range : 1;
            unsigned is_idle : 1;
            unsigned erase_reset : 1;
            unsigned illegal_cmd_error : 1;
            unsigned crc_error : 1;
            unsigned erase_seq_error : 1;
            unsigned address_error : 1;
            unsigned param_error : 1;
            unsigned : 1; //Always 0
            
        };
        uint8_t data[2];
        uint16_t packet;
    } card_status_t;
       
    typedef enum {
        CARD_NO_ERROR = 0, CARD_SPI_TIMEOUT, CARD_CRC_ERROR, CARD_RESPONSE_ERROR,
        CARD_ILLEGAL_CMD, CARD_VOLTAGE_NOT_SUPPORTED, CARD_PATTERN_ERROR, 
        CARD_WRITE_IN_PROGRESS, CARD_WRITE_SIZE_ERROR, CARD_NOT_INIT
    } command_error_t;
    
    typedef enum {
        CCS_INVALID = -1, CCS_LOW_CAPACITY, CCS_HIGH_CAPACITY
    } card_capacity_t;
    
    typedef enum {
        STATUS_CARD_NONE = 0, STATUS_CARD_NOT_INIT, STATUS_CARD_ERROR, STATUS_CARD_READY
    } memory_card_driver_status_t;
    
    //Init the Memory Card Driver
    void memCard_initDriver(void);
    
    //Init an inserted Memory Card
    bool memCard_initCard(void);
    
    //Returns the status of the memory card
    memory_card_driver_status_t memCard_getCardStatus(void);
    
    //Returns true if the card is ready
    bool memCard_isCardReady(void);
    
    //Sets read/write time delay, read/write clock delay, and max SPI Clock
    bool memCard_setupTimings(void);
    
    //Calculates the checksum for a block of data
    uint16_t memCard_calculateCRC16(uint8_t* data, uint16_t dLen);
    
    //Notifies the driver that a card is now attached
    //DOES NOT INITIALIZE THE CARD
    void memCard_attach(void);
    
    //Notifies the driver that the card is not attached
    void memCard_detach(void);
    
    //Calls CMD8 to configure the operating voltages
    command_error_t memCard_configureCard(void);
    
    //Send a command to the memory card and processes an R1 response
    uint8_t memCard_sendCMD_R1(uint8_t commandIndex, uint32_t data);
    
    //Send an ACOMMAND to the memory card and processes an R1 response
    uint8_t memCard_sendACMD_R1(uint8_t commandIndex, uint32_t data);
    
    //Returns whether the card is high capacity
    card_capacity_t memCard_getCapacityType(void);
    
    //Returns an R1 type response
    bool memCard_receiveResponse_R1(uint8_t* dst);
    
    //Reads the 4-byte OCR Register
    command_error_t memCard_readOCR(uint8_t* data);
    
    //Reads the 16-byte CSD Register
    command_error_t memCard_readCSD(uint8_t* data);
    
    //Prepare to write to a specified sector.
    //Clears cache to 0, updates write iterators
    bool memCard_prepareWrite(uint32_t sector);
    
    //Queues dLen bytes of data to write, sets bw to the number of bytes queued
    //Returns true if successful, false if failed
    bool memCard_queueWrite(uint8_t* data, uint16_t dLen);
    
    //Writes the current (modified) cache to the memory card
    command_error_t memCard_writeBlock(void);
    
    //Reads a sector of data
    command_error_t memCard_readSector(uint32_t blockAddr, uint8_t* dest);
    
    //Receives length bytes of data. Does not transmit the command
    command_error_t memCard_receiveBlockData(uint8_t* data, uint16_t length);
    
    //Compute CRC7 for the memory card commands
    uint8_t memCard_runCRC7(uint8_t* dataIn, uint8_t len);
    
#ifdef	__cplusplus
}
#endif

#endif	/* MEMORYCARD_H */

