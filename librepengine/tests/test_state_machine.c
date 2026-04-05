#include "state_machine.h"
#include <stdio.h>
#include <stdlib.h>

#define TEST_ASSERT(condition, message)                                        \
    if (!(condition)) {                                                        \
        printf("FAIL: %s - %s\n", __func__, message);                          \
        exit(1);                                                               \
    }

#define TEST_PASS() printf("PASS: %s\n", __func__)

void test_sm_init(void)
{
    Rep_Sm_State sm;
    rep_sm_init(&sm);

    TEST_ASSERT(sm.rep_count == 0, "Rep count should be 0");
    TEST_ASSERT(sm.current_state == REP_SM_STATE_UP,
                "Initial state should be UP");
    TEST_ASSERT(sm.threshold_high == DEFAULT_THRESHOLD_HIGH,
                "Threshold high should be default");
    TEST_ASSERT(sm.threshold_low == DEFAULT_THRESHOLD_LOW,
                "Threshold low should be default");
    TEST_ASSERT(sm.debounce_ms == DEFAULT_DEBOUNCE_MS,
                "Debounce should be default");
    TEST_PASS();
}

void test_sm_process_rep(void)
{
    Rep_Sm_State sm;
    rep_sm_init(&sm);
    sm.debounce_ms = 0;
    rep_sm_set_thresholds(&sm, 180.0f, 140.0f);

    int rep1 = rep_sm_process(&sm, 130.0f, 1000);
    TEST_ASSERT(rep1 == 0, "First transition should not count rep");
    TEST_ASSERT(sm.current_state == REP_SM_STATE_DOWN,
                "Should be in DOWN state");

    int rep2 = rep_sm_process(&sm, 180.0f, 1500);
    TEST_ASSERT(rep2 == 1, "Second transition should count rep");
    TEST_ASSERT(sm.rep_count == 1, "Rep count should be 1");
    TEST_ASSERT(sm.current_state == REP_SM_STATE_UP,
                "Should be back in UP state");

    TEST_PASS();
}

void test_sm_debounce(void)
{
    Rep_Sm_State sm;
    rep_sm_init(&sm);
    sm.debounce_ms = 500;
    rep_sm_set_thresholds(&sm, 180.0f, 140.0f);

    rep_sm_process(&sm, 130.0f, 1000);
    TEST_ASSERT(sm.current_state == REP_SM_STATE_DOWN,
                "Should transition to DOWN");

    int rep = rep_sm_process(&sm, 180.0f, 1200);
    TEST_ASSERT(rep == 0, "Should not count rep within debounce period");
    TEST_ASSERT(sm.current_state == REP_SM_STATE_DOWN,
                "Should remain in DOWN state");

    rep = rep_sm_process(&sm, 180.0f, 1800);
    TEST_ASSERT(rep == 1, "Should count rep after debounce period");

    TEST_PASS();
}

void test_sm_get_count(void)
{
    Rep_Sm_State sm;
    rep_sm_init(&sm);
    sm.debounce_ms = 0;
    rep_sm_set_thresholds(&sm, 180.0f, 140.0f);

    rep_sm_process(&sm, 130.0f, 1000);
    rep_sm_process(&sm, 180.0f, 1100);
    rep_sm_process(&sm, 130.0f, 1200);
    rep_sm_process(&sm, 180.0f, 1300);

    TEST_ASSERT(rep_sm_get_rep_count(&sm) == 2, "Should have 2 reps");
    TEST_PASS();
}

int main(void)
{
    printf("Running state machine tests...\n");

    test_sm_init();
    test_sm_process_rep();
    test_sm_debounce();
    test_sm_get_count();

    printf("All state machine tests passed!\n");
    return 0;
}
