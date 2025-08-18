#include "../include/linux_path.h"

const char *linux_paths[] = {
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

const int linux_count = sizeof(linux_paths) / sizeof(linux_paths[0x0]);