#include "velocity.h"
#include <math.h>
#include <string.h>

void velocity_tracker_init(Velocity_Tracker *tracker)
{
    if (!tracker) {
        return;
    }

    memset(tracker, 0, sizeof(Velocity_Tracker));
    tracker->index     = 0;
    tracker->count     = 0;
    tracker->min_angle = 180.0f;
    tracker->max_angle = 0.0f;
}

void velocity_tracker_push(Velocity_Tracker *tracker, float angle,
                           uint64_t timestamp_ms)
{
    if (!tracker) {
        return;
    }

    tracker->angles[tracker->index]     = angle;
    tracker->timestamps[tracker->index] = timestamp_ms;

    if (angle < tracker->min_angle) {
        tracker->min_angle = angle;
    }
    if (angle > tracker->max_angle) {
        tracker->max_angle = angle;
    }

    tracker->index = (tracker->index + 1) % VELOCITY_BUFFER_SIZE;
    if (tracker->count < VELOCITY_BUFFER_SIZE) {
        tracker->count++;
    }
}

float velocity_tracker_compute_peak_concentric(Velocity_Tracker *tracker)
{
    if (!tracker || tracker->count < 2) {
        return 0.0f;
    }

    float peak_velocity = 0.0f;
    int   start_idx = (tracker->index - tracker->count + VELOCITY_BUFFER_SIZE) %
                    VELOCITY_BUFFER_SIZE;

    for (int i = 0; i < tracker->count - 1; i++) {
        int   curr_idx = (start_idx + i) % VELOCITY_BUFFER_SIZE;
        int   next_idx = (curr_idx + 1) % VELOCITY_BUFFER_SIZE;

        float angle_diff =
            tracker->angles[curr_idx] - tracker->angles[next_idx];
        float time_diff_ms =
            tracker->timestamps[next_idx] - tracker->timestamps[curr_idx];

        if (time_diff_ms > 0 && angle_diff > 0) {
            float velocity = (angle_diff * 1000.0f) / time_diff_ms;
            if (velocity > peak_velocity) {
                peak_velocity = velocity;
            }
        }
    }

    return peak_velocity;
}

float velocity_tracker_compute_avg_velocity(Velocity_Tracker *tracker)
{
    if (!tracker || tracker->count < 2) {
        return 0.0f;
    }

    float total_velocity = 0.0f;
    int   valid_samples  = 0;
    int   start_idx = (tracker->index - tracker->count + VELOCITY_BUFFER_SIZE) %
                    VELOCITY_BUFFER_SIZE;

    for (int i = 0; i < tracker->count - 1; i++) {
        int   curr_idx = (start_idx + i) % VELOCITY_BUFFER_SIZE;
        int   next_idx = (curr_idx + 1) % VELOCITY_BUFFER_SIZE;

        float angle_diff =
            fabsf(tracker->angles[curr_idx] - tracker->angles[next_idx]);
        float time_diff_ms =
            tracker->timestamps[next_idx] - tracker->timestamps[curr_idx];

        if (time_diff_ms > 0) {
            float velocity = (angle_diff * 1000.0f) / time_diff_ms;
            total_velocity += velocity;
            valid_samples++;
        }
    }

    return (valid_samples > 0) ? (total_velocity / valid_samples) : 0.0f;
}

int velocity_tracker_is_ready(Velocity_Tracker *tracker)
{
    if (!tracker) {
        return 0;
    }
    return tracker->count >= VELOCITY_BUFFER_SIZE;
}
