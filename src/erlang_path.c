#include "../include/erlang_path.h"

const char *erlang_paths[] = {
    "/rebar.config",
    "/rel/sys.config",
    "/rel/vm.args",
    "/logs/",
    "/tmp/",
    "/admin/",
    "/debug/",
    "/health",
    "/metrics",
    "/api/v1/",
    "/api/v2/",
    "/.env",
    "/.git/",
    "/config/config.exs",
    "/config/prod.exs"
};

const int erlang_count = sizeof(erlang_paths) / sizeof(erlang_paths[0]);
