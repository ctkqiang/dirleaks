#include <stdio.h>
#include <curl/curl.h>

#include "../include/http.h"

int http_check(const char *base_url, const char *path) {
    CURL *curl;
    CURLcode res;

    long response_code = -1;
    char full_url[1024];

    snprintf(full_url, sizeof(full_url), "%s%s", base_url, path);

    curl = curl_easy_init();

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, full_url);
        curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        res = curl_easy_perform(curl);
        if (res == CURLE_OK) {
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
        }
        curl_easy_cleanup(curl);
    }

    return (int)response_code;
}
