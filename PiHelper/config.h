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
#include "pihelper.h"

int save_config(pihole_config * config, char * config_path);

pihole_config * read_config(char * config_path);

pihole_config * pihole_config_new();

void config_set_host(pihole_config * config, char * host);

void config_set_password(pihole_config * config, char * password);

void config_set_api_key(pihole_config * config, char * api_key);

void free_config(pihole_config * config);

static void trim_string(char * raw_str);
#endif

