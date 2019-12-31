#include <getopt.h>

static char * shortopts = "cd:ef:hqv";

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

