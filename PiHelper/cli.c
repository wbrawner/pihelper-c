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
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "cli.h"
#include "pihelper.h"

int main(int argc, char ** argv) {

    bool configure, enable;
    char * disable = NULL;
    char * config_path;
    char ch;
    while ((ch = getopt_long(argc, argv, shortopts, longopts, NULL)) != -1) {
        switch(ch) {
            case 'c':
                configure = true;
                break;
            case 'd':
                if (optarg != NULL) {
                    disable = malloc(strlen(optarg) + 1);
                    strncpy(disable, optarg, strlen(optarg));
                    disable[strlen(optarg)] = '\0';
                } else {
                    disable = "";
                }
                break;
            case 'e':
                enable = true;
                break;
            case 'f':
                if (optarg == NULL) break;
                if (strstr(optarg, "/") != optarg) {
                    // This is a relative path, prepend the current working directory
                    char * cwd = getcwd(NULL, 0);
                    int full_path_len = strlen(cwd) + 1 + strlen(optarg) + 1;
                    config_path = malloc(full_path_len);
                    strncpy(config_path, cwd, strlen(cwd));
                    config_path[strlen(cwd)] = '/';
                    strncpy(&(config_path[strlen(cwd) + 1]), optarg, strlen(optarg));
                    config_path[full_path_len] = '\0';
                } else {
                    // This is an absolute path, copy as-is
                    config_path = malloc(strlen(optarg) + 1);
                    strncpy(config_path, optarg, strlen(optarg));
                    config_path[strlen(optarg)] = '\0';
                }
                break;
            case 'q':
                pihelper_set_log_level(PIHELPER_LOG_DISABLED);
                break;
            case 'v':
                pihelper_set_log_level(PIHELPER_LOG_DEBUG);
                break;
            case 'h':
            default:
                print_usage();
                return PIHELPER_HELP;
        }
    }
    if (config_path == NULL) {
        char * home_dir = getenv("HOME");
        int path_len = strlen(home_dir) + strlen(DEFAULT_CONFIG_PATH);
        config_path = malloc(path_len + 1);
        sprintf(config_path, "%s%s", home_dir, DEFAULT_CONFIG_PATH);
        config_path[path_len + 1] = '\0';
    }
    FILE * config_file = fopen(config_path, "r+");
    if (config_file == NULL) {
        char * user_input = malloc(2);
        // Intentionally using printf here to ensure that this is always printed
        printf("No Pi-Helper configuration found. Would you like to create it now? [Y/n] ");
        fgets(user_input, 2, stdin);
        if (strstr(user_input, "\n") == user_input
                || strstr(user_input, "Y") == user_input
                || strstr(user_input, "y") == user_input
           ) {
            configure = true;
        } else {
            return 1;
        }
    } else {
        write_log(PIHELPER_LOG_DEBUG, "Using config file at: %s", config_path);
    }

    pihole_config * config;
    if (configure) {
        config = configure_pihole(config_path);
    } else {
        config = read_config(config_path);
    }

    if (config == NULL) {
        write_log(PIHELPER_LOG_ERROR, "Failed to parse Pi-Helper config at %s", config_path);
        exit(1);
    }

    if (enable && disable != NULL) {
        print_usage();
        return PIHELPER_INVALID_COMMANDS;
    } else if (enable) {
        return enable_pihole(config);
    } else if (disable != NULL) {
        return disable_pihole(config, disable);
    } else {
        return get_status(config);
    }
}

void print_usage() {
    printf("Usage: pihelper [options]\n");
    printf(" -c, --configure              Configure Pi-Helper\n");
    printf(" -d, --disable <duration>     Disable the Pi-hole for a given duration, or permanently if empty\n");
    printf(" -e, --enable                 Enable the Pi-hole\n");
    printf(" -f, --file <config-file>     Use the given config file instead of the default\n");
    printf(" -h, --help                   Display this message\n");
    printf(" -q, --quiet                  Don't print anything\n");
    printf(" -v, --verbose                Print debug logs\n");
}

