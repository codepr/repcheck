#ifndef FORM_H
#define FORM_H

#ifdef __cplusplus
extern "C" {
#endif

#define FORM_ANGLE_MIN         10.0f
#define FORM_ANGLE_MAX         170.0f
#define FORM_QUALITY_THRESHOLD 0.7f

typedef struct {
    float shoulder_y;
    float elbow_y;
    float wrist_y;
    float hip_y;
    float knee_y;
} Pose_Landmarks;

typedef struct {
    float depth_ratio;
    float straight_line_score;
    int   is_good_form;
    float quality_score;
} Form_Result;

void  form_validate(Form_Result *result, const Pose_Landmarks *landmarks);
float form_compute_depth_ratio(const Pose_Landmarks *landmarks);
float form_compute_range_of_motion(float start_angle, float end_angle);
int   form_is_acceptable(const Form_Result *result);

#ifdef __cplusplus
}
#endif

#endif
