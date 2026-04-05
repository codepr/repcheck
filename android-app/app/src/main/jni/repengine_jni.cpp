#include "rep_engine.h"
#include <jni.h>
#include <string>

// Helper: Convert Java float array to C array
static float *getFloatArray(JNIEnv *env, jobjectArray array, jsize *size)
{
    *size = env->GetArrayLength(array);
    if (*size == 0)
        return nullptr;

    float *data = (float *)malloc(*size * sizeof(float));
    if (data) {
        env->GetFloatArrayRegion(array, 0, *size, data);
    }
    return data;
}

// Helper: Convert C array to Java float array
static jobjectArray floatArrayToJava(JNIEnv *env, float *data, jsize length)
{
    if (!data || length <= 0)
        return nullptr;

    jobjectArray result = env->NewFloatArray(length);
    if (result) {
        env->SetFloatArrayRegion(result, 0, length, data);
    }
    return result;
}

// ============================================================================
// Native Methods for RepEngine
// ============================================================================

extern "C" {

JNIEXPORT jlong JNICALL Java_com_repcount_app_engine_RepEngine_nativeCreate(
    JNIEnv *env, jclass, jint exerciseType)
{
    RE_ExerciseType type = static_cast<RE_ExerciseType>(exerciseType);
    RE_Context     *ctx  = re_create(type);
    return reinterpret_cast<jlong>(ctx);
}

JNIEXPORT void JNICALL Java_com_repcount_app_engine_RepEngine_nativeDestroy(
    JNIEnv *env, jclass, jlong ctx)
{
    if (ctx != 0) {
        re_destroy(reinterpret_cast<RE_Context *>(ctx));
    }
}

JNIEXPORT jfloatArray JNICALL
Java_com_repcount_app_engine_RepEngine_nativeProcessFrame(
    JNIEnv *env, jclass, jlong ctx, jfloatArray xCoords, jfloatArray yCoords,
    jfloatArray zCoords, jfloat timestamp)
{
    if (ctx == 0) {
        return nullptr;
    }

    jsize  landmarkCount = 0;
    float *xData         = getFloatArray(env, xCoords, &landmarkCount);
    float *yData         = getFloatArray(env, yCoords, &landmarkCount);
    float *zData         = getFloatArray(env, zCoords, &landmarkCount);

    if (!xData || !yData || !zData || landmarkCount == 0) {
        free(xData);
        free(yData);
        free(zData);
        return nullptr;
    }

    // Create landmark array
    RE_Landmark *landmarks =
        (RE_Landmark *)malloc(landmarkCount * sizeof(RE_Landmark));
    for (jsize i = 0; i < landmarkCount; i++) {
        landmarks[i].x = xData[i];
        landmarks[i].y = yData[i];
        landmarks[i].z = zData[i];
    }

    // Process frame
    RE_Context *context = reinterpret_cast<RE_Context *>(ctx);
    RE_Output   output  = {0, 0.0f, 0.0f, 0};
    re_process_frame(context, landmarks, landmarkCount, timestamp, &output);

    // Free memory
    free(landmarks);
    free(xData);
    free(yData);
    free(zData);

    // Return result as float array: [rep_count, velocity, rpe, form_ok]
    float result[4] = {static_cast<float>(output.rep_count),
                       output.last_rep_velocity, output.estimated_rpe,
                       static_cast<float>(output.form_ok)};

    return floatArrayToJava(env, result, 4);
}

} // extern "C"
