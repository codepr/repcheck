#include "state_machine.h"
#include <string.h>

void rep_sm_init(Rep_Sm_State *state)
{
    if (!state) {
        return;
    }

    memset(state, 0, sizeof(Rep_Sm_State));
    state->threshold_high = DEFAULT_THRESHOLD_HIGH;
    state->threshold_low  = DEFAULT_THRESHOLD_LOW;
    state->debounce_ms    = DEFAULT_DEBOUNCE_MS;
    state->rep_count      = 0;
}

void rep_sm_set_thresholds(Rep_Sm_State *state, float high, float low)
{
    if (!state) {
        return;
    }
    state->threshold_high = high;
    state->threshold_low  = low;
}

void rep_sm_set_debounce(Rep_Sm_State *state, uint32_t ms)
{
    if (!state) {
        return;
    }
    state->debounce_ms = ms;
}

int rep_sm_process(Rep_Sm_State *state, float elbow_angle,
                   uint64_t timestamp_ms)
{
    if (!state) {
        return 0;
    }

    int rep_detected = 0;

    switch (state->current_state) {
    case REP_SM_STATE_UP:
        if (elbow_angle <= state->threshold_low) {
            if (timestamp_ms - state->last_state_change_ms >=
                state->debounce_ms) {
                state->current_state        = REP_SM_STATE_DOWN;
                state->last_state_change_ms = timestamp_ms;
            }
        }
        break;

    case REP_SM_STATE_DOWN:
        if (elbow_angle >= state->threshold_high) {
            if (timestamp_ms - state->last_state_change_ms >=
                state->debounce_ms) {
                state->current_state        = REP_SM_STATE_UP;
                state->last_state_change_ms = timestamp_ms;
                state->rep_count++;
                rep_detected = 1;
            }
        }
        break;

    default:
        state->current_state        = REP_SM_STATE_UP;
        state->last_state_change_ms = timestamp_ms;
        break;
    }

    return rep_detected;
}

int rep_sm_get_rep_count(const Rep_Sm_State *state)
{
    if (!state) {
        return 0;
    }
    return state->rep_count;
}

Rep_Sm_State_Type rep_sm_get_current_state(const Rep_Sm_State *state)
{
    if (!state) {
        return REP_SM_STATE_UP;
    }
    return state->current_state;
}
