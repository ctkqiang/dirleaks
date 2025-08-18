#include "../include/docker_path.h"

const char *docker_paths[] = {
    "/Dockerfile",
    "/docker-compose.yml",
    "/.dockerignore",
    "/var/lib/docker/volumes/",
    "/.docker/config.json"
};

const int docker_count = sizeof(docker_paths) / sizeof(docker_paths[0x0]);