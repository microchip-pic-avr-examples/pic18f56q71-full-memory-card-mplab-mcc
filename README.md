<!-- Please do not change this logo with link -->

[![MCHP](images/microchip.png)](https://www.microchip.com)

# FatFs on a Memory Card with PIC18F56Q71

[FatFs](http://elm-chan.org/fsw/ff/00index_e.html) is an open-source library for FAT file system management. This example implements a memory card interface and implements the FatFs library to access the FAT file system on the card with the PIC18F56Q71 microcontroller, and logs ??? every 60s. 

## Related Documentation

- [FatFs API Documentation](http://elm-chan.org/fsw/ff/00index_e.html)
- [Memory Card Communication](http://elm-chan.org/docs/mmc/mmc_e.html)

## Software Used

- [MPLAB&reg; X IDE v6.1.5 or newer](#)
- [MPLAB XC8 v2.45 or newer](#)
- [MPLAB Code Configurator (MCC)](#)
- [FatFs R0.15 (Included in Project)](http://elm-chan.org/fsw/ff/00index_e.html)

## Hardware Used

- [PIC18F56Q71  Curiosity Nano Evaluation Kit (EV01G21A)](#)
- [Curiosity Nano Base for Click Boards&trade;](#)
- [MicroSD Click (MIKROE-924)](https://www.mikroe.com/microsd-click)
- [Click (???)](#)
- [Memory Card](https://www.amazon.com/PNY-Elite-microSDHC-Memory-P-SDU32GU185DAC-GE/dp/B07TBH38JK/)
    - Both a 2 GB and a 32 GB card worked without issues in this example.

## Setup

### Hardware Setup

With the power off, plug in the Curiosity Nano into the adapter board. Put the MicroSD Click in slot 1.

### Memory Card Setup

Before use, format the memory card as a FAT volume. FatFs does not need a file to be pre-loaded for this example.

### UART Setup

1. Plug-in the Curiosity Nano.
2. Open MPLAB X IDE.
3. Click on the MPLAB Data Visualizer button in the toolbar.  
![Toolbar](./images/toolbar.png)
4. On the COM port associated with the nano, press the gear symbol.  
![COM Port](./images/comPort.png)
5. Set the baud rate to 115,200.
6. Click the terminal button to connect the COM port to the serial terminal.

Data will print when a memory card is inserted, as shown below.  
![Example Output](./images/exampleOutput.png)

## Implementing FatFs

FatFs handles file system management, but does not handle communication with the memory card. The library requires the following functions to be implemented:

- `disk_initialize`
- `disk_readp`
- `disk_writep`
- `disk_status`
- `disk_ioctl`

These functions call the memory card API to perform file system tasks.  

## Theory of Operation

When a memory card is inserted, a switch in the socket pulls a detection line low. The microcontroller debounces this signal, then sets a flag to initialize the memory card outside of the interrupt handler. When inserted, the card may fail to initialize due to powering on delays, but the program will retry multiple times before erroring out. 

Communication with the memory card is via SPI. A series of commands are sent to the card to configure it and prepare it for file read/write. For commands, the clock frequency is 400 kHz. During memory read/write, the clock frequency is increased up-to a maximum of 10.6 MHz, depending on the memory card's indicated maximum.

## Operation  

When a memory card is inserted, the program will initialize the card with the function `disk_initialize`. If the disk is initialized successfully, the program will look for the file `info.htm`. If the file does not exist, a file is created and filled with information about this program.

After this, the program will create or append the file `log.csv` on the memory card. Every 60s, the program logs the amount ???. 

## Program Options

| Macro | Value | Description
| ----- | ----- | -----------
| MEM_CARD_DEBUG_ENABLE | Defined | Prints debug messages. If not defined, memory usage and performance will improve.
| MEM_CARD_FILE_DEBUG_ENABLE | Defined | Prints file operation requests. If not defined, memory usage and performance will improve.
| MEM_CARD_MEMORY_DEBUG_ENABLE | Not defined | Prints the raw memory bytes received from the memory card. If not defined, memory usage and performance will improve.
| R1_TIMEOUT_BYTES | 10 | How many bytes to wait for a valid response code.
| READ_TIMEOUT_BYTES | 30 | How many bytes to wait for a data response.
| WRITE_TIMEOUT_BYTES | 30 | How many bytes to wait for a write response.
| INIT_RETRIES | 100 | How many times to try and send the initization command to the memory card.
| FULL_RETRIES | 5 | This sets the number of times the system will attempt to initialize the memory card.
| DISABLE_SPEED_SWITCH | Not defined | If defined, the card will remain at 400 kHz speeds for all communication. This will impact performance of read/write operations. 
| CRC_VALIDATE_READ | Defined | If defined, block reads will verify the CRC of the data. **To reject bad data, set ENFORCE_DATA_CRC.**
| ENFORCE_DATA_CRC | Defined | If defined, block reads with a bad CRC will fail. 

*Note: FatFs has a set of macros to modify functionality and/or memory usage. See `ffconf.h` for more information.*

## Summary

This example has demonstrated how to implement FatFs on the PIC18F56Q71 family of microcontrollers to log ????.