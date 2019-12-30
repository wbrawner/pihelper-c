#include <getopt.h>
#include "config.h"

static struct option longopts[] = {
        { "configure",  no_argument,            NULL,           'c' },
        { "disable",    optional_argument,      NULL,           'd' },
        { "enable",     no_argument,            NULL,           'e' },
        { "file",       required_argument,      NULL,           'f' },
        { "help",       no_argument,            NULL,           'h' },
        { "quiet",      no_argument,            NULL,           'q' },
        { "verbose",    no_argument,            NULL,           'v' }
};

void print_usage();

pihole_config * configure_pihole(char * config_path);

