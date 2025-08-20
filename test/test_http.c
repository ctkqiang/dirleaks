#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "../src/http.c"

CURL *__wrap_curl_easy_init(void) {
    return (CURL *)mock();
}

CURLcode __wrap_curl_easy_setopt(CURL *curl, CURLoption option, ...) {
    return (CURLcode)mock();
}

CURLcode __wrap_curl_easy_perform(CURL *curl) {
    return (CURLcode)mock();
}

CURLcode __wrap_curl_easy_getinfo(CURL *curl, CURLINFO info, ...) {
    va_list args;
    va_start(args, info);
    long *response_code = va_arg(args, long *);
    *response_code = (long)mock();
    va_end(args);
    return CURLE_OK;
}

void __wrap_curl_easy_cleanup(CURL *curl) {
    mock();
}



static void test_http_check_200(void **state) {
    (void)state; 

    will_return(__wrap_curl_easy_init, (CURL *)0x123);
    will_return(__wrap_curl_easy_setopt, CURLE_OK);
    will_return(__wrap_curl_easy_setopt, CURLE_OK);
    will_return(__wrap_curl_easy_setopt, CURLE_OK);
    will_return(__wrap_curl_easy_perform, CURLE_OK);
    will_return(__wrap_curl_easy_getinfo, 200L);
    will_return(__wrap_curl_easy_cleanup, NULL);

    int status = http_check("http://example.com", "/test");
    assert_int_equal(status, 200);
}

// Test case for http_check when status is 404
static void test_http_check_404(void **state) {
    (void)state; // unused

    will_return(__wrap_curl_easy_init, (CURL *)0x123);
    will_return(__wrap_curl_easy_setopt, CURLE_OK);
    will_return(__wrap_curl_easy_setopt, CURLE_OK);
    will_return(__wrap_curl_easy_setopt, CURLE_OK);
    will_return(__wrap_curl_easy_perform, CURLE_OK);
    will_return(__wrap_curl_easy_getinfo, 404L);
    will_return(__wrap_curl_easy_cleanup, NULL);

    int status = http_check("http://example.com", "/nonexistent");
    assert_int_equal(status, 404);
}

// Test case for http_check when curl_easy_perform fails
static void test_http_check_perform_fail(void **state) {
    (void)state; // unused

    will_return(__wrap_curl_easy_init, (CURL *)0x123);
    will_return(__wrap_curl_easy_setopt, CURLE_OK);
    will_return(__wrap_curl_easy_setopt, CURLE_OK);
    will_return(__wrap_curl_easy_setopt, CURLE_OK);
    will_return(__wrap_curl_easy_perform, CURLE_COULDNT_CONNECT);
    will_return(__wrap_curl_easy_cleanup, NULL);

    int status = http_check("http://example.com", "/test");
    assert_int_equal(status, -1); // Expect -1 if curl_easy_perform fails
}

// Test case for http_check when curl_easy_init fails
static void test_http_check_init_fail(void **state) {
    (void)state; // unused

    will_return(__wrap_curl_easy_init, NULL);

    int status = http_check("http://example.com", "/test");
    assert_int_equal(status, -1); // Expect -1 if curl_easy_init fails
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_http_check_200),
        cmocka_unit_test(test_http_check_404),
        cmocka_unit_test(test_http_check_perform_fail),
        cmocka_unit_test(test_http_check_init_fail),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}