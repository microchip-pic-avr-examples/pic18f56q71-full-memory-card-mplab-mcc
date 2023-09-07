/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */

/* Definitions of physical drive number for each drive */
#define DEV_RAM		0	/* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */

#include "../memoryCard.h"

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive number to identify the drive */
)
{
	DSTATUS stat;
	int result;

	switch (pdrv) {
	case DEV_RAM:
		return STA_NODISK;

	case DEV_MMC:
    {
        MemoryCardDriverStatus memStat;
        memStat = memCard_getCardStatus();
        
        switch (memStat)
        {
            case STATUS_CARD_READY:
            {
                return 0;
            }
            case STATUS_CARD_NOT_INIT:
            {
                return STA_NOINIT;
            }
            case STATUS_CARD_ERROR:
            {
                return STA_NOINIT;
            }
            default:
                return STA_NODISK;
        }
        break;
    }
	case DEV_USB:

		return STA_NODISK;
	}
	return STA_NODISK;
}



/*-----------------------------------------------------------------------*/
/* Initialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive number to identify the drive */
)
{
	DSTATUS stat;
	int result;

	switch (pdrv) {
	case DEV_RAM :

		return STA_NOINIT;

	case DEV_MMC :
		if (memCard_initCard())
        {
            return 0;
        }

		return STA_NOINIT;

	case DEV_USB :

		return STA_NOINIT;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive number to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res = RES_ERROR;
	int result;

	switch (pdrv) {
	case DEV_RAM :
		// translate the arguments here

		return RES_NOTRDY;

	case DEV_MMC :
		// translate the arguments here
        
        if (memCard_readFromDisk(sector, 0, &buff[0], FAT_BLOCK_SIZE))
        {
            return 0;
        }

		return RES_NOTRDY;

	case DEV_USB :

		return RES_NOTRDY;
	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res = RES_ERROR;
	int result;

	switch (pdrv) {
	case DEV_RAM :

		return res;

	case DEV_MMC :
    {
        if (memCard_prepareWrite(sector))
        {
            if (memCard_queueWrite(buff, 512))
            {
                if (memCard_writeBlock() == CARD_NO_ERROR)
                {
                    return 0;
                }
            }
        }
        
        return res;
    }		
	case DEV_USB :
		return res;
	}

	return RES_PARERR;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
	case DEV_RAM :

		// Process of the command for the RAM drive

		return res;

	case DEV_MMC :

		switch(cmd)
        {
            case CTRL_SYNC:
            {
                //Commit data to disk
                
                if (memCard_writeBlock() == CARD_NO_ERROR)
                {
                    return 0;
                }
                
                break;
            }
            case GET_SECTOR_COUNT:
            {
                //Get the number of available sectors
                //TODO
                
                break;
            }
            case GET_SECTOR_SIZE:
            {
                //Get the size of the sector
                //Unused since FF_MAX_SS == FF_MIN_SS
                
                break;
            }
            case GET_BLOCK_SIZE:
            {
                //Get the erase block size (units of sectors)
                //TODO
                
                return 1;
                break;
            }
            case CTRL_TRIM:
            {
                //Not used
                
                break;
            }
        }

		return res;

	case DEV_USB :

		// Process of the command the USB drive

		return res;
	}

	return RES_PARERR;
}

