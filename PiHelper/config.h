#ifndef PIHELPER_CONFIG
#define PIHELPER_CONFIG
#include <openssl/sha.h>

static char * DEFAULT_CONFIG_PATH = "/.config/pihelper.conf";

typedef struct {
    char * host;
    char * api_key;
} pihole_config;


void save_config(pihole_config * config, char * config_path);

pihole_config * read_config(char * config_path);

pihole_config * configure_pihole(char * config_path);
#endif

