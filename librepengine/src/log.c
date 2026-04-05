#include "log.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static int   g_log_level  = LOG_LEVEL_INFO;
static FILE *g_log_output = NULL;

void         log_init(void)
{
    const char *env_level = getenv("RE_LOG_LEVEL");
    if (env_level) {
        int level = atoi(env_level);
        if (level >= LOG_LEVEL_OFF && level <= LOG_LEVEL_DEBUG) {
            g_log_level = level;
        }
    }
    if (g_log_output == NULL) {
        g_log_output = stderr;
    }
}

void               log_set_level(int level) { g_log_level = level; }

int                log_get_level(void) { return g_log_level; }

void               log_set_output(FILE *stream) { g_log_output = stream; }

static const char *level_to_string(int level)
{
    switch (level) {
    case LOG_LEVEL_DEBUG:
        return "DEBUG";
    case LOG_LEVEL_INFO:
        return "INFO";
    case LOG_LEVEL_WARN:
        return "WARN";
    case LOG_LEVEL_ERROR:
        return "ERROR";
    default:
        return "UNKNOWN";
    }
}

void log_log(int level, const char *file, int line, const char *fmt, ...)
{
    if (level < g_log_level) {
        return;
    }

    time_t     now     = time(NULL);
    struct tm *tm_info = localtime(&now);
    char       time_buf[9];
    strftime(time_buf, sizeof(time_buf), "%H:%M:%S", tm_info);

    const char *filename = strrchr(file, '/');
    filename             = filename ? filename + 1 : file;

    fprintf(g_log_output, "[%s] [%s] %s:%d: ", time_buf, level_to_string(level),
            filename, line);

    va_list args;
    va_start(args, fmt);
    vfprintf(g_log_output, fmt, args);
    va_end(args);

    fprintf(g_log_output, "\n");
    fflush(g_log_output);
}
