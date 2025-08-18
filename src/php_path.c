#include "../include/php_path.h"

const char *paths[] = {
    "/.env",
    "/config.php",
    "/db.php",
    "/wp-config.php",
    "/configuration.php",
    "/application/config/config.php",
    "/app/etc/env.php",
    "/storage/logs/laravel.log",
    "/composer.json",
    "/vendor/"
};

const int count = sizeof(paths) / sizeof(paths[0x0]);