/*******************************************************************
*Date: 24.12.22
*Author: Aviv Jilin
*reviewer: Lital 
*version: 1.0
*****************************************************************/


#include <stdlib.h> /* malloc, free */ 
#include <assert.h> /* assert       */ 
#include <stddef.h> /* size_t       */

#include "dlist.h"

typedef struct dlist_node
{
    void *data;
    struct dlist_node *next;
    struct dlist_node *prev;
} dlist_node_ty;

struct dlist
{
    dlist_node_ty *head;
    dlist_node_ty *tail;
};

static dlist_node_ty *GetNode(dlist_iter_ty iter_to_node)
{
    return (dlist_node_ty *)iter_to_node;
}

static dlist_iter_ty GetIter(dlist_node_ty *node_to_iter)
{
    return (dlist_iter_ty)node_to_iter;
}

static int IsDummyEnd(dlist_iter_ty iter)
{
    return (NULL == GetNode(iter)->next);
}

static int IsDummyBegin(dlist_iter_ty iter)
{
    return (NULL == GetNode(iter)->prev);
}


static dlist_iter_ty FindEndDummy(dlist_iter_ty iter)
{
    while (!IsDummyEnd(iter))
    {
        iter = DlistIterNext(iter);
    }
    
    return iter;
}

static dlist_node_ty *Insert(dlist_iter_ty where, const void *data)
{
    dlist_node_ty *where_node = NULL;
    dlist_node_ty *prev_node = NULL;
    dlist_node_ty *new = NULL;
    
    assert(!IsDummyBegin(where));
        
    new = (dlist_node_ty *)malloc(sizeof(dlist_node_ty));
    
    if (NULL == new)
    {
        return NULL;
    }
    
    where_node = GetNode(where);
    prev_node = GetNode(DlistIterPrev(where));
    
    new->next = where_node;
    new->prev = prev_node;
    new->data = (void *)data;
    
    prev_node->next = new;
    where_node->prev = new;
    
    return new;
}

dlist_ty *DlistCreate(void)
{
    dlist_node_ty *dummy_end = NULL;
    dlist_node_ty *dummy_begin = NULL;
    dlist_ty *dlist = (dlist_ty *)malloc(sizeof(dlist_ty));

    if (NULL == dlist)
    {
        return NULL;
    }

    dummy_end = (dlist_node_ty *)malloc(sizeof(dlist_node_ty));

    if (NULL == dummy_end)
    {
        free(dlist);
        dlist = NULL;
        return NULL;
    }

    dummy_begin = (dlist_node_ty *)malloc(sizeof(dlist_node_ty));

    if (NULL == dummy_begin)
    {
        free(dlist);
        dlist = NULL;

        free(dummy_end);
        dummy_end = NULL;

        return NULL;
    }

    dlist->tail = dummy_end;
    dlist->head = dummy_begin;

    dummy_begin->data = dlist;
    dummy_begin->next = dummy_end;
    dummy_begin->prev = NULL;
    
    dummy_end->data = dlist;
    dummy_end->next = NULL;
    dummy_end->prev = dummy_begin;

    return dlist;
}

/*******************************************************************************
 *  frees all dynamically allocated resources used by "dlist"
 *  note: undefined behaviour if "dlist" is NULL
 *  Time Complexity: determined by the used system call complexity
*******************************************************************************/
void DlistDestroy(dlist_ty *dlist)
{
    dlist_node_ty *curr_node = NULL;
    dlist_iter_ty curr_iter = NULL;  
    
    assert(NULL != dlist);

    curr_iter = GetIter(dlist->head);

    while(!IsDummyEnd(curr_iter))
    {
        curr_node = GetNode(curr_iter);
        curr_iter = DlistIterNext(curr_iter);
        free(curr_node);
    }

    curr_node = GetNode(curr_iter);
    free(curr_node);
    dlist->head = NULL;
    dlist->tail = NULL;
    free(dlist);
    dlist = NULL;
}

