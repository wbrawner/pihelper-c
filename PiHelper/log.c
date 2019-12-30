#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "log.h"

void write_log(int level, char * format, ...) {
    if (level == LOG_DEBUG) return;
    va_list args;
    va_start(args, format);
    int format_len = strlen(format);
    char * new_format = malloc(format_len + 1);
    memcpy(new_format, format, format_len);
    new_format[format_len] = '\n';
    new_format[format_len + 1] = '\0';
    vprintf(new_format, args);
    va_end(args);
}

