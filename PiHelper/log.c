/**
 * Copyright © 2019, 2020 William Brawner.
 *
 * This file is part of PiHelper.
 *
 * PiHelper is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PiHelper is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PiHelper.  If not, see <https://www.gnu.org/licenses/>.
 */
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

