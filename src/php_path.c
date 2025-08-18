#include "../include/php_path.h"

const char *php_paths[] = {
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

const int php_count = sizeof(php_paths) / sizeof(php_paths[0x0]);