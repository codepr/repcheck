#include "velocity.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define TEST_ASSERT_FLOAT_EQ(expected, actual, epsilon)                        \
    do {                                                                       \
        if (fabsf((expected) - (actual)) > (epsilon)) {                        \
            printf("FAIL: %s - Expected %f, got %f\n", __func__, (expected),   \
                   (actual));                                                  \
            exit(1);                                                           \
        }                                                                      \
    } while (0)

#define TEST_ASSERT_TRUE(cond)                                                 \
    do {                                                                       \
        if (!(cond)) {                                                         \
            printf("FAIL: %s - Expected true\n", __func__);                    \
            exit(1);                                                           \
        }                                                                      \
    } while (0)

#define TEST_ASSERT_FALSE(cond)                                                \
    do {                                                                       \
        if (cond) {                                                            \
            printf("FAIL: %s - Expected false\n", __func__);                   \
            exit(1);                                                           \
        }                                                                      \
    } while (0)

void test_velocity_tracker_init()
{
    Velocity_Tracker tracker;
    velocity_tracker_init(&tracker);

    TEST_ASSERT_TRUE(tracker.count == 0);
    TEST_ASSERT_TRUE(tracker.index == 0);
    TEST_ASSERT_TRUE(tracker.min_angle == 180.0f);
    TEST_ASSERT_TRUE(tracker.max_angle == 0.0f);
    printf("PASS: test_velocity_tracker_init\n");
}

void test_velocity_tracker_push()
{
    Velocity_Tracker tracker;
    velocity_tracker_init(&tracker);

    uint64_t base_time = 1000;
    velocity_tracker_push(&tracker, 170.0f, base_time);
    velocity_tracker_push(&tracker, 160.0f, base_time + 100);
    velocity_tracker_push(&tracker, 150.0f, base_time + 200);

    TEST_ASSERT_TRUE(tracker.count == 3);
    TEST_ASSERT_TRUE(tracker.min_angle == 150.0f);
    TEST_ASSERT_TRUE(tracker.max_angle == 170.0f);
    printf("PASS: test_velocity_tracker_push\n");
}

void test_velocity_tracker_compute_peak_concentric()
{
    Velocity_Tracker tracker;
    velocity_tracker_init(&tracker);

    uint64_t base_time = 1000;
    velocity_tracker_push(&tracker, 170.0f, base_time + 0);   // Top position
    velocity_tracker_push(&tracker, 160.0f, base_time + 100); // Going down
    velocity_tracker_push(&tracker, 150.0f, base_time + 200); // Bottom
    velocity_tracker_push(&tracker, 155.0f,
                          base_time + 300); // Going up (concentric)
    velocity_tracker_push(&tracker, 165.0f,
                          base_time + 400); // Going up (concentric)
    velocity_tracker_push(&tracker, 170.0f, base_time + 500); // Top

    float peak_velocity = velocity_tracker_compute_peak_concentric(&tracker);
    TEST_ASSERT_TRUE(peak_velocity > 0.0f);
    printf("PASS: test_velocity_tracker_compute_peak_concentric (peak=%.2f "
           "deg/s)\n",
           peak_velocity);
}

void test_velocity_tracker_compute_avg_velocity()
{
    Velocity_Tracker tracker;
    velocity_tracker_init(&tracker);

    uint64_t base_time = 1000;
    velocity_tracker_push(&tracker, 170.0f, base_time + 0);
    velocity_tracker_push(&tracker, 150.0f, base_time + 200);
    velocity_tracker_push(&tracker, 170.0f, base_time + 400);

    float avg_velocity = velocity_tracker_compute_avg_velocity(&tracker);
    TEST_ASSERT_TRUE(avg_velocity > 0.0f);
    printf(
        "PASS: test_velocity_tracker_compute_avg_velocity (avg=%.2f deg/s)\n",
        avg_velocity);
}

void test_velocity_tracker_is_ready()
{
    Velocity_Tracker tracker;
    velocity_tracker_init(&tracker);

    TEST_ASSERT_FALSE(velocity_tracker_is_ready(&tracker));

    for (int i = 0; i < VELOCITY_BUFFER_SIZE; i++) {
        velocity_tracker_push(&tracker, 170.0f - (i * 5.0f), 1000 + (i * 100));
    }

    TEST_ASSERT_TRUE(velocity_tracker_is_ready(&tracker));
    printf("PASS: test_velocity_tracker_is_ready\n");
}

int main()
{
    printf("Running velocity tracker tests...\n\n");

    test_velocity_tracker_init();
    test_velocity_tracker_push();
    test_velocity_tracker_compute_peak_concentric();
    test_velocity_tracker_compute_avg_velocity();
    test_velocity_tracker_is_ready();

    printf("\nAll velocity tracker tests passed!\n");
    return 0;
}
