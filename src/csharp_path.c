#include "../include/csharp_path.h"

const char *csharp_paths[] = {
    "/web.config",
    "/appsettings.json",
    "/bin/",
    "/obj/",
    "/.vs/",
    "/packages/",
    "/Global.asax"
};

const int csharp_count = sizeof(csharp_paths) / sizeof(csharp_paths[0x0]);