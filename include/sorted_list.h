/*******************************************************************************
 * Author:  HRD28
 * Version: 1.1
*******************************************************************************/
#ifndef __SORTED_LIST_H__
#define __SORTED_LIST_H__

#include <stddef.h>     /*  size_t                              */
#include "dlist.h"      /*  dlist_iter_ty                       */
#include "utilities.h"  /*  is_match_func_ty, action_func_ty,
                            cmp_func_ty                         */

/*  "sort_list" handler                                                       */
typedef struct sort_list sort_list_ty;

/*  sort_list_node is a temporary object, it might change, do not use it      */
typedef struct sort_list_iter
{
    dlist_iter_ty dlist_iter;
    #ifdef NDEBUG
    sort_list_ty *list;
    #endif
} sort_list_iter_ty;

/*******************************************************************************
 *  creates an empty sorted list - "sort_list" with the sorting criteria 
 *  evaluated by "cmp_func"
 *  returns pointer to "sort_list" on success
 *  or NULL on failure
 *  Time Complexity: determined by the used system call complexity
*******************************************************************************/
sort_list_ty *SortedListCreate(cmp_func_ty cmp_func);

/*******************************************************************************
 *  frees all resources used by "sort_list"
 *  note: undefined behaviour if "sort_list" is NULL
 *  Time Complexity: determined by the used system call complexity
*******************************************************************************/
void SortedListDestroy(sort_list_ty *sort_list);

/*******************************************************************************
 *  returns the number of elements in "sort_list"
 *  note: undefined behaviour if "sort_list" is NULL
 *  Time Complexity: O(n)
*******************************************************************************/
size_t SortedListSize(const sort_list_ty *sort_list);

/*******************************************************************************
 *  returns 1 if "sort_list" is empty, 0 otherwise
 *  note: undefined behaviour if "sort_list" is NULL
 *  Time Complexity: O(1)
*******************************************************************************/
int SortedListIsEmpty(const sort_list_ty *sort_list);

/*******************************************************************************
 *  returns an iterator to the first valid element in "sort_list", SortedListEnd() 
 *  if empty.
 *  note: undefined behaviour if "sort_list" is NULL
 *  Time Complexity: O(1)
*******************************************************************************/
sort_list_iter_ty SortedListBegin(const sort_list_ty *sort_list);

/*******************************************************************************
 *  returns an iterator to the end of "sort_list", this is an invalid element
 *  use SortedListPrev(SortedListEnd(sort_list)) to get last valid element in list
 *  note: undefined behaviour if "sort_list" is NULL
 *  Time Complexity: O(1)
*******************************************************************************/
sort_list_iter_ty SortedListEnd(const sort_list_ty *sort_list);

/*******************************************************************************
 *  returns an iterator to the next element in sort_list
 *  note: undefined behaviour if "iter" is SortedListEnd()
 *  Time Complexity: O(1)
*******************************************************************************/
sort_list_iter_ty SortedListNext(sort_list_iter_ty iter);

/*******************************************************************************
 *  returns an iterator to the previous element in sort_list
 *  note: undefined behaviour if "iter" is SortedListBegin()
 *  Time Complexity: O(1)
*******************************************************************************/
sort_list_iter_ty SortedListPrev(sort_list_iter_ty iter);

/*******************************************************************************
 *  returns 1 if two iterators are pointing to the same element in a sort_list,
 *  0 otherwise
 *  Time Complexity: O(1)
*******************************************************************************/
int SortedListIterIsEqual(sort_list_iter_ty iter1, sort_list_iter_ty iter2);

/*******************************************************************************
 *  returns data stored in "iter"
 *  note: undefined behaviour if "iter" is SortedListEnd()
 *  Time Complexity: O(1)
*******************************************************************************/
void *SortedListGetData(sort_list_iter_ty iter);

/*******************************************************************************
 *  inserts "data" to the correct position in the list
 *  returns an iterator to the added data if succeeded, SortedListEnd() otherwise
 *  note: may cause iterator invalidity
 *  Time Complexity: O(n)
*******************************************************************************/
sort_list_iter_ty SortedListInsert(sort_list_ty *sort_list, const void *data);

/*******************************************************************************
 *  removes the element at "iter_to_remove" and returns iterator to the next 
 *  element in the sort_list
 *  note: undefined behaviour if "iter_to_remove" is SortedListEnd()
 *  Time Complexity: O(1)
*******************************************************************************/
sort_list_iter_ty SortedListRemove(sort_list_iter_ty iter_to_remove);

/*******************************************************************************
 *  removes the first element of "sort_list" and returns the data that was 
 *  stored in the removed element
 *  note: undefined behaviour if "sort_list" is empty or NULL
 *  Time Complexity: determined by the used system call complexity
*******************************************************************************/
void *SortedListPopFront(sort_list_ty *sort_list);

/*******************************************************************************
 *  removes the last element of "sort_list" and returns the data that was 
 *  stored in the removed element
 *  note: undefined behaviour if "sort_list" is empty or NULL
 *  Time Complexity: determined by the used system call complexity
*******************************************************************************/
void *SortedListPopBack(sort_list_ty *sort_list);

/*******************************************************************************
 *  performs "action" on each element in the range ["from", "to") till success
 *  returns 0 if succeeded, not 0 otherwise
 *  note: undefined behaviour if "from" is SortedListEnd() or if "from" and 
 *        "to" are in different sort_lists
          "from" must be before "to" in the list
          undefined behaviour if "action" causes de-sorting of the list
 *  Time Complexity: O(n)
*******************************************************************************/
int SortedListForeach(sort_list_iter_ty from, sort_list_iter_ty to, 
                                            action_func_ty action, void *param);

/*******************************************************************************
 *  merges "src" into "dest", "src" is destroyed
 *  returns a pointer to "dest"
 *  note: undefined behaviour if either one of the lists ptrs is NULL
 *        or if the lists have different sorting criteria
 *  Time Complexity: O(n)
*******************************************************************************/
void SortedListMerge(sort_list_ty *dest, sort_list_ty *src);

/*******************************************************************************
 *  performs "cmp_func" on each element in the range ["from", "to") till 
 *  success
 *  if found, returns the iterator to that element, returns "to" iter otherwise
 *  note: undefined behaviour if "from" is SortedListEnd() or if "from" and 
 *        "to" are in different sort_lists
          "from" must be before "to" in the list
 *  Time Complexity: O(n)
*******************************************************************************/
sort_list_iter_ty SortedListFind(sort_list_ty *list, sort_list_iter_ty from, 
                                 sort_list_iter_ty to, const void *param);

/*******************************************************************************
 *  performs "match_func" on each element in the range ["from", "to") till 
 *  success
 *  if found, returns the iterator to that element, returns "to" iter otherwise
 *  note: undefined behaviour if "from" is SortedListEnd() or if "from" and 
 *        "to" are in different sort_lists
          "from" must be before "to" in the list
 *  Time Complexity: O(n)
*******************************************************************************/
sort_list_iter_ty SortedListFindIf(sort_list_iter_ty from, sort_list_iter_ty to, 
                                is_match_func_ty match_func, const void *param);

#endif  /*  __SORTED_LIST_H__   */
