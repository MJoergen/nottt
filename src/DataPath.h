#pragma once

#include <string>

// Returns the path of a data file, given relative to the data directory,
// e.g. "assets/RedCross.png".
std::string dataPath(const std::string& relativePath);

// Returns the path of the log file "name" in the user's log directory, which
// is created if needed. Returns "" if "name" is not a plain file name, or if
// the directory is not available.
std::string logPath(const std::string& name);
