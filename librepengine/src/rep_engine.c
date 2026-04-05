#include "rep_engine.h"
#include "log.h"
#include <stdlib.h>
#include <string.h>

void rep_engine_init(Rep_Engine *engine)
{
    if (!engine) {
        return;
    }

    memset(engine, 0, sizeof(Rep_Engine));

    rep_sm_init(&engine->sm);
    velocity_tracker_init(&engine->velocity);
    rpe_config_init(&engine->rpe_config);

    engine->initialization_frames = 0;
    engine->session_start_ms      = 0;
    engine->last_elbow_angle      = 180.0f;
}

void rep_engine_process_frame(Rep_Engine           *engine,
                              const Pose_Landmarks *landmarks,
                              uint64_t              timestamp_ms)
{
    if (!engine || !landmarks) {
        return;
    }

    if (engine->session_start_ms == 0) {
        engine->session_start_ms = timestamp_ms;
    }

    float elbow_angle = angle_compute_elbow(
        landmarks->shoulder_y, landmarks->elbow_y, landmarks->wrist_y);
    elbow_angle = angle_clamp(elbow_angle);

    velocity_tracker_push(&engine->velocity, elbow_angle, timestamp_ms);

    if (engine->initialization_frames < INITIALIZATION_FRAMES) {
        engine->initialization_frames++;
        engine->last_elbow_angle = elbow_angle;
        return;
    }

    int rep_detected = rep_sm_process(&engine->sm, elbow_angle, timestamp_ms);

    if (rep_detected) {
        LOG_INFO("Rep detected! Total: %d", rep_sm_get_rep_count(&engine->sm));

        int   current_reps = rep_sm_get_rep_count(&engine->sm);
        float peak_vel =
            velocity_tracker_compute_peak_concentric(&engine->velocity);
        float avg_vel =
            velocity_tracker_compute_avg_velocity(&engine->velocity);

        rpe_compute(&engine->rpe_result, &engine->rpe_config, peak_vel, avg_vel,
                    current_reps);
    }

    engine->last_elbow_angle = elbow_angle;
}

void rep_engine_reset(Rep_Engine *engine)
{
    if (!engine) {
        return;
    }

    rep_sm_init(&engine->sm);
    velocity_tracker_init(&engine->velocity);
    memset(&engine->rpe_result, 0, sizeof(Rpe_Result));
    engine->initialization_frames = 0;
    engine->session_start_ms      = 0;
    engine->last_elbow_angle      = 180.0f;
}

int rep_engine_get_rep_count(const Rep_Engine *engine)
{
    if (!engine) {
        return 0;
    }
    return rep_sm_get_rep_count(&engine->sm);
}

Rep_Sm_State_Type rep_engine_get_current_state(const Rep_Engine *engine)
{
    if (!engine) {
        return REP_SM_STATE_UP;
    }
    return rep_sm_get_current_state(&engine->sm);
}

float rep_engine_get_last_angle(const Rep_Engine *engine)
{
    if (!engine) {
        return 180.0f;
    }
    return engine->last_elbow_angle;
}

int rep_engine_get_initialization_progress(const Rep_Engine *engine)
{
    if (!engine) {
        return 0;
    }
    return engine->initialization_frames;
}

Rpe_Result rep_engine_get_rpe_result(const Rep_Engine *engine)
{
    if (!engine) {
        Rpe_Result empty = {0};
        return empty;
    }
    return engine->rpe_result;
}

Form_Result rep_engine_get_form_result(const Rep_Engine *engine)
{
    Form_Result result = {0};
    if (engine) {
        Pose_Landmarks empty_landmarks = {0};
        form_validate(&result, &empty_landmarks);
    }
    return result;
}

void rep_engine_set_thresholds(Rep_Engine *engine, float high, float low)
{
    if (!engine) {
        return;
    }
    rep_sm_set_thresholds(&engine->sm, high, low);
}
