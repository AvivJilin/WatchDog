/*******************************************************************************
Author:  HRD28
Version: 1.1
*******************************************************************************/
#ifndef __DLIST_H__
#define __DLIST_H__

#include <stddef.h>     /*  size_t                              */
#include "utilities.h"  /*  is_match_func_ty, action_func_ty    */

/*  dlist_node is a temporary object, it might change, do not use it          */
typedef struct dlist_node *dlist_iter_ty;

/*  "dlist" handler                                                           */
typedef struct dlist dlist_ty;

/*******************************************************************************
 *  creates an empty doubly linked list - "dlist"
 *  returns pointer to "dlist" on success
 *  or NULL on failure
 *  Time Complexity: determined by the used system call complexity
*******************************************************************************/
dlist_ty *DlistCreate(void);

/*******************************************************************************
 *  frees all resources used by "dlist"
 *  note: undefined behaviour if "dlist" is NULL
 *  Time Complexity: determined by the used system call complexity
*******************************************************************************/
void DlistDestroy(dlist_ty *dlist);

/******************************************************************************* 
 *  returns the number of elements in "dlist"
 *  note: undefined behaviour if "dlist" is NULL
 *  Time Complexity: O(n)
*******************************************************************************/
size_t DlistSize(const dlist_ty *dlist);

/******************************************************************************* 
 *  returns 1 if "dlist" is empty, 0 otherwise
 *  note: undefined behaviour if "dlist" is NULL
 *  Time Complexity: O(1)
*******************************************************************************/
int DlistIsEmpty(const dlist_ty *dlist);

/******************************************************************************* 
 *  returns an iterator to the first valid element in "dlist", DlistIterEnd() 
 *  if empty.
 *  note: undefined behaviour if "dlist" is NULL
 *  Time Complexity: O(1)
*******************************************************************************/
dlist_iter_ty DlistIterBegin(const dlist_ty *dlist);

/******************************************************************************* 
 *  returns an iterator to the end of "dlist", this is an invalid element
 *  use DlistIterPrev(DlistIterEnd(dlist)) to get last valid element in list
 *  note: undefined behaviour if "dlist" is NULL
 *  Time Complexity: O(1)
*******************************************************************************/
dlist_iter_ty DlistIterEnd(const dlist_ty *dlist);

/******************************************************************************* 
 *  returns an iterator to the next element in dlist
 *  note: undefined behaviour if "iter" is DlistIterEnd()
 *  Time Complexity: O(1)
*******************************************************************************/
dlist_iter_ty DlistIterNext(dlist_iter_ty iter);

/******************************************************************************* 
 *  returns an iterator to the previous element in dlist
 *  note: undefined behaviour if "iter" is DlistIterBegin()
 *  Time Complexity: O(1)
*******************************************************************************/
dlist_iter_ty DlistIterPrev(dlist_iter_ty iter);

/******************************************************************************* 
 *  returns 1 if two iterators are pointing to the same element in a dlist,
 *  0 otherwise
 *  Time Complexity: O(1)
*******************************************************************************/
int DlistIterIsEqual(dlist_iter_ty iter1, dlist_iter_ty iter2);

/******************************************************************************* 
 *  returns data stored in "iter"
 *  note: undefined behaviour if "iter" is DlistIterEnd()
 *  Time Complexity: O(1)
*******************************************************************************/
void *DlistIterGetData(dlist_iter_ty iter);

/******************************************************************************* 
 *  inserts "data" to the back of the list
 *  returns an iterator to the added data if succeeded, DlistIterEnd() otherwise
 *  Time Complexity: determined by the used system call complexity
*******************************************************************************/
dlist_iter_ty DlistPushBack(dlist_ty *dlist, const void *data);

/******************************************************************************* 
 *  removes the last element of "dlist" and returns the data that was stored in 
 *  the removed element
 *  note: undefined behaviour if "dlist" is empty or NULL
 *  Time Complexity: determined by the used system call complexity
*******************************************************************************/
void *DlistPopBack(dlist_ty *dlist);

/******************************************************************************* 
 *  inserts "data" to the front of the list
 *  returns an iterator to the added data if succeeded, DlistIterEnd() otherwise
 *  note: may cause IterBegin invalidity
 *  Time Complexity: determined by the used system call complexity
*******************************************************************************/
dlist_iter_ty DlistPushFront(dlist_ty *dlist, const void *data);

/******************************************************************************* 
 *  removes the first element of "dlist" and returns the data that was stored in 
 *  the removed element
 *  note: undefined behaviour if "dlist" is empty or NULL
 *  Time Complexity: determined by the used system call complexity
*******************************************************************************/
void *DlistPopFront(dlist_ty *dlist);

/******************************************************************************* 
 *  inserts "data" before a given iter "where"
 *  returns an iterator to the added data if succeeded, DlistIterEnd() otherwise
 *  note: may cause iterator invalidity
 *  Time Complexity: avg - O(1), worst - O(n)
*******************************************************************************/
dlist_iter_ty DlistInsert(dlist_iter_ty where, const void *data);

/******************************************************************************* 
 *  removes the element at "iter_to_remove" and returns iterator to the next 
 *  element in the dlist
 *  note: undefined behaviour if "iter_to_remove" is DlistIterEnd()
 *  Time Complexity: O(1)
*******************************************************************************/
dlist_iter_ty DlistRemove(dlist_iter_ty iter_to_remove);

/******************************************************************************* 
 *  performs "match_func" on each element in the range ["from", "to") till 
 *  success
 *  if found, returns the iterator to that element, return "to" iter otherwise
 *  note: undefined behaviour if "from" is DlistIterEnd() or if "from" and 
 *        "to" are in different dlists
          "from" must be before "to" in the list
 *  Time Complexity: O(n)
*******************************************************************************/
dlist_iter_ty DlistFind(dlist_iter_ty from, dlist_iter_ty to, 
                        is_match_func_ty match_func, void *param);

/******************************************************************************* 
 *  performs "match_func" on each element in the range ["from", "to") and 
 *  stores all matching data references, if any, in "dest_dlist",
 *  returns 0 if succeeded, not 0 otherwise
 *  note: undefined behaviour if "from" is DlistIterEnd() or if "from" and 
 *        "to" are in different dlists
          "from" must be before "to" in the list
 *  Time Complexity: O(n)
*******************************************************************************/
int DlistMultiFind(dlist_iter_ty from, dlist_iter_ty to, 
                    is_match_func_ty match_func, void *param, 
                    dlist_ty *dest_dlist);

/******************************************************************************* 
 *  performs "action" on each element in the range ["from", "to") till success
 *  returns 0 if succeeded, not 0 otherwise
 *  note: undefined behaviour if "from" is DlistIterEnd() or if "from" and 
 *        "to" are in different dlists
          "from" must be before "to" in the list
 *  Time Complexity: O(n)
*******************************************************************************/
int DlistForEach(dlist_iter_ty from, dlist_iter_ty to, action_func_ty action, void *param);

/*******************************************************************************
 *  moves the elements in range [from, to) in a src_dlist to a dest_dlist before
 *  the iterator "where"
 *  returns an iterator to the last spliced element
 *  note: undefined behaviour if "from" is DlistIterEnd() or if "from" and 
 *        "to" are in different dlists
          "from" must be before "to" in the list
 *  Time Complexity: O(1)
*******************************************************************************/
dlist_iter_ty DlistSplice(dlist_iter_ty where, dlist_iter_ty from, dlist_iter_ty to);

#endif  /*  __DLIST_H__  */
