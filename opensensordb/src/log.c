#include "osdb_priv.h"

static int _log_level = LOG_INFO;
static FILE* _log_file = NULL;

int log_set_file(const char* filename)
{
        FILE* fp = fopen(filename, "a");
        if (fp == NULL) return -1;
        _log_file = fp;
        return 0;
}

void log_set_filep(FILE* file)
{
        _log_file = file;
}

int log_get_level()
{
        return _log_level;
}

void log_set_level(int level)
{
        _log_level = level;
        if (_log_level < LOG_DEBUG) 
                _log_level = LOG_DEBUG;
        else if (_log_level > LOG_ERROR)
                _log_level = LOG_ERROR;
}

static char _timestamp_buffer[256];
static time_t _timestamp_last = 0;

// Not re-entrant!
static const char* get_timestamp()
{
        struct timeval tv;
        gettimeofday(&tv, NULL);
        if (tv.tv_sec != _timestamp_last) {
                struct tm r;
                localtime_r(&tv.tv_sec, &r);
                snprintf(_timestamp_buffer, 256, "%04d-%02d-%02d %02d:%02d:%02d",
                         1900 + r.tm_year, 1 + r.tm_mon, r.tm_mday, 
                         r.tm_hour, r.tm_min, r.tm_sec);
        }
        return _timestamp_buffer;
}

static void log_(int level, const char* s)
{
        const char* time = get_timestamp();
        const char* type = "Unknown";
        switch (level) {
        case LOG_DEBUG: type = "Debug"; break;
        case LOG_INFO: type = "Info"; break;
        case LOG_WARNING: type = "Warning"; break;
        case LOG_ERROR: type = "Error"; break;
        }

        fprintf(_log_file, "[%s] %s: %s\n", time, type, s);
        fflush(_log_file);
}

void log_err(const char* format, ...)
{
        char buffer[1024];
        va_list ap;

        if (_log_level > LOG_ERROR)
                return;
        if (_log_file == NULL)
                _log_file = stderr;

        va_start(ap, format);
        vsnprintf(buffer, 1024, format, ap);
        buffer[1023] = 0;
        va_end(ap);

        log_(LOG_ERROR, buffer);
}

void log_warn(const char* format, ...)
{
        char buffer[1024];
        va_list ap;

        if (_log_level > LOG_WARNING)
                return;
        if (_log_file == NULL)
                _log_file = stderr;

        va_start(ap, format);
        vsnprintf(buffer, 1024, format, ap);
        buffer[1023] = 0;
        va_end(ap);

        log_(LOG_WARNING, buffer);
}

void log_info(const char* format, ...)
{
        char buffer[1024];
        va_list ap;

        if (_log_level > LOG_INFO)
                return;
        if (_log_file == NULL)
                _log_file = stderr;

        va_start(ap, format);
        vsnprintf(buffer, 1024, format, ap);
        buffer[1023] = 0;
        va_end(ap);

        log_(LOG_INFO, buffer);
}

void log_debug(const char* format, ...)
{
        char buffer[1024];
        va_list ap;

        if (_log_level > LOG_DEBUG) 
                return;
        if (_log_file == NULL)
                _log_file = stderr;

        va_start(ap, format);
        vsnprintf(buffer, 1024, format, ap);
        buffer[1023] = 0;
        va_end(ap);

        log_(LOG_DEBUG, buffer);
}

