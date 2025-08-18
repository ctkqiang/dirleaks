#include "../include/python_path.h"

const char *python_paths[] = {
    "/settings.py",
    "/local_settings.py",
    "/config.py",
    "/requirements.txt",
    "/Pipfile",
    "/venv/",
    "/__pycache__/"
};

const int python_count = sizeof(python_paths) / sizeof(python_paths[0x0]);