/******************************************************************************* 
 *  returns the number of elements in "dlist"
 *  note: undefined behaviour if "dlist" is NULL
 *  Time Complexity: O(n)
*******************************************************************************/
size_t DlistSize(const dlist_ty *dlist)
{
    dlist_iter_ty curr_iter = NULL;
    size_t count = 0;

    assert(NULL != dlist);

    curr_iter = DlistIterBegin(dlist);

    while (!IsDummyEnd(curr_iter)) 
    {
        curr_iter = DlistIterNext(curr_iter); 
        ++count;
    }

    return count;
}


/******************************************************************************* 
 *  returns 1 if "dlist" is empty, 0 otherwise
 *  note: undefined behaviour if "dlist" is NULL
 *  Time Complexity: O(1)
*******************************************************************************/
int DlistIsEmpty(const dlist_ty *dlist)
{
    assert(NULL != dlist);

    if (DlistIterIsEqual(DlistIterBegin(dlist), DlistIterEnd(dlist)))
    {
        return 1;
    }

    return 0;
}

/******************************************************************************* 
 *  returns an iterator to the first valid element in "dlist", DlistIterEnd() 
 *  if empty.
 *  note: undefined behaviour if "dlist" is NULL
 *  Time Complexity: O(1)
*******************************************************************************/
dlist_iter_ty DlistIterBegin(const dlist_ty *dlist)
{
    assert(NULL != dlist);

    return GetIter(dlist->head->next);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
}

/******************************************************************************* 
 *  returns an iterator to the end of "dlist", this is an invalid element
 *  use DlistIterPrev(DlistIterEnd(dlist)) to get last valid element in list
 *  note: undefined behaviour if "dlist" is NULL
 *  Time Complexity: O(1)
*******************************************************************************/
dlist_iter_ty DlistIterEnd(const dlist_ty *dlist)
{
    assert(NULL != dlist);

    return GetIter(dlist->tail);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
}

/******************************************************************************* 
 *  returns an iterator to the next element in dlist
 *  note: undefined behaviour if "iter" is DlistIterEnd()
 *  Time Complexity: O(1)
*******************************************************************************/
dlist_iter_ty DlistIterNext(dlist_iter_ty iter)
{
    assert(!IsDummyEnd(iter)); 

    return GetIter(GetNode(iter)->next);
}

/******************************************************************************* 
 *  returns an iterator to the previous element in dlist
 *  note: undefined behaviour if "iter" is DlistIterBegin()
 *  Time Complexity: O(1)
*******************************************************************************/
dlist_iter_ty DlistIterPrev(dlist_iter_ty iter)
{
    assert(!IsDummyBegin(iter)); 

    return GetIter(GetNode(iter)->prev);
}

/******************************************************************************* 
 *  returns 1 if two iterators are pointing to the same element in a dlist,
 *  0 otherwise
 *  Time Complexity: O(1)
*******************************************************************************/
int DlistIterIsEqual(dlist_iter_ty iter1, dlist_iter_ty iter2)
{
    return (GetNode(iter1) == GetNode(iter2));
}

/******************************************************************************* 
 *  returns data stored in "iter"
 *  note: undefined behaviour if "iter" is DlistIterEnd()
 *  Time Complexity: O(1)
*******************************************************************************/
void *DlistIterGetData(dlist_iter_ty iter)
{
    assert(!IsDummyEnd(iter));  
    assert(!IsDummyBegin(iter)); 

    return GetNode(iter)->data;
}


/******************************************************************************* 
 *  inserts "data" to the back of the list
 *  returns an iterator to the added data if succeeded, DlistIterEnd() otherwise
 *  Time Complexity: O(1)
*******************************************************************************/
dlist_iter_ty DlistPushBack(dlist_ty *dlist, const void *data)
{
    dlist_node_ty *new_node = NULL;
    dlist_iter_ty dummy_iter;

    assert(NULL != dlist);

    dummy_iter = DlistIterEnd(dlist);

    new_node = Insert(dummy_iter, data);

    if (NULL != new_node)
    {
        return GetIter(new_node);
    }
    else
    {
        return dummy_iter;
    }

}

