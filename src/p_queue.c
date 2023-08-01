/*******************************************************************
*Date: 27.12.22
*Author: Aviv Jilin
*reviewer: Eliran 
*version: 1.0
*****************************************************************/

#include <stdlib.h> /* malloc, free */ 
#include <assert.h> /* assert       */ 
#include <stddef.h> /* size_t       */

#include "p_queue.h"
#include "sorted_list.h"

struct p_queue 
{
    sort_list_ty *sort_list;
};

/*******************************************************************************
 * Create an empty p_queue with priorities determined by "cmp_priority", 
 * as defined in "utilities.h"
 * Returns pointer to the p_queue on success, NULL otherwise
 * Time Complexity: determined by the used system call complexity
*******************************************************************************/
p_queue_ty *PQueueCreate(cmp_func_ty cmp_priority)
{   
    p_queue_ty *p_queue = NULL;

    assert(NULL != cmp_priority);

    p_queue = (p_queue_ty *)malloc(sizeof(p_queue_ty));

    if(NULL == p_queue)
    {
        return NULL;
    }

    p_queue->sort_list = SortedListCreate(cmp_priority);

    if(NULL == p_queue->sort_list)
    {
        free(p_queue);
        p_queue = NULL;

        return NULL;
    }

    return p_queue;
}


/*******************************************************************************
 * Frees all resources used by "p_queue"
 * note: undefined behaviour if "p_queue" is NULL
 * Time Complexity: determined by the used system call complexity
*******************************************************************************/
void PQueueDestroy(p_queue_ty *p_queue)
{
    assert(NULL != p_queue);

    SortedListDestroy(p_queue->sort_list);
    
    free(p_queue); 
    
    p_queue = NULL;
}

/*******************************************************************************
 * Returns the data from the front of the "p_queue"
 * note: undefined behaviour if "p_queue" is empty or NULL
 * Time Complexity: O(1)
*******************************************************************************/
void *PQueuePeek(const p_queue_ty *p_queue)
{
    assert(NULL != p_queue);

    return SortedListGetData(SortedListPrev(SortedListEnd(p_queue->sort_list)));
}

/*******************************************************************************
 * Adds data to "p_queue" according to it's priority
 * returns 0 if succeeded, not 0 otherwise
 * note: undefined behaviour if "p_queue" is NULL
 * Time Complexity: ~O(n) (determined by the used system call complexity) 
*******************************************************************************/
int PQueueEnqueue(p_queue_ty *p_queue, const void *data)
{
    assert(NULL != p_queue);

    return (SortedListIterIsEqual((SortedListEnd(p_queue->sort_list)), 
                        (SortedListInsert(p_queue->sort_list, (void *)data))));
}


/*******************************************************************************
 * Removes data from the front of the "p_queue"
 * note: undefined behaviour if "p_queue" is empty or NULL
 * Time Complexity: ~O(1) (determined by the used system call complexity)
*******************************************************************************/
void PQueueDequeue(p_queue_ty *p_queue)
{
    assert(NULL != p_queue);

    SortedListPopBack(p_queue->sort_list);    
}

/*******************************************************************************
 * Returns 1 if the "p_queue" is empty, 0 otherwise
 * note: undefined behaviour if "p_queue" is NULL
 * Time Complexity: O(1)
*******************************************************************************/
int PQueueIsEmpty(p_queue_ty *p_queue)
{
   assert(NULL != p_queue);

   return SortedListIsEmpty(p_queue->sort_list);
}

/*******************************************************************************
 * Returns the size of the "p_queue"
 * note: undefined behaviour if "p_queue" is NULL
 * Time Complexity: O(n)
*******************************************************************************/
size_t PQueueSize(p_queue_ty *p_queue)
{
    assert(NULL != p_queue);

    return(SortedListSize(p_queue->sort_list));
}

/*******************************************************************************
 * Clears all elements from the "p_queue", without deleting "p_queue" itself
 * note: undefined behaviour if "p_queue" is NULL
 * Time Complexity:~O(n) (determined by the used system call complexity)
*******************************************************************************/
void PQueueClear(p_queue_ty *p_queue)
{
    assert(NULL != p_queue);

    while (!PQueueIsEmpty(p_queue))
    {
        SortedListRemove(SortedListBegin(p_queue->sort_list));
    }
}


/*******************************************************************************
 * Removes the first occurrence of "data" in "p_queue", determined by
 * "match_func" as defined in "utilities.h"
 * note: undefined behaviour if "p_queue" is empty or NULL
 * Time Complexity: O(n) (determined by the used system call complexity)
*******************************************************************************/
void *PQueueErase(p_queue_ty *p_queue, is_match_func_ty match_func, void *param)
{
    sort_list_iter_ty to_erase;
    void *data = NULL;

    assert(NULL != p_queue);
    assert(NULL != match_func);

    to_erase = SortedListFindIf(SortedListBegin(p_queue->sort_list), 
        SortedListEnd(p_queue->sort_list), match_func, param);
    if (SortedListIterIsEqual(to_erase, SortedListEnd(p_queue->sort_list)))
    {
        return data;
    }

    data = SortedListGetData(to_erase);

    SortedListRemove(to_erase);

    return data;
}



