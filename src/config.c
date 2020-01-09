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

int save_config(pihole_config * config, char * config_path) {
    if (mkdirs(config_path)) {
        perror(config_path);
        return 1;
    }
    FILE * config_file = fopen(config_path, "w");
    int config_len = strlen(config->host) + strlen(config->api_key) + 16;
    char config_string[config_len + 1];
    snprintf(config_string, config_len, "host=%s\napi-key=%s\n", config->host, config->api_key);
    config_string[config_len + 1] = '\0';
    fputs(config_string, config_file);
    fclose(config_file);
    return 0;
}

/*
 * Calculate the hash of the password
 */
static char * hash_string (char * raw_string) {
    unsigned char bytes[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char *) raw_string, strlen(raw_string), bytes);
    char * hash = malloc(MAX_PIHOLE_API_KEY + 1);
    int i;
    for(i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(hash + (i * 2), "%02x", bytes[i]);
    }
    hash[MAX_PIHOLE_API_KEY] = '\0';
    return hash;
}

pihole_config * read_config(char * config_path) {
    if (access(config_path, F_OK)) {
        write_log(PIHELPER_LOG_ERROR, "The specified config file doesn't exist: %s", config_path);
        return NULL;
    }

    FILE * config_file = fopen(config_path, "r");
    char * host = calloc(1, _POSIX_HOST_NAME_MAX + 7);
    fgets(host, _POSIX_HOST_NAME_MAX + 7, config_file);
    if (strstr(host, "host=") == NULL || strlen(host) < 7) {
        write_log(PIHELPER_LOG_DEBUG, "Config file contains invalid host: %s", host);
        write_log(PIHELPER_LOG_ERROR, "Invalid config file");
        fclose(config_file);
        return NULL;
    }
    pihole_config * config = pihole_config_new();
    config_set_host(config, host + 5);
    free(host);
    char * api_key = calloc(1, 74);
    fgets(api_key, 74, config_file);
    fclose(config_file);
    if (strstr(api_key, "api-key=") == NULL
            || strlen(api_key) < 9) {
        write_log(PIHELPER_LOG_DEBUG, "Config file contains invalid api key: %s", api_key);
        write_log(PIHELPER_LOG_WARN, "The config file is missing a valid API key. Authenticated operations won't work.");
        free(api_key);
        fclose(config_file);
        return config;
    }
    config_set_api_key(config, api_key + 8);
    free(api_key);
    write_log(PIHELPER_LOG_DEBUG, "Using host %s and api key %s", config->host, config->api_key);
    return config;
}

pihole_config * pihole_config_new() {
    pihole_config * config;
    if ((config = malloc(sizeof(pihole_config))) == NULL) {
        write_log(PIHELPER_LOG_ERROR, "Failed to allocate memory for config");
        free_config(config);
        return NULL;
    }
    if ((config->host = calloc(1, _POSIX_HOST_NAME_MAX + 1)) == NULL) {
        write_log(PIHELPER_LOG_ERROR, "Failed to allocate memory for config host");
        free_config(config);
        return NULL;
    }
    if ((config->api_key = calloc(1, MAX_PIHOLE_API_KEY + 1)) == NULL) {
        write_log(PIHELPER_LOG_ERROR, "Failed to allocate memory for config API key");
        free_config(config);
        return NULL;
    }
    config->host[_POSIX_HOST_NAME_MAX] = '\0';
    config->api_key[MAX_PIHOLE_API_KEY] = '\0';
    return config;
}

void config_set_host(pihole_config * config, char * host) {
    strncpy(config->host, host, _POSIX_HOST_NAME_MAX);
    config->host[_POSIX_HOST_NAME_MAX] = '\0';
    trim_string(config->host);
}

void config_set_password(pihole_config * config, char * password) {
    trim_string(password);
    // This is definitely not an API key, so hash it
    // The Pi-hole hashes twice so we do the same here
    char * first = hash_string(password);
    char * hash = hash_string(first);
    free(first);
    config_set_api_key(config, hash);
    free(hash);
}

void config_set_api_key(pihole_config * config, char * api_key) {
    strncpy(config->api_key, api_key, MAX_PIHOLE_API_KEY);
    config->api_key[MAX_PIHOLE_API_KEY] = '\0';
    trim_string(config->api_key);
}

static void trim_string(char * raw_str) {
    char * newline = strstr(raw_str, "\n");
    if (newline != NULL) {
        raw_str[strlen(raw_str) - strlen(newline)] = '\0';
    }
}

void free_config(pihole_config * config) {
    if (config == NULL) return;
    if (config->host != NULL) {
        free(config->host);
    }
    if (config->api_key != NULL) {
        free(config->api_key);
    }
    free(config);
}

