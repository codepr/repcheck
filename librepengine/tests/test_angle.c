#include "angle.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define TEST_ASSERT_FLOAT_EQ(expected, actual, epsilon)                        \
    if (fabsf((expected) - (actual)) > (epsilon)) {                            \
        printf("FAIL: angle_at_point - Expected %f, got %f\n", (expected),     \
               (actual));                                                      \
        exit(1);                                                               \
    }

#define TEST_PASS() printf("PASS: %s\n", __func__)

void test_angle_at_point_straight_line(void)
{
    Rep_Landmark a     = {0.0f, 0.0f, 0.0f};
    Rep_Landmark b     = {0.5f, 0.0f, 0.0f};
    Rep_Landmark c     = {1.0f, 0.0f, 0.0f};

    float        angle = angle_at_point(a, b, c);
    TEST_ASSERT_FLOAT_EQ(180.0f, angle, 0.1f);
    TEST_PASS();
}

void test_angle_at_point_right_angle(void)
{
    Rep_Landmark a     = {0.0f, 1.0f, 0.0f};
    Rep_Landmark b     = {0.0f, 0.0f, 0.0f};
    Rep_Landmark c     = {1.0f, 0.0f, 0.0f};

    float        angle = angle_at_point(a, b, c);
    TEST_ASSERT_FLOAT_EQ(90.0f, angle, 1.0f);
    TEST_PASS();
}

void test_angle_at_point_perfect_bend(void)
{
    Rep_Landmark a     = {0.0f, 0.0f, 0.0f};
    Rep_Landmark b     = {0.0f, 1.0f, 0.0f};
    Rep_Landmark c     = {0.0f, 2.0f, 0.0f};

    float        angle = angle_at_point(a, b, c);
    TEST_ASSERT_FLOAT_EQ(180.0f, angle, 0.1f);
    TEST_PASS();
}

void test_angle_clamp(void)
{
    TEST_ASSERT_FLOAT_EQ(0.0f, angle_clamp(-10.0f), 0.01f);
    TEST_ASSERT_FLOAT_EQ(180.0f, angle_clamp(200.0f), 0.01f);
    TEST_ASSERT_FLOAT_EQ(90.0f, angle_clamp(90.0f), 0.01f);
    TEST_PASS();
}

int main(void)
{
    printf("Running angle tests...\n");

    test_angle_at_point_straight_line();
    test_angle_at_point_right_angle();
    test_angle_at_point_perfect_bend();
    test_angle_clamp();

    printf("All angle tests passed!\n");
    return 0;
}
