#include <stdio.h>
#include <time.h>
#include <stdlib.h> 
#include <string.h>

#include "rbtree.h"

#include "timerqueue.h"

static int compare_elem(const void *k1, const void *k2)
{
    TqKey *n1 = (TqKey *)k1;
    TqKey *n2 = (TqKey *)k2;

    struct timespec diff;
    if (n1->expire.tv_nsec == n2->expire.tv_nsec &&
        n1->expire.tv_sec == n2->expire.tv_sec)
    {
        if (n1->ptr > n2->ptr)
        {
            return 1;
        }
        else if (n1->ptr < n2->ptr)
        {
            return -1;
        }
        else
        {
            return 0;
        }
    }
    else if (check_expire(&n1->expire, &n2->expire))
    {
        return -1;
    }
    else
    {
        return 1;
    }
}

void register_timerqueue_job(TqCtx *tq, TqElem *elem)
{
    clock_gettime(CLOCK_REALTIME, &elem->rbk.expire);
    timespec_add_usec(&elem->rbk.expire, elem->interval_us);

    elem->rbk.ptr = elem;
    elem->rbn.key = &elem->rbk;
    elem->attached = 1;
    elem->active = 1;
    rbtree_insert(tq->rbt, (rbnode_type *)elem);
}

void timerqueue_work(TqCtx *tq)
{
    struct timespec currtime;
    clock_gettime(CLOCK_REALTIME, &currtime);

    TqElem *first = (TqElem *)rbtree_first(tq->rbt);

    while (check_expire(&(first->rbk.expire), &currtime))
    {
        rbtree_delete(tq->rbt, first->rbn.key);
        if (first->active)
        {
            first->callback(first->arg);
            if (first->use_once)
            {
                first->active = 0;
                first->attached = 0;
            }
            else
            {
                timespec_add_usec(&first->rbk.expire, first->interval_us);
                rbtree_insert(tq->rbt, (rbnode_type *)first);
            }
        }
        else
        {
            first->attached = 0;
        }

        first = (TqElem *)rbtree_first(tq->rbt);
    }
}

TqCtx *create_timerqueue()
{
    TqCtx *tq = malloc(sizeof(TqCtx));
    tq->rbt = rbtree_create(compare_elem);
    return tq;
}

void delete_timerqueue(TqCtx *tq)
{
    /*  TODO  */
}