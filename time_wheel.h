#ifndef TIME_WHEEL_H
#define TIME_WHEEL_H

#include <time.h>
#include "circular_ll.h"

#define MAX_INTERVAL_JOB_NAMELEN 100

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

typedef struct
{
    CircularLL cll;
    int interval_us;
    int use_once;
    int delete_flag;
    struct timespec next_event_time;
    void *arg;
    void (*interval_callback)(void *arg);
    char name[MAX_INTERVAL_JOB_NAMELEN];
} IntervalJob;

void unregister_timewheel_job_all();

#define unregister_timewheel_job_safe(ij) \
    do                                   \
    {                                    \
        unregister_timewheel_job(ij);     \
        ij = NULL;                       \
    } while (0);

void unregister_timewheel_job(IntervalJob *ij);

IntervalJob *register_timewheel_job(void (*cb)(),
                                   int interval_us,
                                   int use_once,
                                   void *arg,
                                   char *name);

int timewheel_work();

#endif