#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include "config.h"
#include "log.h"
#include "network.h"

static char * URL_FORMAT = "http://%s/admin/api.php";
static int URL_FORMAT_LEN = 22;
static char * AUTH_QUERY = "auth";
static char * ENABLE_QUERY = "enable";
static char * DISABLE_QUERY = "disable";
static char * HTTP_SCHEME = "http://";
static int HTTP_SCHEME_LEN = 8;
static char * HTTPS_SCHEME = "https://";
static int HTTPS_SCHEME_LEN = 9;

int get_status(pihole_config * config) {
    char * formatted_host = prepend_scheme(config->host);
    char * response = get(formatted_host);
    if (response == NULL) {
        return 1;
    } else {
        parse_status(response);
        free(response);
        return 0;
    }
}

int enable_pihole(pihole_config * config) {
    char * formatted_host = prepend_scheme(config->host);
    append_query_parameter(&formatted_host, AUTH_QUERY, config->api_key);
    append_query_parameter(&formatted_host, ENABLE_QUERY, NULL);
    char * response = get(formatted_host);
    if (response == NULL) {
        return 1;
    } else {
        parse_status(response);
        free(response);
        return 0;
    }
}

int disable_pihole(pihole_config * config, char * duration) {
    char * formatted_host = prepend_scheme(config->host);
    append_query_parameter(&formatted_host, AUTH_QUERY, config->api_key);
    append_query_parameter(&formatted_host, DISABLE_QUERY, duration);
    char * response = get(formatted_host);
    if (response == NULL) {
        return 1;
    } else {
        parse_status(response);
        free(response);
        return 0;
    }
}


/*
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

static char * get(char endpoint[]) {
    http_response response;
    response.body = malloc(1);
    response.size = 0;
    curl_global_init(CURL_GLOBAL_ALL);
    CURL * curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, endpoint);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, receive_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    int res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    if (res == CURLE_OK) {
        return response.body;
    } else {
        return NULL;
    }
}

/*
 * Given a potentially unformatted host (missing scheme), prepends the scheme (http://) to the host. Note
 * that the caller is responsible for freeing the memory allocated by this method.
 * @return a pointer to the host with the scheme prepended
 */
static char * prepend_scheme(char * raw_host) {
    char * formatted_host;
    if (strnstr(raw_host, HTTP_SCHEME, HTTP_SCHEME_LEN) == NULL
            && strnstr(raw_host, HTTPS_SCHEME, HTTPS_SCHEME_LEN) == NULL) {
        formatted_host = malloc(URL_FORMAT_LEN + strlen(raw_host));
        sprintf(formatted_host, URL_FORMAT, raw_host);
    } else {
        formatted_host = malloc(strlen(raw_host));
        strcpy(formatted_host, raw_host);
    }
    return formatted_host;
}

static void parse_status(char * raw_json) {
    cJSON *json = cJSON_Parse(raw_json);
    cJSON *status = cJSON_GetObjectItemCaseSensitive(json, "status");
    if (cJSON_IsString(status) && (status->valuestring != NULL)) {
        printf("Pi-hole status: %s\n", status->valuestring);
    } else {
        write_log(LOG_DEBUG, "Unable to parse response: %s", raw_json);
    }
    cJSON_Delete(json);
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

