/*******************************************************************************
 * Project:     Watchdog
 * Author:      HRD28
 * Version:     1.0 - 09/03/2023
*******************************************************************************/
#ifndef __WATCHDOG_H__
#define __WATCHDOG_H__

#include <stddef.h>


/*******************************************************************************
 * note: SIGUSR1 and SIGUSR2 are used for the operation of the watchdog!      *
*******************************************************************************/

/*******************************************************************************
 * sets a watchdog for the calling program

 * the watchdog checks that the calling program is still alive at a regular
 * "interval" [seconds]

 * if the watchdog does not get a sign of life "max_misses" times in a row, it
 * will restart the calling program, with the command line variables given by
 * "argv" and "argc"

 * returns 0 for success, not 0 otherwise

 * note: undefined behaviour if either "interval" or "max_misses" equals 0
*******************************************************************************/
int MakeMeImmortal(int argc, char *argv[], size_t interval, size_t max_misses);

/*******************************************************************************
 * notifies the watchdog to not resuscitate the calling program

 * returns 0 for success, not 0 otherwise
*******************************************************************************/
int DoNotResuscitate(void);

#endif  /*  __WATCHDOG_H__  */
