#include <stdio.h>
#include <time.h>

#include "time_wheel.h"

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

static void register_jobs()
{
    register_timewheel_job(print_log_cb, 1000000, 0,
                           "1s Interval log", "1sLogJob");
    register_timewheel_job(print_log_cb, 300000, 0,
                           "300ms Interval log", "300msLogJob");
}

int main()
{
    register_jobs();
    while (1)
    {
        timewheel_work();
    }
}
