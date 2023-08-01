/*******************************************************************************
 * Author:  HRD28
 * Version: 1.0
*******************************************************************************/
#ifndef __P_QUEUE_H__
#define __P_QUEUE_H__

#include <stddef.h>     /*  size_t       */
#include "utilities.h"  /*  cmp_func_ty  */

typedef struct p_queue p_queue_ty;

/*******************************************************************************
 * Create an empty p_queue with priorities determined by "cmp_priority", 
 * as defined in "utilities.h"
 * Returns pointer to the p_queue on success, NULL otherwise
 * Time Complexity: determined by the used system call complexity
*******************************************************************************/
p_queue_ty *PQueueCreate(cmp_func_ty cmp_priority);

/*******************************************************************************
 * Frees all resources used by "p_queue"
 * note: undefined behaviour if "p_queue" is NULL
 * Time Complexity: determined by the used system call complexity
*******************************************************************************/
void PQueueDestroy(p_queue_ty *p_queue);

/*******************************************************************************
 * Returns the data from the front of the "p_queue"
 * note: undefined behaviour if "p_queue" is empty or NULL
 * Time Complexity: O(1)
*******************************************************************************/
void *PQueuePeek(const p_queue_ty *p_queue);

/*******************************************************************************
 * Adds data to "p_queue" according to it's priority
 * returns 0 if succeeded, not 0 otherwise
 * note: undefined behaviour if "p_queue" is NULL
 * Time Complexity: ~O(n) (determined by the used system call complexity) 
*******************************************************************************/
int PQueueEnqueue(p_queue_ty *p_queue, const void *data);

/*******************************************************************************
 * Removes data from the front of the "p_queue"
 * note: undefined behaviour if "p_queue" is empty or NULL
 * Time Complexity: ~O(1) (determined by the used system call complexity)
*******************************************************************************/
void PQueueDequeue(p_queue_ty *p_queue);

/*******************************************************************************
 * Returns 1 if the "p_queue" is empty, 0 otherwise
 * note: undefined behaviour if "p_queue" is NULL
 * Time Complexity: O(1)
*******************************************************************************/
int PQueueIsEmpty(p_queue_ty *p_queue);

/*******************************************************************************
 * Returns the size of the "p_queue"
 * note: undefined behaviour if "p_queue" is NULL
 * Time Complexity: O(n)
*******************************************************************************/
size_t PQueueSize(p_queue_ty *p_queue);

/*******************************************************************************
 * Clears all elements from the "p_queue", without deleting "p_queue" itself
 * note: undefined behaviour if "p_queue" is NULL
 * Time Complexity: ~O(n) (determined by the used system call complexity)
*******************************************************************************/
void PQueueClear(p_queue_ty *p_queue);

/*******************************************************************************
 * Removes the first occurrence of "data" in "p_queue", determined by
 * "match_func" as defined in "utilities.h"
 * note: undefined behaviour if "p_queue" is empty or NULL
 * Time Complexity: ~O(n) (determined by the used system call complexity)
*******************************************************************************/
void *PQueueErase(p_queue_ty *p_queue, is_match_func_ty match_func, void *param);

#endif /*   __P_QUEUE_H__     */
