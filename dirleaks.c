#include <stdio.h>

#include "include/tui.h"
#include "include/scanner.h"

#include "include/csharp_path.h"
#include "include/docker_path.h"
#include "include/generic_path.h"
#include "include/java_path.h"
#include "include/linux_path.h"
#include "include/php_path.h"
#include "include/python_path.h"
#include "include/windows_path.h"


int main() {
    char url[256];
    int choice = show_menu();
    ask_url(url, sizeof(url));

    if (choice == 0 || choice == 1) scan_group("CSharp", csharp_paths, csharp_count, url);
    if (choice == 0 || choice == 2) scan_group("Docker", docker_paths, docker_count, url);
    if (choice == 0 || choice == 3) scan_group("Generic", generic_paths, generic_count, url);
    if (choice == 0 || choice == 4) scan_group("Java", java_paths, java_count, url);
    if (choice == 0 || choice == 5) scan_group("Linux", linux_paths, linux_count, url);
    if (choice == 0 || choice == 6) scan_group("PHP", php_paths, php_count, url);
    if (choice == 0 || choice == 7) scan_group("Python", python_paths, python_count, url);
    if (choice == 0 || choice == 8) scan_group("Windows", windows_paths, windows_count, url);

    return 0;
}
