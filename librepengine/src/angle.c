#include "angle.h"
#include <math.h>

#define PI 3.14159265358979323846f

float angle_at_point(Rep_Landmark A, Rep_Landmark B, Rep_Landmark C)
{
    float ba_x   = A.x - B.x;
    float ba_y   = A.y - B.y;
    float ba_z   = A.z - B.z;

    float bc_x   = C.x - B.x;
    float bc_y   = C.y - B.y;
    float bc_z   = C.z - B.z;

    float dot    = ba_x * bc_x + ba_y * bc_y + ba_z * bc_z;
    float ba_len = sqrtf(ba_x * ba_x + ba_y * ba_y + ba_z * ba_z);
    float bc_len = sqrtf(bc_x * bc_x + bc_y * bc_y + bc_z * bc_z);

    if (ba_len < 1e-6f || bc_len < 1e-6f) {
        return 180.0f;
    }

    float cos_theta = dot / (ba_len * bc_len);

    if (cos_theta > 1.0f)
        cos_theta = 1.0f;
    if (cos_theta < -1.0f)
        cos_theta = -1.0f;

    float angle = acosf(cos_theta) * (180.0f / PI);
    return angle;
}

float angle_compute_elbow(float shoulder_y, float elbow_y, float wrist_y)
{
    float        shoulder_x = 0.0f;
    float        elbow_x    = 0.5f;
    float        wrist_x    = 1.0f;

    Rep_Landmark shoulder   = {shoulder_x, shoulder_y, 0.0f};
    Rep_Landmark elbow      = {elbow_x, elbow_y, 0.0f};
    Rep_Landmark wrist      = {wrist_x, wrist_y, 0.0f};

    return angle_at_point(shoulder, elbow, wrist);
}

float angle_clamp(float angle)
{
    if (angle < 0.0f)
        angle = 0.0f;
    if (angle > 180.0f)
        angle = 180.0f;
    return angle;
}
