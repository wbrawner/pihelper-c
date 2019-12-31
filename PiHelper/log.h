#ifndef PIHELPER_LOG
#define PIHELPER_LOG

static int PIHELPER_LOG_DISABLED = -1;
static int PIHELPER_LOG_ERROR    =  0;
static int PIHELPER_LOG_WARN     =  1;
static int PIHELPER_LOG_INFO     =  2;
static int PIHELPER_LOG_DEBUG    =  3;

void set_log_level(int level);

void write_log(int level, char * format, ...);
#endif
