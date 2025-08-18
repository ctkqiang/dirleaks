#include "../include/csharp_path.h"

const char *paths[] = {
    "/web.config",
    "/appsettings.json",
    "/bin/",
    "/obj/",
    "/.vs/",
    "/packages/",
    "/Global.asax"
};

const int count = sizeof(paths) / sizeof(paths[0x0]);