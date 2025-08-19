#include "../include/cpp_path.h"

const char *cpp_paths[] = {
    "/cgi-bin/",
    "/cgi-bin/test.cgi",
    "/cgi-bin/status",
    "/cgi-bin/admin.cgi",
    "/fcgi-bin/",
    "/scripts/",
    "/server-status",
    "/status",
    "/run",
    "/execute",
    "/upload",
    "/logs/error.log",
    "/logs/access.log",
    "/httpd.conf",
    "/nginx.conf"
};

const int cpp_count = sizeof(cpp_paths) / sizeof(cpp_paths[0x0]);