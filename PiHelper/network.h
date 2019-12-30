#ifndef PIHELPER_NETWORK
#define PIHELPER_NETWORK

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

static void parse_status(char * raw_json);

static void append_query_parameter(char ** host, char * key, char * value);

static char * prepend_scheme(char * raw_host);
#endif

