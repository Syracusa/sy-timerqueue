#include <stdio.h>
#include <time.h>

#include "timerqueue.h"

#define LOG_TIME(...)                                   \
    do                                                  \
    {                                                   \
        struct timespec _ts;                            \
        clock_gettime(CLOCK_REALTIME, &_ts);            \
        printf("[%4ld:%6ld] ",                          \
               _ts.tv_sec % 10000, _ts.tv_nsec / 1000); \
        printf(__VA_ARGS__);                            \
    } while (0)

static void print_log_cb(void *arg)
{
    char *msg = arg;
    LOG_TIME("%s\n", msg);
}

static void unregister_job(void * arg)
{
    fprintf(stderr, "Unregister job...\n");
    TqElem* elem = arg;
    elem->active = 0;
}

static void register_jobs(TqCtx* tq)
{
    static TqElem e1, e2, e3;

    e1.callback = print_log_cb;
    e1.interval_us = 1000000;
    e1.use_once = 0;
    e1.arg = "1s Interval log";

    register_timerqueue_job(tq, &e1);

    e2.callback = print_log_cb;
    e2.interval_us = 300000;
    e2.use_once = 0;
    e2.arg = "300ms Interval log";

    register_timerqueue_job(tq, &e2);


    e3.callback = unregister_job;
    e3.interval_us = 3000000;
    e3.use_once = 1;
    e3.arg = &e2;

    register_timerqueue_job(tq, &e3);
}

int main()
{
    TqCtx* tq = create_timerqueue();
    // tq->rbt->cmp(NULL, NULL);
    register_jobs(tq);

    while (1)
    {
        timerqueue_work(tq);
    }
}
