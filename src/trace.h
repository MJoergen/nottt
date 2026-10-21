#ifndef TRACE_H
#define TRACE_H

#include <ostream>

// The tournament log. It is null unless a log file was given on the command
// line.
extern std::ostream *gpLog;

#define LOG(x)           \
    do {                 \
        if (gpLog) {     \
            *gpLog << x; \
        }                \
    } while (0)

#endif
