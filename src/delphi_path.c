#include "../include/delphi_path.h"

const char *delphi_paths[] = {
    "/datasnap/rest/",
    "/datasnap/info",
    "/ems/",
    "/emsserver/",
    "/intraweb/",
    "/iw/",
    "/soap/Service.wsdl",
    "/files/",
    "/upload/",
    "/debug/",
    "/staging/",
    "/emsserver.ini",
    "/datasnap.ini"
};


const int delphi_count = sizeof(delphi_paths) / sizeof(delphi_paths[0x0]);