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
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <json-c/json_object.h>
#include <json-c/json_tokener.h>
#include "config.h"
#include "log.h"
#include "network.h"

int get_status(pihole_config * config) {
    write_log(PIHELPER_LOG_DEBUG, "Getting Pi-hole status…");
    char * formatted_host = prepend_scheme(config->host);
    char * response = get(formatted_host);
    free(formatted_host);
    if (response == NULL) {
        write_log(PIHELPER_LOG_ERROR, "Failed to retrieve status for Pi-hole at %s\n", config->host);
        return PIHELPER_OPERATION_FAILED;
    } else {
        int status = parse_status(response);
        free(response);
        return status;
    }
}

int enable_pihole(pihole_config * config) {
    write_log(PIHELPER_LOG_DEBUG, "Enabling Pi-hole…");
    char * formatted_host = prepend_scheme(config->host);
    append_query_parameter(&formatted_host, AUTH_QUERY, config->api_key);
    append_query_parameter(&formatted_host, ENABLE_QUERY, NULL);
    char * response = get(formatted_host);
    free(formatted_host);
    if (response == NULL) {
        return PIHELPER_OPERATION_FAILED;
    } else {
        int status = parse_status(response);
        free(response);
        return status;
    }
}

int disable_pihole(pihole_config * config, char * duration) {
    if (*duration == '\0') {
        write_log(PIHELPER_LOG_DEBUG, "Disabling Pi-hole permanently…");
    } else {
        write_log(PIHELPER_LOG_DEBUG, "Disabling Pi-hole for %s seconds…", duration);
    }
    char * formatted_host = prepend_scheme(config->host);
    append_query_parameter(&formatted_host, AUTH_QUERY, config->api_key);
    append_query_parameter(&formatted_host, DISABLE_QUERY, duration);
    char * response = get(formatted_host);
    free(formatted_host);
    if (response == NULL) {
        return PIHELPER_OPERATION_FAILED;
    } else {
        int status = parse_status(response);
        free(response);
        return status;
    }
}

/**
 * Used to handle curl data callbacks
 */
static size_t receive_data(char *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t realsize = size * nmemb;
    http_response * response = (http_response *) userdata;
    char * next = realloc(response->body, response->size + realsize + 1);
    response->body = next;
    memcpy(&(response->body[response->size]), ptr, realsize);
    response->size += realsize;
    response->body[response->size] = '\0';
    return realsize;
}

/**
 * Used to make a GET request to a given endpoint
 */
static char * get(char endpoint[]) {
    http_response response;
    response.body = malloc(1);
    response.size = 0;
    curl_global_init(CURL_GLOBAL_ALL);
    CURL * curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, endpoint);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, receive_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
    if (LOG_LEVEL == PIHELPER_LOG_DEBUG) {
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
    }
    int res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    if (res == CURLE_OK) {
        return response.body;
    } else {
        free(response.body);
        return NULL;
    }
}

/**
 * Given a potentially unformatted host (missing scheme), prepends the scheme (http://) to the host. Note
 * that the caller is responsible for freeing the memory allocated by this method.
 * @return a pointer to the host with the scheme prepended, or the given pointer if the host is already
 * properly formatted.
 */
static char * prepend_scheme(char * raw_host) {
    if (raw_host == NULL) return NULL;
    char * formatted_host;
    if (strstr(raw_host, HTTP_SCHEME) != raw_host
            && strstr(raw_host, HTTPS_SCHEME) != raw_host) {
        formatted_host = malloc(URL_FORMAT_LEN + strlen(raw_host));
        sprintf(formatted_host, URL_FORMAT, raw_host);
    } else {
        formatted_host = malloc(strlen(raw_host));
        strcpy(formatted_host, raw_host);
    }
    return formatted_host;
}

static int parse_status(char * raw_json) {
    json_tokener *tok = json_tokener_new();
    json_object *jobj = NULL;
    int stringlen = 0;
    int retval = PIHELPER_OPERATION_FAILED;
    enum json_tokener_error jerr;
    do {
        stringlen = strlen(raw_json);
        jobj = json_tokener_parse_ex(tok, raw_json, stringlen);
    } while ((jerr = json_tokener_get_error(tok)) == json_tokener_continue);
    if (jerr != json_tokener_success) {
        write_log(PIHELPER_LOG_ERROR, "Failed to parse JSON: %s", json_tokener_error_desc(jerr));
        json_tokener_free(tok);
        return retval;
    }
    write_log(PIHELPER_LOG_DEBUG, "%s", json_object_to_json_string_ext(jobj, JSON_C_TO_STRING_PRETTY));
    json_object *status;
    const char * status_string;
    if (json_object_object_get_ex(jobj, "/status", &status) == 0
            && (status_string = json_object_get_string(status)) != NULL) {
        if (strstr(status_string, "enabled") == status_string) {
            retval = PIHELPER_ENABLED;
        } else if (strstr(status_string, "disabled") == status_string) {
            retval = PIHELPER_DISABLED;
        }
    } else {
        write_log(PIHELPER_LOG_DEBUG, "Unable to parse response: %s", raw_json);
    }
    json_tokener_free(tok);
    json_object_put(jobj);
    return retval;
}

static void append_query_parameter(char ** host, char * key, char * value) {
    char separator = strstr(*host, "?") ? '&' : '?';
    int host_len = strlen(*host);
    int new_len = host_len + 1 + strlen(key) +  1;
    if (value) {
        // Add another byte for the '='
        new_len += strlen(value) + 1;
    }
    *host = realloc(*host, new_len);
    char * format = value ? "%c%s=%s" : "%c%s";
    sprintf(&((*host)[host_len]), format, separator, key, value);
    (*host)[strlen(*host)] = '\0';
}

