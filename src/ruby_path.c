#include "../include/ruby_path.h"

const char *ruby_paths[] = {
    "/config/database.yml",
    "/config/secrets.yml",
    "/config/application.rb",
    "/config/routes.rb",
    "/log/",
    "/tmp/",
    "/public/",
    "/admin/",
    "/login",
    "/logout",
    "/register",
    "/reset-password",
    "/change-password",
    "/db/schema.rb",
    "/db/structure.sql",
    "/.git/",
    "/.env",
    "/health",
    "/metrics"
};

const int ruby_count = sizeof(ruby_paths) / sizeof(ruby_paths[0]);
