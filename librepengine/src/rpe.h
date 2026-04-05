#ifndef RPE_H
#define RPE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    float peak_concentric_velocity;
    float avg_velocity;
    int   reps_completed;
    float rpe_estimate;
    int   is_valid;
} Rpe_Result;

typedef struct {
    float velocity_to_rpe_a;
    float velocity_to_rpe_b;
    float velocity_to_rpe_c;
    int   min_reps_for_rpe;
    float max_velocity_for_rpe;
} Rpe_Config;

void  rpe_config_init(Rpe_Config *config);
float rpe_estimate_from_velocity(float peak_velocity, float avg_velocity);
void  rpe_compute(Rpe_Result *result, const Rpe_Config *config,
                  float peak_velocity, float avg_velocity, int reps);
int   rpe_is_valid(const Rpe_Result *result);

#ifdef __cplusplus
}
#endif

#endif
