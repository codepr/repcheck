#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef enum { REP_SM_STATE_UP = 0, REP_SM_STATE_DOWN = 1 } Rep_Sm_State_Type;

#define DEFAULT_THRESHOLD_HIGH 150.0f
#define DEFAULT_THRESHOLD_LOW  30.0f
#define DEFAULT_DEBOUNCE_MS    200

typedef struct {
    Rep_Sm_State_Type current_state;
    int               rep_count;
    uint64_t          last_state_change_ms;
    float             threshold_high;
    float             threshold_low;
    uint32_t          debounce_ms;
} Rep_Sm_State;

void rep_sm_init(Rep_Sm_State *state);
void rep_sm_set_thresholds(Rep_Sm_State *state, float high, float low);
void rep_sm_set_debounce(Rep_Sm_State *state, uint32_t ms);
int  rep_sm_process(Rep_Sm_State *state, float elbow_angle,
                    uint64_t timestamp_ms);
int  rep_sm_get_rep_count(const Rep_Sm_State *state);
Rep_Sm_State_Type rep_sm_get_current_state(const Rep_Sm_State *state);

#ifdef __cplusplus
}
#endif

#endif
