/*******************************************************************************
 * Project:     Watchdog
 * Author:      AvivJilin
 * Version:     1.0 - 11/03/2023
*******************************************************************************/
#define _POSIX_C_SOURCE 200112L  /* sigset_t, CLOCK_REALTIME, SIG_UNBLOCK */

#include <stdio.h> /* fprintf */
#include <unistd.h> /* getpid */
#include <stdlib.h> /*   malloc, free, setenv, unsetenv, atoi    */
#include <pthread.h> /* pthread */
#include <assert.h>  /* assert */
#include <signal.h>  /* SIGUSR1, SIGUSR2, sigaction */
#include <string.h> /* strcpy, memcpy */
#include <sys/wait.h> /* waitpid */

#include "watchdog.h"
#include "wd_internal.h"
#include "scheduler.h"
#include "utils.h"

#define FILE_NAME "./wd_app"
#define BUFFER_SIZE 10

static volatile size_t g_signal_cnt = 0;
static volatile int g_stop_flag = 0;
static sem_t g_dnr_return;

/* Signal handlers */
static void HandlerSIGUSR1(int sig_num);
static void HandlerSIGUSR2(int sig_num);

static void *WDRoutine(void *params);
int CreateNewThread(wd_params_ty *wd_params);
static int BlockSignals(void);
static int InstallSignalHandlers(void);
static char *AllocNumber(size_t num);
static int Revive(wd_params_ty *params);
static char **CreateNewVector(int argc, char *argv[], size_t interval, size_t max_misses);
static int IsConnected(void *wd);
static int IsWatchDogExist(wd_params_ty *wd);
static pid_t GetEnvNum(const char *var_name);
static int DestroyAll(wd_params_ty *wd_params, scheduler_ty *scheduler, char *argv[]);

/* Signal handlers */
static void HandlerSIGUSR1(int sig_num)
{
    assert(sig_num == SIGUSR1);
    
    /* atomic operation g_signal_cnt = 0; */
    __atomic_store_n(&g_signal_cnt, 0, __ATOMIC_SEQ_CST);

}

static void HandlerSIGUSR2(int sig_num)
{
    assert(sig_num == SIGUSR2);
    
    /* atomic operation g_stop_flag = 1; */
    __atomic_store_n(&g_stop_flag, TRUEE, __ATOMIC_SEQ_CST);
}


int MakeMeImmortal(int argc, char *argv[], size_t interval, size_t max_misses)
{
    wd_params_ty *wd_params = NULL;
    int status = 0;
    sem_t have_connection;
    
    /* Create semaphore */
    status = sem_init(&have_connection, 0, 0); 
    RETURN_IF_BAD(!status, "sem_init", MMI_FAIL);
    
    status = BlockSignals(); 
    RETURN_IF_BAD_CLEAN(!status, "BlockSignals \n", MMI_FAIL,
                                                sem_destroy(&have_connection));

    wd_params = CreateStruct(argc, argv, interval, max_misses, 0);
    RETURN_IF_BAD_CLEAN((NULL != wd_params), "CreateStruct \n", MMI_FAIL,
                                                sem_destroy(&have_connection));
    
    wd_params->have_connection = have_connection;
    wd_params->p_type = WD;

    wd_params->argv = CreateNewVector(argc, argv, interval, max_misses);
    RETURN_IF_BAD_CLEAN((NULL != wd_params->argv), "CreateNewVector \n", MMI_FAIL,
                                                DestroyAll(wd_params, NULL, NULL));
    /* Create watchdog thread */
    status = CreateNewThread(wd_params);
    RETURN_IF_BAD_CLEAN(!status, "CREATE_NEW_THREAD_FAIL", CREATE_NEW_THREAD_FAIL, DestroyAll(wd_params, NULL, wd_params->argv));
    
    status = sem_wait(&(wd_params->have_connection));
    RETURN_IF_BAD_CLEAN(!status, "sem_wait", SEM_WAIT_FAIL, DestroyAll(wd_params, NULL, wd_params->argv));

    /* destroy semaphore */
    status = sem_destroy(&have_connection);
    RETURN_IF_BAD_CLEAN(!status, "sem_destroy", SEM_DESTROY_FAIL, DestroyAll(wd_params, NULL, wd_params->argv));

    return status; 
}

