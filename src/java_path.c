#include "../include/java_path.h"

const char *paths[] = {
    "/WEB-INF/web.xml",
    "/WEB-INF/classes/",
    "/application.properties",
    "/application.yml",
    "/logback.xml",
    "/log4j.properties",
    "/.mvn/",
    "/target/"
};

const int count = sizeof(paths) / sizeof(paths[0x0]);