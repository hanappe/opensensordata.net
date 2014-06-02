#ifndef _LOG_H
#define _LOG_H

#define LOG_DEBUG 0
#define LOG_INFO 1
/* App can continue: */
#define LOG_WARNING 2
/* App cannot continue: */
#define LOG_ERROR 3

int log_get_level();
void log_set_level(int level);
int log_set_file(const char* filename);
void log_set_filep(FILE* file);

void log_err(const char* format, ...);
void log_warn(const char* format, ...);
void log_info(const char* format, ...);
void log_debug(const char* format, ...);

#endif // _LOG_H
