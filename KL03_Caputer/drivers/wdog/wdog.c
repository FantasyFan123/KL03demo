/*
 * File:        wdog.c
 * Purpose:     Provide common watchdog module routines
 *
 * Notes:		Need to add more functionality. Right now it
 * 				is just a disable routine since we know almost
 * 				all projects will need that.       
 *              
 */

#include "derivative.h"

#include "wdog.h"
       
/********************************************************************/
/*
 * Watchdog timer disable routine
 *
 * Parameters:
 * none
 */
void wdog_disable(void)
{
    SIM_COPC = 0x00;
}
/********************************************************************/
