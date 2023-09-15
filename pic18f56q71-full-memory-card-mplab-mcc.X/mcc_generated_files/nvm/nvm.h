/**
 * 
 * @file nvm.h
 *
 * @defgroup nvm_driver  Non-Volatile Memory
 *
 * @brief This file contains API prototypes and other data types for the Non-Volatile Memory (NVM) driver.
 *
 * @version NVM Driver Version 3.0.0
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

#ifndef NVM_H
#define NVM_H

#include <xc.h>
#include <stdbool.h>

/**
 * @ingroup nvm_driver
 * @def UNLOCK_KEY
 * Contains the unlock key required for the NVM operations.
 */
#define UNLOCK_KEY                 (0xAA55U)

/**
 * @ingroup nvm_driver
 * @brief Data type for the Device ID data.
 */
typedef uint16_t device_id_data_t;
/**
 * @ingroup nvm_driver
 * @brief Data type for the Device ID address.
 */
typedef uint24_t device_id_address_t;

/**
 * @ingroup nvm_driver
 * @brief Data type for the Configurations Settings data.
 */
typedef uint8_t configuration_data_t;
/**
 * @ingroup nvm_driver
 * @brief Data type for the Configurations Settings address.
 */
typedef uint24_t configuration_address_t;

/**
 * @ingroup nvm_driver
 * @enum nvm_status_t
 * @brief Contains the return codes for the NVM driver APIs.
 */
typedef enum
{
    NVM_OK, /**<0 - The NVM operation succeeded.*/
    NVM_ERROR /**<1 - The NVM operation failed.*/
} nvm_status_t;

/**
 * @ingroup nvm_driver
 * @brief Initializes the NVM driver. 
 * @param None.
 * @return None.
 */
void NVM_Initialize(void);

/**
 * @ingroup nvm_driver
 * @brief Checks if the NVM is busy with the read/write/erase commands.
 * @param None.
 * @retval True - The NVM operation is being performed.
 * @retval False - The NVM operation is not being performed.
 */
bool NVM_IsBusy(void);

/**
 * @ingroup nvm_driver
 * @brief Returns the status of the last NVM operation.
 * @param None.
 * @retval NVM_OK - The NVM operation succeeded.
 * @retval NVM_ERROR - The NVM operation failed.
 */
nvm_status_t NVM_StatusGet(void);

/**
 * @ingroup nvm_driver
 * @brief Clears the NVM error status.
 * @param None.
 * @return None.
 */
void NVM_StatusClear(void);

/**
 * @ingroup nvm_driver
 * @brief Sets the unlock keys required to perform any erase or write operations on Flash and EEPROM.
 * @param [in] unlockKey - Unlock key value specified in the device data sheet. 
 * @return None.
 */
void NVM_UnlockKeySet(uint16_t unlockKey);

/**
 * @ingroup nvm_driver
 * @brief Clears the previously configured unlock keys.
 * @param None. 
 * @return None.
 */
void NVM_UnlockKeyClear(void);

/**
 * @ingroup nvm_driver
 * @brief Reads the Device ID.
 * @param [in] address - Starting address of the Device ID.
 * @return Device ID.
 */
device_id_data_t DeviceID_Read(device_id_address_t address);

/**
 * @ingroup nvm_driver
 * @brief Reads one Configuration byte.
 * @param [in] address - Configuration byte address to be read.
 * @return Configuration byte data.
 */
configuration_data_t CONFIGURATION_Read(configuration_address_t address);

/**
 * @ingroup nvm_driver
 * @brief Writes one Configuration byte.
 *      The NVM busy status must be checked using the @ref NVM_IsBusy() API to know if the write operation is completed.  
 *      Use the @ref NVM_StatusGet() API to see the result of the write operation.
 * @pre Set the unlock key using the @ref NVM_UnlockKeySet() API, if the key has been cleared before.
 *      AoU: **Address Qualifiers** must be configured to **Require** under *Project Properties>XC8 Compiler>Optimizations*.
 * @param [in] address - Configuration byte address to be written.
 * @param [in] data - Configuration byte to be written.
 * @return None.
 */
void CONFIGURATION_Write(configuration_address_t address , configuration_data_t data);

#endif //NVM_H