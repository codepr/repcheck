#include "rpe.h"
#include <math.h>

void rpe_config_init(Rpe_Config *config)
{
    if (!config) {
        return;
    }

    config->velocity_to_rpe_a    = 0.15f;
    config->velocity_to_rpe_b    = 2.5f;
    config->velocity_to_rpe_c    = 3.0f;
    config->min_reps_for_rpe     = 3;
    config->max_velocity_for_rpe = 500.0f;
}

float rpe_estimate_from_velocity(float peak_velocity, float avg_velocity)
{
    float velocity_factor = (peak_velocity + avg_velocity) / 2.0f;
    velocity_factor       = fminf(velocity_factor, 500.0f);

    float rpe             = 1.0f + (velocity_factor * 1.75f);
    rpe                   = fmaxf(1.0f, fminf(10.0f, rpe));

    return rpe;
}

void rpe_compute(Rpe_Result *result, const Rpe_Config *config,
                 float peak_velocity, float avg_velocity, int reps)
{
    if (!result || !config) {
        return;
    }

    result->peak_concentric_velocity = peak_velocity;
    result->avg_velocity             = avg_velocity;
    result->reps_completed           = reps;

    if (reps >= config->min_reps_for_rpe &&
        peak_velocity <= config->max_velocity_for_rpe) {
        result->rpe_estimate =
            rpe_estimate_from_velocity(peak_velocity, avg_velocity);
        result->is_valid = 1;
    } else {
        result->rpe_estimate = 0.0f;
        result->is_valid     = 0;
    }
}

int rpe_is_valid(const Rpe_Result *result)
{
    if (!result) {
        return 0;
    }
    return result->is_valid;
}
