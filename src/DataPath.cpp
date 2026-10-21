#include <SDL.h>
#include <fstream>

#include "DataPath.h"

/*
 * The data files (assets/ and tables/) are looked up relative to the
 * executable, not the current directory. They are found next to it, as in the
 * repository, or in ../share/nottt/, as installed by "make install".
 * If the file is in neither place, the path next to the executable is
 * returned, so that error messages name it.
 */
std::string dataPath(const std::string& relativePath) {
    std::string baseDir;
    char *basePath = SDL_GetBasePath();
    if (basePath != NULL) {
        baseDir = basePath; // Ends with a path separator
        SDL_free(basePath);
    }

    const std::string localPath = baseDir + relativePath;
    if (std::ifstream(localPath))
        return localPath;

    const std::string installedPath = baseDir + "../share/nottt/" + relativePath;
    if (std::ifstream(installedPath))
        return installedPath;

    return localPath;
} // end of dataPath

/*
 * The logs are written to a directory the user can write to, wherever the
 * program is installed or started from: ~/.local/share/nottt/logs/ on Linux
 * and %APPDATA%\nottt\logs\ on Windows. SDL creates it if it is missing.
 * The name must be a plain file name, so that it cannot leave the directory:
 * only letters, digits, '.', '_' and '-', and not starting with '.'.
 */
std::string logPath(const std::string& name) {
    if (name.empty() || name[0] == '.')
        return "";
    for (char c : name) {
        const bool isPlain = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '.' || c == '_' || c == '-';
        if (!isPlain)
            return "";
    }

    char *prefPath = SDL_GetPrefPath("nottt", "logs");
    if (prefPath == NULL)
        return "";
    const std::string logDir = prefPath; // Ends with a path separator
    SDL_free(prefPath);

    return logDir + name;
} // end of logPath
