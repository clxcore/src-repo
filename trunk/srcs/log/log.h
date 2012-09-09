#ifndef LOG_H__
#define LOG_H__

#include <stdio.h>

#ifndef LOG_ALL_OFF

#define LOG(p, l, f, ...) \
    if (l <= p->level) log_write(__func__, __LINE__, fmt, __VA_ARGS__)

#define LOG_FATAL(p, f, ...) \
    if (LOG_LEVEL_FATAL <= p->level) \
        log_write(p, __func__, __LINE__, f, __VA_ARGS__)

#define LOG_ERROR(p, f, ...) \
    if (LOG_LEVEL_ERROR <= p->level) \
        log_write(p, __func__, __LINE__, f, __VA_ARGS__)

#define LOG_WARN(p, f, ...) \
    if (LOG_LEVEL_WARN  <= p->level) \
        log_write(p, __func__, __LINE__, f, __VA_ARGS__)

#define LOG_INFO(p, f, ...) \
    if (LOG_LEVEL_INFO  <= p->level) \
        log_write(p, __func__, __LINE__, f, __VA_ARGS__)

#define LOG_DEBUG(p, f, ...) \
    if (LOG_LEVEL_DEBUG <= p->level) \
        log_write(p, __func__, __LINE__, f, __VA_ARGS__)

#define LOG_TRACE(p, f, ...) \
    if (LOG_LEVEL_TRACE <= p->level) \
        log_write(p, __func__, __LINE__, f, __VA_ARGS__)

#else

#define LOG(      p, l, ...) ((void) 0)
#define LOG_FATAL(p, l, ...) ((void) 0)
#define LOG_ERROR(p, l, ...) ((void) 0)
#define LOG_WARN( p, l, ...) ((void) 0)
#define LOG_INFO( p, l, ...) ((void) 0)
#define LOG_DEBUG(p, l, ...) ((void) 0)
#define LOG_TRACE(p, l, ...) ((void) 0)

#endif

typedef enum log_level_t_ {
    LOG_LEVEL_OFF   = 0x00,
    LOG_LEVEL_FATAL = 0x10,
    LOG_LEVEL_ERROR = 0x20,
    LOG_LEVEL_WARN  = 0x30,
    LOG_LEVEL_INFO  = 0x40,
    LOG_LEVEL_DEBUG = 0x50,
    LOG_LEVEL_TRACE = 0x60,
    LOG_LEVEL_ALL   = 0xff
} log_level_t;

typedef enum log_flag_t_ {
    LOG_FLAG_OUTPUT = 0x01,
    LOG_FLAG_TIME   = 0x02,
    LOG_FLAG_TRACE  = 0x04,
    LOG_FLAG_PREFIX = 0x08
} log_flag_t;

typedef struct log_t_ {
    log_flag_t  flag;
    log_level_t level;
    char *prefix;
    FILE *fp;
} log_t;

log_t *log_open(const char *filename);
void log_close(log_t *p);
void log_write(log_t *p, const char *func, unsigned int line, const char *fmt, ...);

#endif
