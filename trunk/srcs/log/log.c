#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include "log.h"

log_t *log_open(const char *filename)
{
    log_t *p = NULL;

    if (filename == NULL)
        return NULL;
    p = malloc(sizeof(log_t));
    if (p == NULL)
        return NULL;
    memset(p, 0, sizeof(log_t));
    p->fp = fopen(filename, "ab");
    if (p->fp == NULL) {
        free(p);
        return NULL;
    }
    /* default options */
    p->flag = LOG_FLAG_OUTPUT | LOG_FLAG_TIME | LOG_FLAG_PREFIX;
    p->level = LOG_LEVEL_ALL;
    return p;
}

void log_close(log_t *p)
{
    if (p == NULL)
        return;
    if (p->fp) {
        fflush(p->fp);
        fclose(p->fp);
    }
    free(p);
}

void log_write(log_t *p, const char *func, unsigned int line, const char *fmt, ...)
{
    va_list ap;

    if (p->fp && (p->flag & LOG_FLAG_OUTPUT)) {
#ifdef LOG_MULTIPLE_THREADS_
        flockfile(p->fp);
#endif
        /* time */
        if (p->flag & LOG_FLAG_TIME) {
            time_t tick = time(NULL);
            struct tm *now = localtime(&tick);
            fprintf(p->fp, "[%04d-%02d-%02d %02d:%02d:%02d] ",
                    now->tm_year + 1900, now->tm_mon + 1, now->tm_mday,
                    now->tm_hour, now->tm_min, now->tm_sec);
        }
        /* function name and line number */
        if (func != NULL && line != 0 &&
            (p->flag & LOG_FLAG_TRACE))
            fprintf(p->fp, "%s:%d: ", func, line);
        /* prefix */
        if (p->prefix && (p->flag & LOG_FLAG_PREFIX)) {
            fprintf(p->fp, "%s", p->prefix);
        }
        va_start(ap, fmt);
        vfprintf(p->fp, fmt, ap);
        va_end(ap);
        fputc('\n', p->fp);
        fflush(p->fp);
#ifdef LOG_MULTIPLE_THREADS_
        funlockfile(p->fp);
#endif
    }
}
