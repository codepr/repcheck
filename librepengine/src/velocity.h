#ifndef VELOCITY_H
#define VELOCITY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define VELOCITY_BUFFER_SIZE 10

typedef struct {
    float    angles[VELOCITY_BUFFER_SIZE];
    uint64_t timestamps[VELOCITY_BUFFER_SIZE];
    int      index;
    int      count;
    float    min_angle;
    float    max_angle;
} Velocity_Tracker;

void  velocity_tracker_init(Velocity_Tracker *tracker);
void  velocity_tracker_push(Velocity_Tracker *tracker, float angle,
                            uint64_t timestamp_ms);
float velocity_tracker_compute_peak_concentric(Velocity_Tracker *tracker);
float velocity_tracker_compute_avg_velocity(Velocity_Tracker *tracker);
int   velocity_tracker_is_ready(Velocity_Tracker *tracker);

#ifdef __cplusplus
}
#endif

#endif
