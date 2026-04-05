# Add project specific ProGuard rules here.
# You can control the set of applied configuration files using the
# proguardFiles setting in build.gradle.

# Keep JNI native methods
-keepclassmembers class com.repcount.app.engine.RepEngine {
    public static long nativeCreate(int);
    public static void nativeProcessFrame(long, float[], float[], float[], float[]);
    public static void nativeDestroy(long);
}

# Keep C library
-keep class com.repcount.app.engine.** { *; }
