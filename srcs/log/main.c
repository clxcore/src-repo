#include <stdio.h>
#include <time.h>
#include "log.h"


int main(int argc, char **argv)
{
    log_t *log = log_open("log");
    log->flag |= LOG_FLAG_PREFIX;
    log->level = LOG_LEVEL_WARN;
    log->prefix = "====  ";

    LOG_DEBUG(log, "1, time[%ld]", time(NULL));
    LOG_TRACE(log, "2, time[%ld]", time(NULL));
    log->flag |= LOG_FLAG_TRACE;
    LOG_INFO(log, "3, time[%ld]", time(NULL));
    log->flag &= ~LOG_FLAG_TIME;
    LOG_ERROR(log, "4, time[%ld]", time(NULL));
    LOG_WARN(log, "5, time[%ld]", time(NULL));
    log->flag |= LOG_FLAG_TIME;
    LOG_FATAL(log, "6, time[%ld]", time(NULL));
    LOG_FATAL(log, "=========");

    log_close(log);
    return 0;
}
