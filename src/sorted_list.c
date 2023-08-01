/*******************************************************************
*Date: 25.12.22
*Author: Aviv Jilin
*reviewer: Almog 
*version: 1.0
*****************************************************************/

#include <stdlib.h> /* malloc, free */ 
#include <assert.h> /* assert       */ 
#include <stddef.h> /* size_t       */

#include "sorted_list.h"

struct sort_list
{
    dlist_ty *dlist;
    cmp_func_ty cmp_func;
};

static sort_list_iter_ty GetSortedIter(dlist_iter_ty dlist_iter);
static dlist_iter_ty GetDlistIter(sort_list_iter_ty sorted_iter);
static sort_list_iter_ty SortedListFirstBig(sort_list_ty *list, sort_list_iter_ty from, 
                                 sort_list_iter_ty to, const void *param);


sort_list_ty *SortedListCreate(cmp_func_ty cmp_func)
{
    sort_list_ty *new_list = NULL;

    assert(NULL != cmp_func);

    new_list = (sort_list_ty *)malloc(sizeof(sort_list_ty));

    if(NULL == new_list)
    {
        return NULL;
    }

    new_list->dlist = DlistCreate();

    if(NULL == new_list->dlist)
    {
        free(new_list);
        new_list = NULL;

        return NULL;
    }

    new_list->cmp_func = cmp_func;

    return new_list;
}


void SortedListDestroy(sort_list_ty *sort_list)
{
    assert(NULL != sort_list);

    DlistDestroy(sort_list->dlist);

    sort_list->dlist = NULL;
    
    free(sort_list); 
    
    sort_list = NULL;
}

size_t SortedListSize(const sort_list_ty *sort_list)
{
    assert(NULL != sort_list);

    return DlistSize(sort_list->dlist);
}

int SortedListIsEmpty(const sort_list_ty *sort_list)
{
    assert(NULL != sort_list);

    return DlistIsEmpty(sort_list->dlist);
}

sort_list_iter_ty SortedListBegin(const sort_list_ty *sort_list)
{
    assert(NULL != sort_list);

    return GetSortedIter(DlistIterBegin(sort_list->dlist));
}

sort_list_iter_ty SortedListEnd(const sort_list_ty *sort_list)
{
    assert(NULL != sort_list);

    return GetSortedIter(DlistIterEnd(sort_list->dlist));
}

sort_list_iter_ty SortedListNext(sort_list_iter_ty iter)
{   
    return GetSortedIter(DlistIterNext(GetDlistIter(iter)));
}

sort_list_iter_ty SortedListPrev(sort_list_iter_ty iter)
{
    return GetSortedIter(DlistIterPrev(GetDlistIter(iter)));
}

int SortedListIterIsEqual(sort_list_iter_ty iter1, sort_list_iter_ty iter2)
{
    return DlistIterIsEqual(GetDlistIter(iter1), GetDlistIter(iter2));
}

void *SortedListGetData(sort_list_iter_ty iter)
{
    return (DlistIterGetData(GetDlistIter(iter)));
}

sort_list_iter_ty SortedListInsert(sort_list_ty *sort_list, const void *data)
{
    sort_list_iter_ty start;
    sort_list_iter_ty end;
    sort_list_iter_ty found;

    assert(NULL != sort_list);

    start = SortedListBegin(sort_list);
    end = SortedListEnd(sort_list);

    found = SortedListFirstBig(sort_list, start, end, (void *)data);

    found = GetSortedIter(DlistInsert(GetDlistIter(found), data));

    return found; 
}

sort_list_iter_ty SortedListRemove(sort_list_iter_ty iter_to_remove)
{  
    return GetSortedIter(DlistRemove(GetDlistIter(iter_to_remove)));
}


void *SortedListPopFront(sort_list_ty *sort_list)
{
    assert(NULL != sort_list);

    return DlistPopFront(sort_list->dlist);
}

void *SortedListPopBack(sort_list_ty *sort_list)
{
    assert(NULL != sort_list);

    return DlistPopBack(sort_list->dlist);
}

int SortedListForeach(sort_list_iter_ty from, sort_list_iter_ty to, 
                                            action_func_ty action, void *param)
{
    assert(!SortedListIterIsEqual(from, to));
    assert(NULL != action);

    return(DlistForEach(GetDlistIter(from), GetDlistIter(to), action, param));
}

void SortedListMerge(sort_list_ty *dest, sort_list_ty *src)
{
    sort_list_iter_ty from_dest;
    sort_list_iter_ty to_dest;
    sort_list_iter_ty from_src;
    sort_list_iter_ty to_src;
    sort_list_iter_ty found_where;
    sort_list_iter_ty found_to;
    void *data = NULL;

    assert(NULL != dest);
    assert(NULL != src);
    
    to_src = SortedListEnd(src);
    to_dest = SortedListEnd(dest);
    
    while (0 == SortedListIsEmpty(src))
    {
        from_dest = SortedListBegin(dest);
        from_src = SortedListBegin(src);
        data = SortedListGetData(from_src);
                                                  
        found_where = SortedListFirstBig(dest, from_dest, to_dest, data);
        if (SortedListIterIsEqual(found_where, to_dest))
        {
            found_to = to_src;
        }
        else
        {
            found_to = SortedListFirstBig(dest, SortedListNext(from_src), to_src, 
                                  SortedListGetData(found_where));
        }
        
        DlistSplice(GetDlistIter(found_where), GetDlistIter(from_src), 
                    GetDlistIter(found_to));
    }
    
    SortedListDestroy(src);
}



sort_list_iter_ty SortedListFind(sort_list_ty *list, sort_list_iter_ty from, 
                                 sort_list_iter_ty to, const void *to_find)
{
 
    while(0 == SortedListIterIsEqual(from, to))
    {
        if(0 == list->cmp_func(SortedListGetData(from), (void *)to_find)) 
        {
            return from;
        }
        from = SortedListNext(from);
    }
    return to;
}    

sort_list_iter_ty SortedListFindIf(sort_list_iter_ty from, sort_list_iter_ty to, 
                                is_match_func_ty match_func, const void *param)
{
    return GetSortedIter(DlistFind(GetDlistIter(from), GetDlistIter(to), match_func, (void *)param));
}

static sort_list_iter_ty GetSortedIter(dlist_iter_ty dlist_iter)
{
    sort_list_iter_ty sorted_iter;

    sorted_iter.dlist_iter = dlist_iter;

    return sorted_iter;
}

static dlist_iter_ty GetDlistIter(sort_list_iter_ty sorted_iter)
{
    return sorted_iter.dlist_iter;
}

static sort_list_iter_ty SortedListFirstBig(sort_list_ty *list, sort_list_iter_ty from, 
                                 sort_list_iter_ty to, const void *param)
{
    assert(list->cmp_func);
    
    while (!SortedListIterIsEqual(from, to) && 
            list->cmp_func(SortedListGetData(from), (void *)param) < 0)
    {
        from = SortedListNext(from);
    }
    return from;
}





