#include <stdlib.h> /* malloc, free */
#include <assert.h> /* assert       */

#include "task.h" 

struct task
{
    ilrd_uid_ty uid;
    oper_func_ty operation;
    void *operation_param;
    clean_func_ty clean;
    time_t time_to_run;
    size_t interval;
};

task_ty *TaskCreate(oper_func_ty operation, size_t interval, clean_func_ty clean_func, void *param)
{
    task_ty *new_task = NULL;

    assert(NULL != operation);
    assert(0 != interval);

    new_task = (task_ty *)malloc(sizeof(task_ty));
    if (NULL == new_task)
    {
        return NULL;
    }

    new_task->uid = UIDCreate();
    if (UIDIsSame((new_task->uid), UIDBadID))
    {
        return NULL;
    }

    new_task->operation = operation;
    new_task->operation_param = param;
    new_task->clean = clean_func;
    new_task->time_to_run = new_task->uid.time;
    new_task->interval = interval;

    return new_task;
}

void TaskDestroy(task_ty *task)
{
    assert (NULL != task);

    task->clean(task->uid, task->operation_param);

    free(task);
    task = NULL;
}

int TaskRun(task_ty *task)
{
    assert (NULL != task);

    return task->operation(task->operation_param);
}

time_t TaskGetTimeToRun(const task_ty *task)
{
    assert (NULL != (task_ty *)task);

    return task->time_to_run;
}

ilrd_uid_ty TaskGetUID(const task_ty *task)
{
    assert (NULL != (task_ty *)task);
    
    return task->uid;
}

void TaskUpdateTimeToRun(task_ty *task)
{
    assert (NULL != task);

    task->time_to_run = task->time_to_run + task->interval;
}

int TaskIsMatchUID(const task_ty *task, ilrd_uid_ty uid)
{
    assert (NULL != (task_ty *)task);

    return UIDIsSame(task->uid, uid);
}

int TaskIsBefore(const task_ty *task1, const task_ty *task2, void *param)
{
    (void)param;
    assert (NULL != task1);
    assert (NULL != task2);

    return task1->time_to_run < task2->time_to_run;
}



 
