#include "../include/docker_path.h"

const char *paths[] = {
    "/Dockerfile",
    "/docker-compose.yml",
    "/.dockerignore",
    "/var/lib/docker/volumes/",
    "/.docker/config.json"
};

const int count = sizeof(paths) / sizeof(paths[0x0]);