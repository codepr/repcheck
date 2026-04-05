# RepCount Android App

## Project Setup Complete ✅

### What's Been Created

**Project Structure:**
```
repcount-android/
├── app/
│   ├── src/main/
│   │   ├── java/com/repcount/app/
│   │   │   ├── camera/         # CameraX integration
│   │   │   ├── mediapipe/      # MediaPipe wrapper
│   │   │   ├── engine/         # C library JNI bindings
│   │   │   └── viewmodel/      # ViewModels
│   │   ├── jni/                # C++ JNI layer
│   │   └── res/                # Resources
│   └── build.gradle.kts
├── CMakeLists.txt              # NDK build config
├── build.gradle.kts            # Root Gradle
├── settings.gradle.kts         # Project settings
└── gradle.properties           # Gradle config
```

### Dependencies Configured

**Core:**
- ✅ CameraX 1.3.0 (camera preview + image analysis)
- ✅ MediaPipe Pose Detection 18.0.0 (pose tracking)
- ✅ Jetpack Compose 1.5.4 (UI)
- ✅ Kotlin 1.9.0
- ✅ Lifecycle ViewModel 2.6.1
- ✅ Coroutines 1.7.1

**NDK:**
- ✅ CMake 3.22.1
- ✅ NDK 25.1.8937393
- ✅ Links to librepengine.a (C library)

### Next Steps

1. **Generate Gradle files** (required for first build):
   ```bash
   cd repcount-android
   ./gradlew wrapper
   ```

2. **Create stub Kotlin files** for each module

3. **Implement the layers:**
   - Phase 1: CameraX + MediaPipe integration
   - Phase 2: JNI bridge layer
   - Phase 3: Kotlin wrapper for C engine
   - Phase 4: UI with Jetpack Compose

### Important Notes

- **Min SDK:** 24 (Android 7.0)
- **Target SDK:** 34 (Android 14)
- **ABI Filters:** armeabi-v7a, arm64-v8a, x86, x86_64
- **C Library:** Links to `../src/` (your existing C engine)

### Build Commands

```bash
# Generate Gradle wrapper
./gradlew wrapper

# Build debug
./gradlew assembleDebug

# Build release
./gradlew assembleRelease

# Run tests
./gradlew test

# Clean build
./gradlew clean
```

### Open Questions

1. Should we use Hilt for dependency injection, or manual DI?
2. Do you want a simple launcher icon or do you have assets?
3. Should we add permissions request UI or handle silently?

Ready for Phase 1 implementation?
