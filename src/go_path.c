#include "../include/go_path.h"

const char *go_paths[] = {
    "/debug/pprof/",
    "/debug/pprof/cmdline",
    "/debug/pprof/profile",
    "/debug/pprof/symbol",
    "/debug/pprof/trace",
    "/swagger/",
    "/swagger/index.html",
    "/swagger/doc.json",
    "/api/v1/",
    "/api/v2/",
    "/admin/",
    "/login",
    "/logout",
    "/register",
    "/reset-password",
    "/change-password",
    "/config.yaml",
    "/config.json",
    "/.env",
    "/logs/",
    "/backup/",
    "/tmp/",
    "/health",
    "/metrics",
    "/version"
};

const int go_count = sizeof(go_paths) / sizeof(go_paths[0]);
