#include "form.h"
#include <math.h>

void form_validate(Form_Result *result, const Pose_Landmarks *landmarks)
{
    if (!result || !landmarks) {
        return;
    }

    result->depth_ratio = form_compute_depth_ratio(landmarks);

    float shoulder_to_wrist_dy =
        fabsf(landmarks->shoulder_y - landmarks->wrist_y);
    float elbow_to_hip_dy     = fabsf(landmarks->elbow_y - landmarks->hip_y);

    float straight_line_score = 1.0f;
    if (shoulder_to_wrist_dy > 0.01f) {
        float ideal_ratio   = elbow_to_hip_dy / shoulder_to_wrist_dy;
        float deviation     = fabsf(ideal_ratio - 1.0f);
        straight_line_score = 1.0f - (deviation * 0.5f);
    }
    result->straight_line_score = fmaxf(0.0f, fminf(1.0f, straight_line_score));

    result->quality_score =
        (result->depth_ratio + result->straight_line_score) / 2.0f;
    result->is_good_form =
        (result->quality_score >= FORM_QUALITY_THRESHOLD) ? 1 : 0;
}

float form_compute_depth_ratio(const Pose_Landmarks *landmarks)
{
    if (!landmarks) {
        return 0.0f;
    }

    float shoulder_y = landmarks->shoulder_y;
    float wrist_y    = landmarks->wrist_y;
    float elbow_y    = landmarks->elbow_y;

    float full_range = fabsf(shoulder_y - wrist_y);
    if (full_range < 0.001f) {
        return 0.0f;
    }

    float elbow_displacement = fabsf(shoulder_y - elbow_y);
    return elbow_displacement / full_range;
}

float form_compute_range_of_motion(float start_angle, float end_angle)
{
    return fabsf(start_angle - end_angle);
}

int form_is_acceptable(const Form_Result *result)
{
    if (!result) {
        return 0;
    }
    return result->is_good_form;
}
