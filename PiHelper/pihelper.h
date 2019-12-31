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
#ifndef PIHELPER
#define PIHELPER
#include "config.h"
#include "log.h"
#include "network.h"
static int PIHELPER_OK = 0;
static int PIHELPER_HELP = 1;
static int PIHELPER_INVALID_COMMANDS = 2;

void pihelper_set_log_level(int level);

int pihelper_get_status(pihole_config * config);

int pihelper_enable_pihole(pihole_config * config);

int pihelper_disable_pihole(pihole_config * config, char * duration);
#endif

