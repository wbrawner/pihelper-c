#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "log.h"

int LOG_LEVEL = 2; // Default to info logs

void set_log_level(int level) {
    LOG_LEVEL = level;
}

void write_log(int level, char * format, ...) {
    if (level > LOG_LEVEL) return;
    va_list args;
    va_start(args, format);
    int format_len = strlen(format);
    char * new_format = malloc(format_len + 1);
    memcpy(new_format, format, format_len);
    new_format[format_len] = '\n';
    new_format[format_len + 1] = '\0';
    FILE *stream = level < PIHELPER_LOG_INFO ? stderr : stdout;
    vfprintf(stream, new_format, args);
    va_end(args);
}

