
#include <stdlib.h> /* malloc, free */
#include <time.h> /* time_t         */
#include <assert.h> /* assert       */
#include <stddef.h> /* size_t       */
#include <unistd.h> /* sleep       */

#include "scheduler.h"
#include "p_queue.h"
#include "task.h"

struct scheduler
{
    p_queue_ty *p_queue;
    int stop;
};

static int CmpExecutionTime(void *task1, void *task2)
{
    return TaskIsBefore((task_ty *)task2, (task_ty *)task1, NULL);
}

static int match(void *task, void *uid)
{
    return UIDIsSame(TaskGetUID((task_ty *)task), *((ilrd_uid_ty *)uid));
}

scheduler_ty *SchedulerCreate(void)
{
    scheduler_ty *scheduler = (scheduler_ty *)malloc(sizeof(scheduler_ty));
    if (NULL == scheduler)
    {
        return NULL;
    }

    scheduler->p_queue = PQueueCreate(CmpExecutionTime);
    if (NULL == scheduler->p_queue)
    {
        free(scheduler);
        return NULL;
    }

    scheduler->stop = 0;

    return scheduler;
}

void SchedulerDestroy(scheduler_ty *scheduler)
{
    assert(NULL != scheduler);

    PQueueDestroy(scheduler->p_queue);
    free(scheduler);
}

ilrd_uid_ty SchedulerAddTask(scheduler_ty *scheduler, size_t interval, 
                            oper_func_ty operation, void *param,
                            clean_func_ty clean_func)
{
    task_ty *new_task = NULL;

    assert (NULL != scheduler);

    new_task = TaskCreate(operation, interval, clean_func, param);
    if (NULL == new_task)
    {
        return UIDBadID;
    }

    if (0 != PQueueEnqueue(scheduler->p_queue, new_task))
    {
        TaskDestroy(new_task);
        return UIDBadID;
    }

    return TaskGetUID(new_task);
}

int SchedulerRemoveTask(scheduler_ty *scheduler, ilrd_uid_ty uid)
{
    task_ty *curr_task = NULL;

    assert(NULL != scheduler);
    assert(1 != PQueueIsEmpty(scheduler->p_queue));
    assert(!UIDIsSame(UIDBadID,uid));

    curr_task = PQueueErase(scheduler->p_queue, match, &uid);
    if (NULL != curr_task)
    {
        TaskDestroy(curr_task);
        return 0;
    }
    return 1;
}


int SchedulerRun(scheduler_ty *scheduler)
{
    task_ty *curr_task = NULL;
    int oper_result = 0;
    time_t sleep_time = 0;

    assert(NULL != scheduler);

    while (!scheduler->stop && !SchedulerIsEmpty(scheduler))
    {
        curr_task = (task_ty *)PQueuePeek(scheduler->p_queue);
        sleep_time = TaskGetTimeToRun(curr_task) - time(NULL);
        if (sleep_time > 0)
        {
            sleep(sleep_time);
        }
        
        PQueueDequeue(scheduler->p_queue);
        
        oper_result = TaskRun(curr_task);
        
        if (0 != oper_result)
        {
            TaskDestroy(curr_task);
        }
        else
        {
            TaskUpdateTimeToRun(curr_task);
            PQueueEnqueue(scheduler->p_queue, curr_task);
        }
    }
    if (scheduler->stop && !SchedulerIsEmpty(scheduler))
    {
        SchedulerClear(scheduler);
    }
    return 0;
}

int SchedulerRun2(scheduler_ty *scheduler)
{
    task_ty *task = NULL;
    time_t time_to_run;
    int break_task = 1;
    
    assert(scheduler);
    
    scheduler->stop = 0;
    
    while(!(scheduler->stop) && (!SchedulerIsEmpty(scheduler)))
    {
        task = PQueuePeek(scheduler->p_queue);
        
        PQueueDequeue(scheduler->p_queue);
        
        time_to_run = TaskGetTimeToRun(task);
        
        while(time_to_run > time(NULL)); 
        
        if (TaskRun(task) == break_task)
        {
            TaskDestroy(task);
        }
        else
        {
            TaskUpdateTimeToRun(task);
                        
            if (1 == PQueueEnqueue(scheduler->p_queue ,(void *)task))
            {
                TaskDestroy(task);
                
                return 1;
            }
        }
    }
    
    return 0;
}


void SchedulerStop(scheduler_ty *scheduler)
{
    assert(NULL != scheduler);

    scheduler->stop = 1;
}

size_t SchedulerSize(scheduler_ty *scheduler)
{
    assert(NULL != scheduler);

    return PQueueSize(scheduler->p_queue);
}

int SchedulerIsEmpty(scheduler_ty *scheduler)
{
    assert(NULL != scheduler);

    return PQueueIsEmpty(scheduler->p_queue);
}

void SchedulerClear(scheduler_ty *scheduler)
{
    task_ty *curr_task = NULL;

    assert(NULL != scheduler);

    while(!SchedulerIsEmpty(scheduler))
    {
        curr_task = (task_ty *)PQueuePeek(scheduler->p_queue);
        TaskDestroy(curr_task);
        PQueueDequeue(scheduler->p_queue);
    }
    PQueueClear(scheduler->p_queue);
}
