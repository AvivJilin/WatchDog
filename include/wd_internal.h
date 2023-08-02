#ifndef __WD_IN_H__
#define __WD_IN_H__

#include <stddef.h>
#include "scheduler.h"
#include "semaphore.h"

enum {INVALID_PID = -1, FALSEE = 0, TRUEE = 1};

enum {SUCCESS = 0, FAILED = 1};

enum {NUM_OF_ADDED_ARGS = 3};

enum {MMI_FAIL = 2, BLOCKSIGNALS_FAIL = 3, SEM_DESTROY_FAIL= 4, SEM_WAIT_FAIL = 5,
        CREATE_NEW_THREAD_FAIL = 6};

typedef enum p_type {APP = 0, WD = 1} p_type_ty;

typedef struct wd_params
{
    size_t interval;
    size_t max_misses;
    int argc;
    char **argv;
    pid_t other_pid;
    p_type_ty p_type;
    scheduler_ty *scheduler;
    sem_t have_connection;
}wd_params_ty;

int WDFunc(wd_params_ty *params, int should_post);

wd_params_ty *CreateStruct(int argc, char *argv[], size_t interval, size_t max_misses, pid_t other_pid);

int MakeMeImmortal(int argc, char *argv[], size_t interval, size_t max_misses);

int DoNotResuscitate(void); 

void SetEnvNum(const char *var_name, int var);

#endif  /*  __WD_IN_H__  */
