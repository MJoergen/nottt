#ifndef TRACE_H
#define TRACE_H

#include <ostream>

// The tournament log. It is null unless a log file was given on the command
// line.
extern std::ostream *g_log;

#define LOG(x)           \
    do {                 \
        if (g_log) {     \
            *g_log << x; \
        }                \
    } while (0)

#endif
