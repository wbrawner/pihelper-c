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
#include "log.h"

static char * DEFAULT_CONFIG_PATH = "/.config/pihelper.conf";

int main(int argc, char ** argv) {

    bool configure = false, enable = false;
    char * disable = NULL;
    char * config_path = NULL;
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
                    disable = malloc(1);
                    disable[0] = '\0';
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
                    config_path[full_path_len - 1] = '\0';
                    free(cwd);
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
                return PIHELPER_OPERATION_FAILED;
        }
    }
    if (config_path == NULL) {
        char * home_dir = getenv("HOME");
        int path_len = strlen(home_dir) + strlen(DEFAULT_CONFIG_PATH);
        config_path = malloc(path_len + 1);
        sprintf(config_path, "%s%s", home_dir, DEFAULT_CONFIG_PATH);
        config_path[path_len] = '\0';
    }
    if (access(config_path, F_OK)) {
        char * user_input = malloc(4);
        // Intentionally using printf here to ensure that this is always printed
        printf("No Pi-Helper configuration found. Would you like to create it now? [Y/n] ");
        fgets(user_input, 3, stdin);
        user_input[3] = '\0';
        write_log(PIHELPER_LOG_DEBUG, "User's input: %s", user_input);
        if (strstr(user_input, "\n") == user_input
                || strstr(user_input, "Y") == user_input
                || strstr(user_input, "y") == user_input
           ) {
            configure = true;
            free(user_input);
        } else {
            free(config_path);
            free(user_input);
            return 1;
        }
    } else {
        write_log(PIHELPER_LOG_DEBUG, "Using config file at: %s", config_path);
    }

    pihole_config * config;
    if (configure) {
        write_log(PIHELPER_LOG_DEBUG, "Configuring PiHelper");
        config = configure_pihole(config_path);
    } else {
        write_log(PIHELPER_LOG_DEBUG, "Reading existing PiHelper config");
        config = pihelper_read_config(config_path);
    }
    int status;
    if (config == NULL) {
        write_log(PIHELPER_LOG_ERROR, "Failed to parse Pi-Helper config at %s", config_path);
        status = PIHELPER_OPERATION_FAILED;
    } else if (enable && disable != NULL) {
        print_usage();
        status = PIHELPER_OPERATION_FAILED;
    } else if (enable) {
        status = pihelper_enable_pihole(config);
    } else if (disable != NULL) {
        status = pihelper_disable_pihole(config, disable);
        free(disable);
    } else {
        status = pihelper_get_status(config);
    }

    char * status_message = NULL;
    if (status == PIHELPER_ENABLED) {
        status_message = "enabled";
    } else if (status == PIHELPER_DISABLED) {
        status_message = "disabled";
    }

    if (status_message != NULL) {
        write_log(PIHELPER_LOG_INFO, "Pi-hole status: %s", status_message);
    }
    free(config_path);
    pihelper_free_config(config);
    return status;
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

pihole_config * configure_pihole(char * config_path) {
    if (access(config_path, F_OK) == 0) {
        // TODO: Check if file is accessible for read/write (not just if it exists)
        write_log(PIHELPER_LOG_WARN, "WARNING: The config file already exists. Continuing will overwrite any existing configuration.\n");
    }
    pihole_config * config = pihelper_new_config();
    printf("Enter the hostname or ip address for your pi-hole: ");
    char * host = calloc(1, 257);
    fgets(host, 256, stdin);
    host[256] = '\0';
    write_log(PIHELPER_LOG_DEBUG, "User entered \"%s\" for host", host);
    pihelper_config_set_host(config, host);
    free(host);
    char * raw_pass = getpass("Enter the api key or web password for your pi-hole: ");
    if (strlen(raw_pass) != 64) {
        pihelper_config_set_password(config, raw_pass);
    } else {
        pihelper_config_set_api_key(config, raw_pass);
    }
    free(raw_pass);
    // TODO: Make an authenticated request to verify that the credentials are valid and save the config
    pihelper_save_config(config, config_path);
    return config;
}

