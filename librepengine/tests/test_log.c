#include <stdio.h>
#include <setjmp.h>
#include <stdlib.h>
#include "log.h"

#define TEST_ASSERT(condition, message) \
    if (!(condition)) { \
        printf("FAIL: %s at %s:%d\n", message, __FILE__, __LINE__); \
        exit(1); \
    }

#define TEST_PASS() printf("PASS: %s\n", __func__)

void test_log_init(void) {
    log_init();
    TEST_PASS();
}

void test_log_set_level(void) {
    log_init();
    log_set_level(LOG_LEVEL_DEBUG);
    TEST_ASSERT(log_get_level() == LOG_LEVEL_DEBUG, "Log level should be DEBUG");
    TEST_PASS();
}

void test_log_set_output(void) {
    FILE* stream = fopen("/dev/null", "w");
    TEST_ASSERT(stream != NULL, "Failed to open /dev/null");
    
    log_set_output(stream);
    TEST_PASS();
    
    fclose(stream);
}

int main(void) {
    printf("Running log tests...\n");
    
    test_log_init();
    test_log_set_level();
    test_log_set_output();
    
    printf("All log tests passed!\n");
    return 0;
}
