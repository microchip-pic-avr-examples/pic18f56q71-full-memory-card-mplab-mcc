/* 
 * File:   demo.h
 * Author: C62081
 *
 * Created on September 15, 2023, 11:57 AM
 */

#ifndef DEMO_H
#define	DEMO_H

#include "FatFs/ff.h"


#ifdef	__cplusplus
extern "C" {
#endif

//If set, print the temperature values to the terminal    
#define PRINT_TEMP_RESULTS
    
#include "./FatFs/ff.h"
    
#define DEMO_INFO_FILE_NAME "1:/DEMO.HTM"
#define LOG_FILE_NAME "1:/LOG.CSV"
    
    //Initialize the demo
    void Demo_initialize(void);
    
    //Start logging
    void Demo_startLogging(void);
    
    //Stop logging
    void Demo_stopLogging(void);
    
    //Create the demo file
    void Demo_createInfoFile(void);
    
    //Create the log file
    void Demo_createLogFile(void);
    
    //Append the temperature log
    void Demo_logTemperature(void);

#ifdef	__cplusplus
}
#endif

#endif	/* DEMO_H */

