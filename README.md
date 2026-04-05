# RepCheck

A mobile app for rep counting using computer vision and pose detection.
Ideally, through the smartphone camera, it would infer some basic info about
the rep quality during a workout (e.g. Squat - descend below parallel and
ascend to full lock to be considered valid, rep speed to infer RPE etc.)

### Agentic playground

This project is intended to try and evaluate local model capabilities for agentic development

- Qwen 3.5 35B - Q6_K and Qwen-Coder-Next at 4 bits quantization are the adopted models
- A human-written design file was provided with the main architecture and goals of the app.
- A human-written guide on style file was provided for the C part of the project. For the Kotlin on Android, a blend of AI and human has been produced given my limited knowledge of the language and architecture
- The implementation was performed with human feedbacks overseeing and steering certain design decisions. Almost no code was written by hand.

## Project Structure

```
repcheck/
├── librepengine/           # C library (standalone)
│   ├── src/                # Core implementation
│   ├── tests/              # Unit tests
│   ├── CMakeLists.txt      # CMake build config
│   ├── Makefile            # Make build config
│   └── README.md           # Library documentation
├── android-app/            # Android application
│   ├── app/                # App source code
│   ├── CMakeLists.txt      # Android NDK build config
│   ├── build.gradle.kts    # Gradle configuration
│   └── src/                # Android source
└── README.md               # This file
```

## Components

### 1. RepEngine C Library (`librepengine/`)

Portable C library for rep counting with:
- **Rep counting** via state machine
- **Angle computation** from pose landmarks
- **Velocity tracking** for RPE estimation
- **Form validation** and quality scoring
- **RPE calculation** based on rep velocity

**Key Features:**
- Deterministic behavior
- Real-time capable (30-60 FPS)
- Minimal memory allocations
- No platform dependencies
- Comprehensive unit tests

**Build:**
```bash
cd librepengine
make
make test
```

### 2. Android App (`android-app/`)

Native Android application using:
- **CameraX** for camera preview
- **MediaPipe** for pose detection (33 landmarks)
- **Jetpack Compose** for UI
- **RepEngine C library** via JNI for core logic

**Key Features:**
- Real-time rep counting
- RPE estimation
- Form feedback
- 30 FPS target

**Build:**
```bash
cd android-app
# Open in Android Studio
open android-app
```

## Architecture

```
Camera (Android)
    ↓
MediaPipe Pose (33 landmarks)
    ↓
JNI Bridge
    ↓
RepEngine C Library
    ↓
Reps + Velocity + RPE + Form
    ↓
Jetpack Compose UI
```

## Getting Started

### Prerequisites

**For C Library:**
- GCC with C11 support
- Make (optional)

**For Android App:**
- Android Studio Hedgehog (2023.1.1) or later
- Android SDK (API 24-34)
- NDK 25.1.8937393
- CMake 3.22.1+

### Build C Library

```bash
cd librepengine
make clean
make
make test
```

### Build Android App

1. Open `android-app/` in Android Studio
2. Wait for Gradle sync
3. Build → Build Bundle(s) / APK(s) → APK
4. Install on device

## Development Workflow

1. **Update C Engine:**
   - Make changes in `librepengine/src/`
   - Update tests in `librepengine/tests/`
   - Run `make test` to verify
   - Commit changes

2. **Update Android App:**
   - Make changes in `android-app/app/src/`
   - Update JNI if needed in `android-app/app/src/main/jni/`
   - Build and test on device
   - Commit changes

## Testing

### C Library Tests

```bash
cd librepengine
make test
```

### Android Tests

```bash
cd android-app
./gradlew test
```

## License

MIT License
