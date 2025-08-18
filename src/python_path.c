#include "../include/python_path.h"

const char *paths[] = {
    "/settings.py",
    "/local_settings.py",
    "/config.py",
    "/requirements.txt",
    "/Pipfile",
    "/venv/",
    "/__pycache__/"
};

const int count = sizeof(paths) / sizeof(paths[0x0]);