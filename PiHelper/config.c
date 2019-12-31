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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <limits.h>
#include <pwd.h>
#include <unistd.h>
#include "config.h"

int mkdirs(char * path) {
    char * curPos = strstr(path, "/") + 1;
    char parents[strlen(path)];
    int retval = 0;
    while (curPos != NULL) {
        snprintf(parents, strlen(path) - strlen(curPos) + 1, "%s", path);
        curPos = strstr(curPos + 1, "/");
        if (access(parents, F_OK)) {
            if ((retval = mkdir(parents, 0755)) != 0) {
                return retval;
            }
        }
    }
    return retval;
}

void save_config(pihole_config * config, char * config_path) {
    if (mkdirs(config_path)) {
        perror(config_path);
        exit(1);
    }
    FILE * config_file = fopen(config_path, "w");
    int config_len = strlen(config->host) + strlen(config->api_key) + 16;
    char config_string[config_len + 1];
    snprintf(config_string, config_len, "host=%s\napi-key=%s\n", config->host, config->api_key);
    config_string[config_len + 1] = '\0';
    fputs(config_string, config_file);
    fclose(config_file);
}

/*
 * Calculate the hash of the password
 */
static char * hash_string (char * raw_string) {
    unsigned char bytes[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char *) raw_string, strlen(raw_string), bytes);
    char * hash = malloc(65);
    int i;
    for(i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(hash + (i * 2), "%02x", bytes[i]);
    }
    hash[64] = '\0';
    return hash;
}

pihole_config * read_config(char * config_path) {
    if (access(config_path, F_OK)) {
        write_log(PIHELPER_LOG_ERROR, "The specified config file doesn't exist: %s", config_path);
        return NULL;
    }

    pihole_config * config = calloc(1, sizeof(pihole_config));
    FILE * config_file = fopen(config_path, "r");
    char host[_POSIX_HOST_NAME_MAX + 7];
    fgets(host, _POSIX_HOST_NAME_MAX + 7, config_file);
    if (strstr(host, "host=") == NULL || strlen(host) < 7) {
        write_log(PIHELPER_LOG_DEBUG, "Config file contains invalid host: %s", host);
        write_log(PIHELPER_LOG_ERROR, "Invalid config file");
        exit(1);
    }
    config->host = calloc(1, strlen(host) - 7);
    strncpy(config->host, host + 5, strlen(host) - 6);
    char api_key[74];
    fgets(api_key, 74, config_file);
    if (strstr(api_key, "api-key=") == NULL) {
        write_log(PIHELPER_LOG_DEBUG, "Config file contains invalid api key: %s", api_key);
        write_log(PIHELPER_LOG_ERROR, "Invalid config file");
        exit(1);
    }
    config->api_key = calloc(1, strlen(api_key) - 8);
    strncpy(config->api_key, api_key + 8, strlen(api_key) - 9);
    fclose(config_file);
    return config;
}

pihole_config * configure_pihole(char * config_path) {
    if (access(config_path, F_OK) == 0) {
        // TODO: Check if file is accessible for read/write (not just if it exists)
        write_log(PIHELPER_LOG_WARN, "WARNING: The config file already exists. Continuing will overwrite any existing configuration.\n");
    }
    pihole_config * config = malloc(sizeof(pihole_config));
    config->host = calloc(1, _POSIX_HOST_NAME_MAX);
    config->host[_POSIX_HOST_NAME_MAX] = '\0';
    // Intentionally using printf to ensure this is always printed
    printf("Enter the hostname or ip address for your pi-hole: ");
    fgets(config->host, _POSIX_HOST_NAME_MAX, stdin);
    char * newline = strstr(config->host, "\n");
    if (newline != NULL) {
        config->host[strlen(config->host) - strlen(newline)] = '\0';
    }
    config->api_key = getpass("Enter the api key or web password for your pi-hole: ");
    if (strlen(config->api_key) < 64) {
        // This is definitely not an API key, so hash it
        // The Pi-hole hashes twice so we do the same here
        char * first = hash_string(config->api_key);
        char * hash = hash_string(first);
        free(first);
        config->api_key = hash;
    }
    // TODO: Make an authenticated request to verify that the credentials are valid and save the config
    save_config(config, config_path);
    return config;
}

