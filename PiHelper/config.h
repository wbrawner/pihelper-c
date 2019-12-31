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
#ifndef PIHELPER_CONFIG
#define PIHELPER_CONFIG
#include <openssl/sha.h>
#include "log.h"

static char * DEFAULT_CONFIG_PATH = "/.config/pihelper.conf";

typedef struct {
    char * host;
    char * api_key;
} pihole_config;


void save_config(pihole_config * config, char * config_path);

pihole_config * read_config(char * config_path);

pihole_config * configure_pihole(char * config_path);
#endif

