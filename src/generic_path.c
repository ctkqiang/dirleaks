#include "../include/generic_path.h"

const char *generic_paths[] = {
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

const int generic_count = sizeof(generic_paths) / sizeof(generic_paths[0x0]);