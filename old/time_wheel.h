#ifndef TIME_WHEEL_H
#define TIME_WHEEL_H

#include <time.h>

/*=== Circular Linked List Implementation ===*/

typedef struct _CircularLL
{
    struct _CircularLL *prev;
    struct _CircularLL *next;
} CircularLL;

#define cll_foreach(trav_elem, head) \
    for ((trav_elem) = (head)->next; \
         (trav_elem) != (head);      \
         (trav_elem) = (trav_elem)->next)

#define cll_no_entry(head) \
    ((head)->next == head)

#define cll_one_entry(head) \
    ((head)->next->next == head)

int cll_add_tail(CircularLL *head, CircularLL *elem);
int cll_delete(CircularLL *head, CircularLL *delete_elem);
int cll_init_head(CircularLL *head);

/*===========================================*/

#define MAX_TWJOB_NAMELEN 100

/* Timewheel Job Structure */
typedef struct
{
    CircularLL cll;
    int interval_us;
    int use_once;
    int delete_flag;
    struct timespec next_event_time;
    void *arg;
    void (*callback)(void *arg);
    char name[MAX_TWJOB_NAMELEN];
} TwJob;

/* Check time and excute registered jobs */
int timewheel_work();

/**
 * @brief Register job in timewheel
 * 
 * @param cb - Function to execute later
 * @param interval_us  - Execute interval
 * @param use_once - if 1 then Job will unregistered after 1 execute
 * @param arg - Argument to callback function
 * @param name - Job name for debug purpose
 * @return TwJob* 
 */
TwJob *register_timewheel_job(void (*cb)(),
                              int interval_us,
                              int use_once,
                              void *arg,
                              char *name);

/* Unregister job in timewheel */
void unregister_timewheel_job(TwJob *ij);

/* Unregister all job */
void unregister_timewheel_job_all();

/* Unregister job and set ptr to NULL */
#define unregister_timewheel_job_safe(ij) \
    do                                    \
    {                                     \
        unregister_timewheel_job(ij);     \
        ij = NULL;                        \
    } while (0);

/* Timespec Related Helper Function */
static inline int timespec_add_usec(struct timespec *t,
                                    unsigned long usec)
{
    unsigned long long add;
    int res = 0;

    if (!t)
    {
        res = -1;
    }
    else
    {
        add = t->tv_nsec + (usec * 1000);
        t->tv_sec += add / 1000000000;
        t->tv_nsec = add % 1000000000;
    }

    return res;
}

static inline void timespec_diff(struct timespec *a,
                                 struct timespec *b,
                                 struct timespec *result)
{
    result->tv_sec = a->tv_sec - b->tv_sec;
    result->tv_nsec = a->tv_nsec - b->tv_nsec;
    if (result->tv_nsec < 0)
    {
        --result->tv_sec;
        result->tv_nsec += 1000000000L;
    }
}

static inline int is_time_expired(struct timespec *sometime,
                                  struct timespec *currtime)
{
    int res = 0;

    if (sometime->tv_sec == currtime->tv_sec)
    {
        res = sometime->tv_nsec <= currtime->tv_nsec ? 1 : 0;
    }
    else
    {
        res = sometime->tv_sec < currtime->tv_sec ? 1 : 0;
    }
    return res;
}

#endif