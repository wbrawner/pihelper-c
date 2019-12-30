#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "log.h"
#include "network.h"
#include "pihelper.h"

int main(int argc, char ** argv) {

    bool configure, enable;
    char * disable;
    char * config_path;
    char ch;
    while ((ch = getopt_long(argc, argv, "cd:ef:hv", longopts, NULL)) != -1) {
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
                write_log(LOG_DEBUG, "Disabling pi-hole for %s seconds", disable);
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
                    write_log(LOG_DEBUG, "Fixed config_path: %s", config_path);
                } else {
                    // This is an absolute path, copy as-is
                    config_path = malloc(strlen(optarg) + 1);
                    strncpy(config_path, optarg, strlen(optarg));
                    config_path[strlen(optarg)] = '\0';
                }
                break;
            case 'v':
                // TODO: Add log level and set here
                //PIHELPER_DEBUG = true;
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
    }

    pihole_config * config;
    if (configure) {
        config = configure_pihole(config_path);
    } else {
        config = read_config(config_path);
    }

    if (config == NULL) {
        printf("Failed to parse Pi-Helper config\n");
        exit(1);
    }

    if (enable && disable != 0) {
        print_usage();
        return PIHELPER_INVALID_COMMANDS;
    } else if (enable) {
        return enable_pihole(config);
    } else if (disable != 0) {
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

