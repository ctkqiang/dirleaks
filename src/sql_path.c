#include "../include/sql_path.h"

const char *sql_paths[] = {
    "/sql",
    "/sql.php",
    "/sql/index.php",
    ".sql",
    ".sql.php"
};

const int sql_count = sizeof(sql_paths) / sizeof(sql_paths[0x0]);