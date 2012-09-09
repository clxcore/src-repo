#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include "log.h"

#define LOG_CONF

/* config file */
#ifdef LOG_CONF

#define CONF_MAX_LINE 4096

const char *log_options[] = {
    "file"  , NULL,
    "level" , NULL,
    "time"  , NULL,
    "prefix", NULL,
    "trace" , NULL,
     NULL   , NULL
};

char *sdup(const char *src)
{
    char *p = NULL;
    if (src && (p = malloc(strlen(src) + 1)))
        strcpy(p, src);
    return p;
}

const char *get_option(const char *name)
{
    const char **p = log_options;

    while (*p) {
        if (strcmp(*p, name) == 0)
            break;
        p += 2;
    }
    return *(p + 1);
}

const char *set_option(const char *name, const char *value)
{
    const char **p = log_options;

    while (*p) {
        if (strcmp(*p, name) == 0) {
            *(p + 1) = sdup(value);
            break;
        }
        p += 2;
    }
    return *p;
}

int log_conf_parse(const char *file, const char **options)
{
    char line[CONF_MAX_LINE], opt[sizeof(line)], val[sizeof(line)];
    size_t line_no = 0, opt_no = 0;
    FILE *fp;

    fp = fopen(file, "rb");
    if (fp == NULL)
        return -1;
    while (fgets(line, sizeof(line), fp) != NULL) {
        line_no++;
        if (line[0] == '#' || line[0] == '\n' ||
            (line[0] == '\r' && line[1] == '\n'))
            continue;
        if (sscanf(line, "%s %[^\r\n#]", opt, val) != 2) {
            printf("%s:%ld: %s\n",file, line_no,
                   (line[strlen(line)-1] = '\0', line));
            break;
        }
        ++opt_no;
        if (set_option(opt, val) == NULL) {
            opt_no = -1;
            break;
        }
        /* printf("%4ld: opt[%s], val[%s]\n", opt_no, opt, val); */
    }
    fclose(fp);
    return opt_no;
}

#endif

static void log_default_options(log_t *p)
{
    if (p) {
        p->flag = LOG_FLAG_OUTPUT | LOG_FLAG_TIME;
        p->level = LOG_LEVEL_ALL;
#ifdef LOG_CONF
#endif
    }
}

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
    log_default_options(p);
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
