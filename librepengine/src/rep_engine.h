#ifndef REP_ENGINE_H
#define REP_ENGINE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "angle.h"
#include "form.h"
#include "log.h"
#include "rpe.h"
#include "state_machine.h"
#include "velocity.h"
#include <stdint.h>

typedef struct {
    Rep_Sm_State     sm;
    Velocity_Tracker velocity;
    Rpe_Config       rpe_config;
    Rpe_Result       rpe_result;
    float            last_elbow_angle;
    int              initialization_frames;
    uint64_t         session_start_ms;
} Rep_Engine;

#define REP_ENGINE_DEFAULT_FPS 30
#define INITIALIZATION_FRAMES  15

void              rep_engine_init(Rep_Engine *engine);
void              rep_engine_process_frame(Rep_Engine           *engine,
                                           const Pose_Landmarks *landmarks,
                                           uint64_t              timestamp_ms);
void              rep_engine_reset(Rep_Engine *engine);
int               rep_engine_get_rep_count(const Rep_Engine *engine);
Rep_Sm_State_Type rep_engine_get_current_state(const Rep_Engine *engine);
float             rep_engine_get_last_angle(const Rep_Engine *engine);
int         rep_engine_get_initialization_progress(const Rep_Engine *engine);
Rpe_Result  rep_engine_get_rpe_result(const Rep_Engine *engine);
Form_Result rep_engine_get_form_result(const Rep_Engine *engine);
void rep_engine_set_thresholds(Rep_Engine *engine, float high, float low);

#ifdef __cplusplus
}
#endif

#endif