static int BlockSignals(void)
{
    int status = SUCCESS;
    sigset_t mask;
    
    status = sigfillset(&mask);
    RETURN_IF_BAD(!status, "sigfillset failed", BLOCKSIGNALS_FAIL);
    
    
    status = sigprocmask(SIG_BLOCK, &mask, NULL);
    RETURN_IF_BAD(!status, "pthread_sigmask", BLOCKSIGNALS_FAIL);
    
    return status;
}

wd_params_ty *CreateStruct(int argc, char *argv[], size_t interval, size_t max_misses, pid_t other_pid)
{
    wd_params_ty *wd_params = (wd_params_ty *)malloc(sizeof(wd_params_ty));
    RETURN_IF_BAD((NULL != wd_params), "CreateParams", NULL);
    
    wd_params->argc = argc;
    wd_params->argv = argv;
    wd_params->interval = interval;
    wd_params->max_misses = max_misses;
    wd_params->other_pid = other_pid;
    wd_params->scheduler = NULL;
    
    return wd_params;
    
}

static int DestroyAll(wd_params_ty *wd_params, scheduler_ty *scheduler, char *argv[])
{
    int i = 0;
    
    if (NULL != scheduler)
    {
        SchedulerDestroy(scheduler);
    }
    
    if(NULL != argv)
    {
        for (i = 0; i < wd_params->argc + NUM_OF_ADDED_ARGS - 1; ++i)
        {
            free(wd_params->argv[i]);
            wd_params->argv[i] = NULL;
        }
        
        free(wd_params->argv);
        wd_params->argv = NULL;
    }
    
    if((NULL != wd_params) && (NULL != argv))
    {
        free(wd_params);
        wd_params = NULL;
    }

    return SUCCESS;
}


int CreateNewThread(wd_params_ty *wd_params)
{
    pthread_t wd_thread;
    pthread_attr_t attr;
    int status = 0;
    
    assert(NULL != wd_params);
    
    status = pthread_attr_init(&attr);
    RETURN_IF_BAD(!status, "pthread_attr_init\n", FAILED);
    
    status = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    RETURN_IF_BAD_CLEAN(!status, "pthread_attr_setdetachstate\n", FAILED,
                                                pthread_attr_destroy(&attr));
    
    status = pthread_create(&wd_thread, &attr, WDRoutine, (void*)wd_params);
    RETURN_IF_BAD_CLEAN(!status, "pthread_create\n", FAILED,
                                                pthread_attr_destroy(&attr));
    
    status = pthread_attr_destroy(&attr);
    RETURN_IF_BAD(!status, "pthread_attr_destroy\n", FAILED);
    
    return SUCCESS;
}

static char **CreateNewVector(int argc, char *argv[], size_t interval, size_t max_misses)
{
    char **new_vector = NULL;
    char *file_name = FILE_NAME;
    
    new_vector = (char **)malloc(sizeof(char *) * (argc + NUM_OF_ADDED_ARGS + 1));
    RETURN_IF_BAD((NULL != new_vector), "malloc failed", NULL);
    
    new_vector[0] = (char *)malloc(sizeof(char ) * (strlen(file_name) + 1));
    RETURN_IF_BAD_CLEAN((NULL != new_vector), "malloc failed", NULL, free(new_vector));
    
    strcpy(new_vector[0], file_name);
    
    new_vector[1] = AllocNumber(interval);
    RETURN_IF_BAD((NULL != new_vector[1]), "malloc failed", NULL);
    
    new_vector[2] = AllocNumber(max_misses);
    RETURN_IF_BAD((NULL != new_vector[2]), "malloc failed", NULL);
    
    memcpy(new_vector + NUM_OF_ADDED_ARGS, argv, sizeof(char *) * (argc + 1));
    
    return new_vector;
}

static char *AllocNumber(size_t num)
{
    char *ptr = NULL;
    char value[BUFFER_SIZE];
    size_t len = 0;
    
    len = sprintf(value, "%ld", num);
    
    value[len] = '\0';
    
    ptr = (char *)malloc(sizeof(char *) * (len + 1));
    RETURN_IF_BAD((NULL != ptr), "malloc failed", NULL);
    
    strcpy(ptr, value);
    
    return ptr;
}

