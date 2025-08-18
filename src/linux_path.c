#include "../include/linux_path.h"

const char *paths[] = {
    "/etc/passwd",
    "/etc/shadow",
    "/etc/hosts",
    "/etc/hostname",
    "/etc/network/interfaces",
    "/etc/mysql/my.cnf",
    "/etc/nginx/nginx.conf",
    "/etc/apache2/apache2.conf",
    "/var/www/html/",
    "/tmp/"
};

const int count = sizeof(paths) / sizeof(paths[0x0]);