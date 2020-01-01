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
static int PIHELPER_OK = 0;
static int PIHELPER_HELP = 1;
static int PIHELPER_INVALID_COMMANDS = 2;

static int PIHELPER_LOG_DISABLED = -1;
static int PIHELPER_LOG_ERROR    =  0;
static int PIHELPER_LOG_WARN     =  1;
static int PIHELPER_LOG_INFO     =  2;
static int PIHELPER_LOG_DEBUG    =  3;


typedef struct {
    char * host;
    char * api_key;
} pihole_config;

void pihelper_set_log_level(int level);

int pihelper_get_status(pihole_config * config);

int pihelper_enable_pihole(pihole_config * config);

int pihelper_disable_pihole(pihole_config * config, char * duration);

pihole_config * pihelper_new_config();

void pihelper_config_set_host(pihole_config * config, char * host);

void pihelper_config_set_password(pihole_config * config, char * password);

void pihelper_config_set_api_key(pihole_config * config, char * api_key);

pihole_config * pihelper_read_config(char * config_path);

int pihelper_save_config(pihole_config * config, char * config_path);

void pihelper_free_config(pihole_config * config);
#endif

