#include "../include/java_path.h"

const char *java_paths[] = {
    "/WEB-INF/",
    "/META-INF/",
    "/manager/html",
    "/admin-console",
    "/jmx-console",
    "/actuator",
    "/actuator/health",
    "/actuator/env",
    "/actuator/metrics",
    "/swagger-ui.html",
    "/openapi.json",
    "/api-docs",
    "/WEB-INF/web.xml",
    "/WEB-INF/classes/",
    "/application.properties",
    "/application.yml",
    "/logback.xml",
    "/log4j.properties",
    "/.mvn/",
    "/target/"
};

const int java_count = sizeof(java_paths) / sizeof(java_paths[0x0]);