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
