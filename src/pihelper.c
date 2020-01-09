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
#include "config.h"
#include "log.h"
#include "network.h"
#include "pihelper.h"

int pihelper_get_status(pihole_config * config) {
    return get_status(config);
}

int pihelper_enable_pihole(pihole_config * config) {
    return enable_pihole(config);
}

int pihelper_disable_pihole(pihole_config * config, char * duration) {
    return disable_pihole(config, duration);
}

void pihelper_set_log_level(int level) {
    set_log_level(level);
}

pihole_config * pihelper_new_config() {
    return pihole_config_new();
}

void pihelper_config_set_host(pihole_config * config, char * host) {
    config_set_host(config, host);
}

void pihelper_config_set_password(pihole_config * config, char * password) {
    config_set_password(config, password);
}

void pihelper_config_set_api_key(pihole_config * config, char * api_key) {
    config_set_api_key(config, api_key);
}

pihole_config * pihelper_read_config(char * config_path) {
    read_config(config_path);
}

int pihelper_save_config(pihole_config * config, char * config_path) {
    save_config(config, config_path);
}

void pihelper_free_config(pihole_config * config) {
    free_config(config);
}

