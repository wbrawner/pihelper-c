#include "pihelper.h"

int pihelper_get_status(pihole_config * config) {
    return get_status(config);
}

int pihelper_enable_pihole(pihole_config * config) {
    return enable_pihole(config);
}

int pihelper_disable_pihole(pihole_config * config, char * duration) {
    return disable_pihole(config, duration);
}

void pihelper_set_log_level(int level) {
    set_log_level(level);
}