int DoNotResuscitate(void)
{
    int status = SUCCESS;
    
    status = kill(0, SIGUSR2);
    RETURN_IF_BAD(!status, "kill SIGUSR2  failed", FAILED);
    
    status = sem_wait(&(g_dnr_return));
    RETURN_IF_BAD(!status, "sem_wait", FAILED);
    
    status = sem_destroy(&(g_dnr_return)); /* check return value */
    RETURN_IF_BAD(!status, "sem_wait", FAILED);
    
    /* return status */
    return status;
}

static int UnBlock()
{
    sigset_t mask;
    int status = SUCCESS;
    
    /* update mask to unblock SIGUSR1 & SIGUSR2 */
    status = sigemptyset(&mask);
    RETURN_IF_BAD(!status, "sigemptyset failed\n", FAILED);
    
    status = sigaddset(&mask, SIGUSR1);
    RETURN_IF_BAD(!status, "sigaddset failed\n", FAILED);

    status = sigaddset(&mask, SIGUSR2);
    RETURN_IF_BAD(!status, "sigaddset failed\n", FAILED);
    
    status = sigprocmask(SIG_UNBLOCK, &mask, NULL);
    RETURN_IF_BAD(!status, "sigprocmask failed\n", FAILED);

    return status;
}

static void *WDRoutine(void *params)
{
    int status = 0; /*TODO ASSERT */
    wd_params_ty *wd_params = (wd_params_ty *)params;

    status = UnBlock();
    RETURN_IF_BAD(!status, "UnBlock failed\n", NULL);

     /* use WDFunc(params); */
    WDFunc(wd_params, 1);
    
    DestroyAll(wd_params, NULL, wd_params->argv);
    
    return NULL;
    
}

static int SignOfLife(void *params)
{
    int status = SUCCESS; 
    wd_params_ty *wd_params = (wd_params_ty *)params;
    
    /* ++g_signal_cnt */
    __atomic_fetch_add(&g_signal_cnt, 1, 0);
    
    /* send SIGUSR1 to  wd App  */
    status = kill(wd_params->other_pid, SIGUSR1);
    
    fprintf(stderr,"send to:%d \n", wd_params->other_pid);
    
    RETURN_IF_BAD(!status, "kill failed\n", 0);

    return status;
}

static int CheckSignOfLife(void *params)
{
    wd_params_ty *wd_params = (wd_params_ty *)params;
    int status = 0;
    /* check if g_stop_flag == 1 */
    if (TRUEE == g_stop_flag)
    {
        /* SchedulerStop(params->scheduler) */
        SchedulerStop(wd_params->scheduler);
    }
    else if(0 == (wd_params->other_pid))
    {
        
        status = IsWatchDogExist(wd_params);
            
        if(status)
        {
            fputs("Already Have WD App\n\n", stderr);
               
            sem_post(&(wd_params->have_connection));
                
            return SUCCESS;
        }
        Revive(wd_params);
        
        return SUCCESS;
    }

    /* if g_signal_cnt > params->max_misses */
    else if(g_signal_cnt >= wd_params->max_misses)
    {

        /* Revive(params) */
        Revive(wd_params);
    }
    
    return SUCCESS;
}

static void CleanFunc(ilrd_uid_ty uid, void *params)
{
    (void)uid;
    (void)params;
}


int WDFunc(wd_params_ty *params, int should_post)
{
    ilrd_uid_ty uid; 
    int status = 0;
    
    /* Install signal handler for SIGUSR1 */
    status = InstallSignalHandlers();
    RETURN_IF_BAD(!status, "SchedulerAddTask ", FAILED);
    
    status = sem_init(&g_dnr_return, 0, 0);
    RETURN_IF_BAD(!status, "SchedulerAddTask ", FAILED);
    
    /* scheduler_ty *scheduler = SchedulerCreate(); */
    params->scheduler = SchedulerCreate();
    
    
    /* Add task to scheduler - SignOfLife */
    uid = SchedulerAddTask(params->scheduler, params->interval, 
                            SignOfLife, (void *)params,
                            CleanFunc);
    
    status = UIDIsSame(uid, UIDBadID);
    RETURN_IF_BAD(!status, "SchedulerAddTask ", FAILED);

    /* Add task to scheduler - CheckSignOfLife */
    uid = SchedulerAddTask(params->scheduler, params->interval, 
                            CheckSignOfLife, (void *)params,
                            CleanFunc);
    status = UIDIsSame(uid, UIDBadID);
    RETURN_IF_BAD(!status, "SchedulerAddTask ", FAILED);

    
    /* check if should_post */
    if(should_post)
    {
        /* add task IsConnected(_mmi_return), short interval */
        uid = SchedulerAddTask(params->scheduler, 1, IsConnected,
         (void *)params, CleanFunc);
        
        status = UIDIsSame(uid, UIDBadID);
        RETURN_IF_BAD(!status, "SchedulerAddTask", FAILED);
    }

    /* Run scheduler */
    SchedulerRun(params->scheduler);
    
    sem_post(&g_dnr_return);
    
    DestroyAll(params, params->scheduler, NULL);
    
    return SUCCESS;
}

