/*******************************************************************************
 * Author:  HRD28
 * Version: 1.0
*******************************************************************************/

#ifndef __TASK_H__
#define __TASK_H__

#include <time.h>   /*  time_t  */
#include <stddef.h> /* size_t   */
#include "uid.h"    /*  ilrd_uid_ty     */
#include "scheduler.h" /* oper_func_ty, clean_func_ty */

typedef struct task task_ty;

/*******************************************************************************
 * Creates a new task which performs "operation" with "param", if needed
 * the task can be cleaned with "clean_func"
 * returns a pointer to the created task if succeeded, NULL otherwise
 * note: Undefined behaviour if "interval" equals 0 or if "operation" is NULL
 * Time Complexity: determined by the used system call complexity
*******************************************************************************/
task_ty *TaskCreate(oper_func_ty operation, size_t interval, 
                    clean_func_ty clean_func, void *param);

/*******************************************************************************
 * Frees all resources used by "task"
 * note: undefined behaviour if "task" is NULL
 * Time Complexity: determined by the used system call complexity
*******************************************************************************/
void TaskDestroy(task_ty *task);

/*******************************************************************************
 * Performs the operation in "task"
 * returns 0 if "operation" should repeat, 1 if it should stop
 * note: undefined behaviour if "task" is NULL
 * Time Complexity: O(1)
*******************************************************************************/
int TaskRun(task_ty *task);

/*******************************************************************************
 * Returns the "task"'s "time_to_run"
 * note: undefined behaviour if "task" is NULL
 * Time Complexity: O(1)
*******************************************************************************/
time_t TaskGetTimeToRun(const task_ty *task);

/*******************************************************************************
 * Returns the "task"'s "uid"
 * note: undefined behaviour if "task" is NULL
 * Time Complexity: O(1)
*******************************************************************************/
ilrd_uid_ty TaskGetUID(const task_ty *task);

/*******************************************************************************
 * Updates "task"'s "time_to_run", according to its "interval"
 * note: undefined behaviour if "task" is NULL
 * Time Complexity: O(1)
*******************************************************************************/
void TaskUpdateTimeToRun(task_ty *task);

/*******************************************************************************
 * Returns 1 if "task"'s uid matches "uid", 0 otherwise
 * note: undefined behaviour if "task" is NULL
 * Time Complexity: O(1)
*******************************************************************************/
int TaskIsMatchUID(const task_ty *task, ilrd_uid_ty uid);

/*******************************************************************************
 * Returns 1 if "task1"'s "time_to_run" is before "task2"'s "time_to_run",
 * 0 otherwise
 * note: undefined behaviour if "task1" or "task2" are NULL
 * Time Complexity: O(1)
*******************************************************************************/
int TaskIsBefore(const task_ty *task1, const task_ty *task2, void *param);

#endif /* __TASK_H__ */