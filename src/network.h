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
#ifndef PIHELPER_NETWORK
#define PIHELPER_NETWORK

#define URL_FORMAT     "http://%s/admin/api.php"
#define URL_FORMAT_LEN 22
#define AUTH_QUERY     "auth"
#define ENABLE_QUERY   "enable"
#define DISABLE_QUERY  "disable"
#define HTTP_SCHEME    "http://"
#define HTTPS_SCHEME   "https://"

typedef struct {
    size_t size;
    char * body;
} http_response;

typedef struct {
    size_t domains_being_blocked;
    size_t dns_queries_today;
    size_t ads_blocked_today;
    double ads_percentage_today;
    size_t unique_domains;
    size_t queries_forwarded;
    size_t queries_cached;
    size_t clients_ever_seen;
    size_t unique_clients;
    size_t dns_queries_all_types;
    size_t reply_NODATA;
    size_t reply_NXDOMAIN;
    size_t reply_CNAME;
    size_t reply_IP;
    size_t privacy_level;
    char * status;
} pihole_status;

int get_status(pihole_config * config);

int enable_pihole(pihole_config * config);

int disable_pihole(pihole_config * config, char * duration);

static char * get(char endpoint[]);

static int parse_status(char * raw_json);

static void append_query_parameter(char ** host, char * key, char * value);

static char * prepend_scheme(char * raw_host);
#endif

