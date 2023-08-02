/*******************************************************************************
 * Project:     Watchdog App
 * Author:      AvivJilin
 * Version:     1.0 - 11/03/2023
*******************************************************************************/
#define _POSIX_C_SOURCE 200112L  /* sigset_t, CLOCK_REALTIME, SIG_UNBLOCK */

#include <stdio.h>      /* printf, perror */
#include <stdlib.h>     /* exit, atoi */
#include <unistd.h>     /* sleep */
#include <signal.h>     /* sig_atomic_t, sigaction, kill, SIGUSR1, SIGUSR2 */
#include <semaphore.h>  /* sem_t, sem_init, sem_wait, sem_post */
#include <sys/types.h>  /* pid_t */
#include <sys/wait.h>   /* waitpid */
#include <stddef.h>     /* size_t */
#include <assert.h>
#include <pthread.h> /* pthread */

#include "watchdog.h"
#include "scheduler.h"
#include "wd_internal.h"


static volatile size_t g_signal_cnt = 0;
static volatile int g_stop_flag = 0;

int main (int argc, char *argv[])
{
    /* set signals */
    /* set enviorement variable as WD_PID */
    /* get the params from argv and use it in WDFunc and it should be not be posted  */
    size_t interval = 0, max_misses = 0;
    wd_params_ty *wd = NULL;
    SetEnvNum("WD_PID",(int)getpid());
    
    fprintf(stderr, "wd_app has been created %d & Thread is %d\n", getpid(), getppid());
    
    interval = atoi(argv[1]);
    max_misses = atoi(argv[2]);
    
    
    wd = CreateStruct(argc, argv + 3, interval, max_misses, 0);
    
    if (NULL == wd)
    {
        return 0; 
    }
    
    wd->other_pid = getppid();
    
    wd->p_type = APP;
    
    WDFunc(wd, 0);
    
    unsetenv("WD_PID");
    
    puts ("finish wd_app");
    
    return 0;
}