/******************************************************************************* 
 *  removes the last element of "dlist" and returns iterator to the next 
 *  element in the dlist
 *  note: undefined behaviour if "dlist" is empty or NULL
 *  Time Complexity: O(1)
*******************************************************************************/
void *DlistPopBack(dlist_ty *dlist)
{
    dlist_iter_ty curr_iter; 
    void *data_removed = NULL; 

    assert(NULL != dlist);

    curr_iter = DlistIterPrev(DlistIterEnd(dlist));
    data_removed = DlistIterGetData(curr_iter);

    DlistRemove(curr_iter); 

    return data_removed; 
}

/******************************************************************************* 
 *  inserts "data" to the front of the list
 *  returns an iterator to the added data if succeeded, DlistIterEnd() otherwise
 *  note: may cause IterBegin invalidity
 *  Time Complexity: O(1)
*******************************************************************************/
dlist_iter_ty DlistPushFront(dlist_ty *dlist, const void *data)
{
    dlist_node_ty *new_node = NULL;

    assert(NULL != dlist);

    new_node = Insert(DlistIterBegin(dlist), data);

    if (NULL != new_node)
    {
        return GetIter(new_node);
    }
    else
    {
        return DlistIterEnd(dlist);
    }
}


/******************************************************************************* 
 *  removes the first element of "dlist" and returns iterator to the next 
 *  element in the dlist
 *  note: undefined behaviour if "dlist" is empty or NULL
 *  Time Complexity: O(1)
*******************************************************************************/
void *DlistPopFront(dlist_ty *dlist)
{
    dlist_iter_ty curr_iter; 
    void *data_removed = NULL; 
    
    assert(NULL != dlist);

    curr_iter = DlistIterBegin(dlist);
    data_removed = DlistIterGetData(curr_iter);

    DlistRemove(curr_iter); 

    return data_removed; 
}

/******************************************************************************* 
 *  inserts "data" before a given iter "where"
 *  returns an iterator to the added data if succeeded, DlistIterEnd() otherwise
 *  note: may cause iterator invalidity
 *  Time Complexity: avg - O(1), worst - O(n)
*******************************************************************************/
dlist_iter_ty DlistInsert(dlist_iter_ty where, const void *data)
{
    dlist_node_ty *new_node = NULL;

    assert(!IsDummyBegin(where));
    
    new_node = Insert(where, data); 

    if (NULL == new_node)
    {   
        return FindEndDummy(where);
    }

    return GetIter(new_node);
}


/******************************************************************************* 
 *  removes the element at "iter_to_remove" and returns iterator to the next 
 *  element in the dlist
 *  note: undefined behaviour if "iter_to_remove" is DlistIterEnd()
 *  Time Complexity: O(1)
*******************************************************************************/
dlist_iter_ty DlistRemove(dlist_iter_ty iter_to_remove)
{
    dlist_node_ty *remove = NULL;
    dlist_node_ty *prev_to_remove = NULL;
    dlist_node_ty *next_to_remove = NULL;

    assert(!IsDummyEnd(iter_to_remove)); 
    assert(!IsDummyBegin(iter_to_remove)); 
    
    remove = GetNode(iter_to_remove);
    prev_to_remove = GetNode(DlistIterPrev(iter_to_remove));
    next_to_remove = GetNode(DlistIterNext(iter_to_remove));
    
    prev_to_remove->next = remove->next;
    next_to_remove->prev = remove->prev;
        
    free(remove);
    remove = NULL;
    
    return GetIter(next_to_remove);
}



