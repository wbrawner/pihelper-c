#ifndef PIHELPER_LOG
#define PIHELPER_LOG

static int LOG_ERROR = 0;
static int LOG_WARN  = 1;
static int LOG_INFO  = 2;
static int LOG_DEBUG = 3;

void write_log(int level, char * format, ...);
#endif
