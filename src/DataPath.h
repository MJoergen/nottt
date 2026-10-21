#pragma once

#include <string>

// Returns the path of a data file, given relative to the data directory,
// e.g. "assets/RedCross.png".
std::string dataPath(const std::string& relativePath);