static int IsConnected(void *wd)
{
    int status = 0;
    
    /* release the MMI (by sem_wait) */
    if (((wd_params_ty *)wd)->other_pid != 0)
    {
        status = kill(((wd_params_ty *)wd)->other_pid, 0);
        if (0 == status)
        {
            fprintf(stderr,"IsConnected to: %d\n", ((wd_params_ty *)wd)->other_pid);

            if (0 != sem_post(&((wd_params_ty *)wd)->have_connection))
            {
                fprintf(stderr, "sem_post FAILED!\n");
            }
            
            return FAILED;
        }
    }
    
    return SUCCESS;
}


static int Revive(wd_params_ty *params)
{
    pid_t other_pid = -1;
    int status = 0;
    
    /* collect the zombie process */
    if (params->other_pid != 0)
    {
        waitpid(-1, NULL, WNOHANG);
    }    
    
     /* do a fork */
    other_pid = fork();
    
    RETURN_IF_BAD((other_pid >= 0), "fork", 1);
    
    if (0 == other_pid)
    {
        /* child process: */
        /*execvp wd_app.out */
        status = execvp(params->argv[0], params->argv);
        
        RETURN_IF_BAD(!status, "execvp Failed", FAILED);
    }
    else
    {
        /* else */
            /* parent process: */
        params->other_pid = other_pid;
        __atomic_store_n(&g_signal_cnt, 0, __ATOMIC_SEQ_CST);
        status = waitpid(other_pid, NULL, 1);
        RETURN_IF_BAD(!status, "waitpid Failed", FAILED);
    }

    /* return status; */
    return SUCCESS;
}

static int IsWatchDogExist(wd_params_ty *wd)
{
    pid_t wd_pid = 0;
    
    assert(wd);
    
    wd_pid = GetEnvNum("WD_PID");
    
    if (0 == wd_pid)
    {
        return 0;
    }
    else
    {
        if(wd_pid == getppid())
        {
            wd->other_pid = wd_pid;
            
            fprintf(stderr, "IsWatchDogExist(), connect to wd_pid %d \n\n", wd_pid);
            
            return FAILED;
        }
    }

    return SUCCESS;
}

void SetEnvNum(const char *var_name, int var)
{
    int status = 0;
    char pid_str[24];
    size_t len = 0;
    
    assert(var_name);

    len = sprintf(pid_str, "%d", var);
    assert(len);
    pid_str[len] = '\0';

    status = setenv(var_name, pid_str, 1);
    assert(-1 != status);
}

static pid_t GetEnvNum(const char *var_name)
{
    char *value = NULL;
    
    assert(var_name);
    
    value = getenv(var_name);
    
    if(NULL == value)
    {
        return SUCCESS;
    }
    
    return atoi(value);
}



static int InstallSignalHandlers(void)
{
    struct sigaction sigusr1_act;
    struct sigaction sigusr2_act;
    int status = 0;

    /* Install signal handler for SIGUSR1 */
    sigusr1_act.sa_flags = 0;
    sigusr1_act.sa_handler = HandlerSIGUSR1;
    
    status = sigemptyset(&sigusr1_act.sa_mask);
    RETURN_IF_BAD(!status, "sigemptyset", FAILED);
    
    status = sigaction(SIGUSR1, &sigusr1_act, NULL);
    RETURN_IF_BAD(!status, "sigaction", FAILED);

    /* Install signal handler for SIGUSR2 */
    sigusr2_act.sa_flags = 0;
    sigusr2_act.sa_handler = HandlerSIGUSR2;
    
    status = sigemptyset(&sigusr2_act.sa_mask);
    RETURN_IF_BAD(!status, "sigemptyset", FAILED);
    
    status = sigaction(SIGUSR2, &sigusr2_act, NULL);
    RETURN_IF_BAD(!status, "sigaction", FAILED);
    
    return SUCCESS;

}


