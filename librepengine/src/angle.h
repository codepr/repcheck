#ifndef ANGLE_H
#define ANGLE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    float x;
    float y;
    float z;
} Rep_Landmark;

/**
 * Calculate angle at point B formed by points A-B-C
 * @param A First landmark
 * @param B Vertex landmark (angle measured here)
 * @param C Second landmark
 * @return Angle in degrees [0, 180]
 */
float angle_at_point(Rep_Landmark A, Rep_Landmark B, Rep_Landmark C);
float angle_compute_elbow(float shoulder_y, float elbow_y, float wrist_y);
float angle_clamp(float angle);

#ifdef __cplusplus
}
#endif

#endif
