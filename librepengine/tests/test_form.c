#include "form.h"
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

void test_form_compute_depth_ratio()
{
    Pose_Landmarks landmarks = {.shoulder_y = 0.4f,
                                .elbow_y    = 0.5f,
                                .wrist_y    = 0.6f,
                                .hip_y      = 0.3f,
                                .knee_y     = 0.35f};

    float          ratio     = form_compute_depth_ratio(&landmarks);
    printf("DEBUG: depth_ratio=%f, shoulder_to_wrist=%f, range=%f\n", ratio,
           fabsf(landmarks.shoulder_y - landmarks.wrist_y),
           (landmarks.elbow_y > landmarks.shoulder_y ? landmarks.elbow_y
                                                     : landmarks.shoulder_y) -
               (landmarks.elbow_y < landmarks.shoulder_y
                    ? landmarks.elbow_y
                    : landmarks.shoulder_y));
    TEST_ASSERT(ratio > 0.0f);
    TEST_ASSERT(ratio < 1.0f);
    printf("PASS: test_form_compute_depth_ratio (ratio=%.3f)\n", ratio);
}

void test_form_compute_range_of_motion()
{
    float rom = form_compute_range_of_motion(170.0f, 150.0f);
    TEST_ASSERT_FLOAT_NEAR(20.0f, rom, 0.1f);
    printf("PASS: test_form_compute_range_of_motion (rom=%.1f°)\n", rom);
}

void test_form_validate()
{
    Pose_Landmarks good_form = {.shoulder_y = 0.4f,
                                .elbow_y    = 0.5f,
                                .wrist_y    = 0.6f,
                                .hip_y      = 0.3f,
                                .knee_y     = 0.35f};

    Form_Result    result;
    form_validate(&result, &good_form);

    TEST_ASSERT(result.is_good_form == 1);
    TEST_ASSERT(result.quality_score > FORM_QUALITY_THRESHOLD);
    printf("PASS: test_form_validate (quality=%.2f)\n", result.quality_score);
}

void test_form_is_acceptable()
{
    Form_Result good_result = {.depth_ratio         = 0.3f,
                               .straight_line_score = 0.85f,
                               .is_good_form        = 1,
                               .quality_score       = 0.8f};

    Form_Result bad_result  = {.depth_ratio         = 0.8f,
                               .straight_line_score = 0.4f,
                               .is_good_form        = 0,
                               .quality_score       = 0.5f};

    TEST_ASSERT(form_is_acceptable(&good_result) == 1);
    TEST_ASSERT(form_is_acceptable(&bad_result) == 0);
    printf("PASS: test_form_is_acceptable\n");
}

int main()
{
    printf("Running form validation tests...\n\n");

    test_form_compute_depth_ratio();
    test_form_compute_range_of_motion();
    test_form_validate();
    test_form_is_acceptable();

    printf("\nAll form validation tests passed!\n");
    return 0;
}
