#include "rep_engine.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define TEST_ASSERT(cond)                                                      \
    do {                                                                       \
        if (!(cond)) {                                                         \
            printf("FAIL: %s - Assertion failed: %s\n", __func__, #cond);      \
            exit(1);                                                           \
        }                                                                      \
    } while (0)

#define TEST_ASSERT_FLOAT_NEAR(expected, actual, epsilon)                      \
    do {                                                                       \
        if (fabsf((expected) - (actual)) > (epsilon)) {                        \
            printf("FAIL: %s - Expected %f, got %f\n", __func__, (expected),   \
                   (actual));                                                  \
            exit(1);                                                           \
        }                                                                      \
    } while (0)

void test_rep_engine_init()
{
    Rep_Engine engine;
    rep_engine_init(&engine);

    TEST_ASSERT(rep_engine_get_rep_count(&engine) == 0);
    TEST_ASSERT(rep_engine_get_current_state(&engine) == REP_SM_STATE_UP);
    TEST_ASSERT(rep_engine_get_initialization_progress(&engine) == 0);
    printf("PASS: test_rep_engine_init\n");
}

void test_rep_engine_process_frame()
{
    Rep_Engine engine;
    rep_engine_init(&engine);

    Pose_Landmarks landmarks = {0};

    for (int i = 0; i < INITIALIZATION_FRAMES + 10; i++) {
        landmarks.shoulder_y = 0.4f;
        landmarks.elbow_y    = 0.4f; // Arm extended (top position)
        landmarks.wrist_y    = 0.4f;
        landmarks.hip_y      = 0.5f;
        landmarks.knee_y     = 0.55f;

        rep_engine_process_frame(&engine, &landmarks, i * 33);
    }

    int init_progress = rep_engine_get_initialization_progress(&engine);
    TEST_ASSERT(init_progress >= 10);
    printf("PASS: test_rep_engine_process_frame (init_progress=%d)\n",
           init_progress);
}

void test_rep_engine_get_last_angle()
{
    Rep_Engine engine;
    rep_engine_init(&engine);

    Pose_Landmarks landmarks = {0};
    landmarks.shoulder_y     = 0.4f;
    landmarks.elbow_y        = 0.4f;
    landmarks.wrist_y        = 0.4f;
    landmarks.hip_y          = 0.5f;
    landmarks.knee_y         = 0.55f;

    for (int i = 0; i < 20; i++) {
        rep_engine_process_frame(&engine, &landmarks, i * 33);
    }

    float angle = rep_engine_get_last_angle(&engine);
    TEST_ASSERT(angle > 0.0f);
    TEST_ASSERT(angle < 180.0f);
    printf("PASS: test_rep_engine_get_last_angle (angle=%.1f°)\n", angle);
}

void test_rep_engine_get_rpe_result()
{
    Rep_Engine engine;
    rep_engine_init(&engine);

    rep_engine_process_frame(&engine, NULL, 0);

    Rpe_Result result = rep_engine_get_rpe_result(&engine);
    TEST_ASSERT(result.rpe_estimate >= 0.0f);
    TEST_ASSERT(result.rpe_estimate <= 10.0f);
    printf("PASS: test_rep_engine_get_rpe_result (rpe=%.1f/10)\n",
           result.rpe_estimate);
}

int main()
{
    printf("Running rep engine tests...\n\n");

    test_rep_engine_init();
    test_rep_engine_process_frame();
    test_rep_engine_get_last_angle();
    test_rep_engine_get_rpe_result();

    printf("\nAll rep engine tests passed!\n");
    return 0;
}
