#include <stdio.h>
#include <stdlib.h>
#include "rpe.h"

#define TEST_PASS() printf("PASS: %s\n", __func__)

void test_rpe_compute(void) {
    float rpe = rpe_estimate_from_velocity(2.5f, 1.5f);
    if (rpe < 1.0f || rpe > 10.0f) {
        printf("FAIL: rpe_compute_from_velocity - RPE out of range\n");
        exit(1);
    }
    TEST_PASS();
}

void test_rpe_low_velocity(void) {
    float rpe = rpe_estimate_from_velocity(0.1f, 0.05f);
    if (rpe < 1.0f || rpe > 3.0f) {
        printf("FAIL: rpe_low_velocity - Expected low RPE, got %f\n", rpe);
        exit(1);
    }
    TEST_PASS();
}

void test_rpe_high_velocity(void) {
    float rpe = rpe_estimate_from_velocity(5.0f, 3.0f);
    if (rpe < 8.0f || rpe > 10.0f) {
        printf("FAIL: rpe_high_velocity - Expected high RPE, got %f\n", rpe);
        exit(1);
    }
    TEST_PASS();
}

int main(void) {
    printf("Running RPE tests...\n");
    
    test_rpe_compute();
    test_rpe_low_velocity();
    test_rpe_high_velocity();
    
    printf("All RPE tests passed!\n");
    return 0;
}
