#include <stdio.h>
#include <stdlib.h>
#include "time_wheel.h"

CircularLL job_list = {&job_list, &job_list};
int job_num;

static void free_interval_job()
{
    CircularLL *ptr;
    cll_foreach(ptr, &job_list)
    {
        IntervalJob *ij = (IntervalJob *)ptr;
        if (ij->delete_flag == 1)
        {
            ptr = ptr->prev;
            int res = cll_delete(&job_list, &(ij->cll));
            if (res <= 0)
            {
                fprintf(stderr,
                        "No element matched or empty list res: %d\n",
                        res);
            }
            else
            {
                fprintf(stderr,
                        "Unregister job %s\n",
                        ij->name);
                free(ij);
                ij = NULL;
            }
        }
    }
}

void unregister_timerwheel_job_all()
{
    // int res = 0;
    CircularLL *ptr;
    cll_foreach(ptr, &job_list)
    {
        IntervalJob *ij = (IntervalJob *)ptr;
        ij->delete_flag = 1;
    }

    free_interval_job();
}

void unregister_timerwheel_job(IntervalJob *ij)
{
    if (ij != NULL)
    {
        ij->delete_flag = 1;
    }
    else
    {
        fprintf(stderr, "NULL on %s\n", __FUNCTION__);
    }
}

IntervalJob *register_timerwheel_job(void (*cb)(),
                                   int interval_us,
                                   int use_once,
                                   void *arg,
                                   char *name)
{
    IntervalJob *ij = malloc(sizeof(IntervalJob));

    if (cb == NULL)
    {
        fprintf(stderr,
                "Callback is NULL! Jobname : %s\n", name);
        return NULL;
    }

    ij->interval_callback = cb;
    ij->interval_us = interval_us;
    ij->use_once = use_once;
    ij->delete_flag = 0;
    ij->arg = arg;
    sprintf(ij->name, "%s", name);

    clock_gettime(CLOCK_REALTIME, &ij->next_event_time);
    timespec_add_usec(&ij->next_event_time, interval_us);
    cll_add_tail(&job_list, &ij->cll);

    return ij;
}

/* return number of processed jobs  */
int timewheel_work()
{
    int timeouted = 0;
    struct timespec currtime;
    clock_gettime(CLOCK_REALTIME, &currtime);

    CircularLL *ptr;
    cll_foreach(ptr, &job_list)
    {
        IntervalJob *ij = (IntervalJob *)ptr;
        if (is_time_expired(&(ij->next_event_time), &currtime))
        {
            ij->interval_callback(ij->arg);

            if (ij->use_once == 1)
            {
                unregister_timerwheel_job(ij);
            }
            else
            {
#if 0 /* Set to 1 If you wanna use sleep() */
                memcpy(&ij->next_event_time, 
                    &currtime, 
                    sizeof(struct timespec));
#endif
                timespec_add_usec(&ij->next_event_time, ij->interval_us);
            }
            timeouted++;
        }
    }
    free_interval_job();

    return timeouted;
}