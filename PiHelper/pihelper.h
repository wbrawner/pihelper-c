#ifndef PIHELPER
#define PIHELPER
#include "config.h"
#include "log.h"
#include "network.h"
static int PIHELPER_OK = 0;
static int PIHELPER_HELP = 1;
static int PIHELPER_INVALID_COMMANDS = 2;

void pihelper_set_log_level(int level);

int pihelper_get_status(pihole_config * config);

int pihelper_enable_pihole(pihole_config * config);

int pihelper_disable_pihole(pihole_config * config, char * duration);
#endif

