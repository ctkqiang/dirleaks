#include "../include/generic_path.h"

const char *paths[] = {
    "/.git/",
    "/.svn/",
    "/.hg/",
    "/.idea/",
    "/.vscode/",
    "/logs/",
    "/debug.log",
    "/backup/",
    "/index.bak",
    "/index.old"
};

const int count = sizeof(paths) / sizeof(paths[0x0]);