/******************************************************************************* 
 *  performs "match_func" on each element in the range ["from", "to") till 
 *  success
 *  if found, returns the iterator to that element, return "to" iter otherwise
 *  note: undefined behaviour if "from" is DlistIterEnd() or if "from" and 
 *        "to" are in different dlists
          "from" must be order before "to" in the list
 *  Time Complexity: O(n)
*******************************************************************************/
dlist_iter_ty DlistFind(dlist_iter_ty from, dlist_iter_ty to, 
                        is_match_func_ty match_func, void *param)
{
    
    assert(!IsDummyEnd(from));   
    assert(NULL != match_func);

    while (!DlistIterIsEqual(from, to)) 
    {
        if (match_func(DlistIterGetData(from), param))
        {
            return from;
        }

        from = DlistIterNext(from); 
    }
    return to;
}

/******************************************************************************* 
 *  performs "match_func" on each element in the range ["from", "to") and 
 *  stores all matching data references, if any, in "dest_dlist",
 *  returns 0 if succeeded, not 0 otherwise
 *  note: undefined behaviour if "from" is DlistIterEnd() or if "from" and 
 *        "to" are in different dlists
          "from" must be order before "to" in the list
 *  Time Complexity: O(n)
*******************************************************************************/
int DlistMultiFind(dlist_iter_ty from, dlist_iter_ty to, 
                    is_match_func_ty match_func, void *param, 
                    dlist_ty *dest_dlist)
{
    dlist_iter_ty found_iter;
    dlist_iter_ty res;
    
    assert(!IsDummyEnd(from));  
    assert(NULL != match_func);
    assert(NULL != dest_dlist);

    while (!DlistIterIsEqual(from, to))
    {
        found_iter = DlistFind(from, to, match_func, param);

        if (!DlistIterIsEqual(found_iter, to))
        {
            res = DlistInsert(DlistIterEnd(dest_dlist), 
                              DlistIterGetData(found_iter)); 

            if (IsDummyEnd(res)) 
            {
                return 1;
            }
        }
        
        from = DlistIterNext(found_iter);
    }

    return 0;
}

/******************************************************************************* 
 *  performs "action" on each element in the range ["from", "to") till success
 *  returns 0 if succeeded, not 0 otherwise
 *  note: undefined behaviour if "from" is DlistIterEnd() or if "from" and 
 *        "to" are in different dlists
          "from" must be before "to" in the list
 *  Time Complexity: O(n)
*******************************************************************************/
int DlistForEach(dlist_iter_ty from, dlist_iter_ty to, action_func_ty action, void *param)
{
    assert(!IsDummyEnd(from));
    assert(NULL != action);

    while (!DlistIterIsEqual(from, to)) 
    {
        if (0 != action(DlistIterGetData(from), param)) 
        {
            return 1;
        }

        from = DlistIterNext(from);
    }

    return 0;
}        

/*******************************************************************************
 *  moves the elements in range [from, to) in a src_dlist to a dest_dlist before
 *  the iterator "where"
 *  returns an iterator to the last spliced element
 *  note: undefined behaviour if "from" is DlistIterEnd() or if "from" and 
 *        "to" are in different dlists
          "from" must be before "to" in the list
 *  Time Complexity: O(1)
*******************************************************************************/
dlist_iter_ty DlistSplice(dlist_iter_ty where, dlist_iter_ty from, dlist_iter_ty to)
{
    dlist_node_ty *where_node = NULL, *to_node = NULL, *from_node = NULL;
    dlist_node_ty *prev_where_node = NULL, *prev_from_node = NULL; 
    dlist_node_ty *prev_to_node = NULL;

    assert(!IsDummyEnd(from));
    assert(!IsDummyBegin(where));

    prev_where_node = GetNode(DlistIterPrev(where));
    prev_from_node = GetNode(DlistIterPrev(from));
    prev_to_node = GetNode(DlistIterPrev(to));

    where_node = GetNode(where);
    from_node  = GetNode(from);
    to_node    = GetNode(to);

    prev_to_node->next = where_node;
    where_node->prev = prev_to_node;

    prev_where_node->next = from_node;
    from_node->prev = prev_where_node;

    prev_from_node->next = to_node;
    to_node->prev = prev_from_node;
    
    return GetIter(prev_to_node);
}