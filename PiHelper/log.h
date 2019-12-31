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
#ifndef PIHELPER_LOG
#define PIHELPER_LOG

static int PIHELPER_LOG_DISABLED = -1;
static int PIHELPER_LOG_ERROR    =  0;
static int PIHELPER_LOG_WARN     =  1;
static int PIHELPER_LOG_INFO     =  2;
static int PIHELPER_LOG_DEBUG    =  3;

void set_log_level(int level);

void write_log(int level, char * format, ...);
#endif
