# RepEngine C Library

## Overview

Portable C library for rep counting, velocity tracking, RPE estimation, and form validation. Used by the Android app via JNI.

## Building

### Prerequisites
- GCC with C11 support
- CMake (optional, for advanced builds)

### Quick Build

```bash
make                    # Build library and tests
make lib                # Build static library only
make clean              # Clean build artifacts
make test               # Run all tests
```

### Test Each Module Individually

```bash
./tests/test_angle      # Angle calculations
./tests/test_form       # Form validation
./tests/test_log        # Logging utilities
./tests/test_rep_engine # Main engine
./tests/test_rpe        # RPE calculation
./tests/test_state_machine # Rep counting
./tests/test_velocity   # Velocity tracking
```

## Library API

### Core Types

```c
typedef struct {
    float x;
    float y;
    float z;
} RE_Landmark;

typedef enum {
    RE_EX_PUSHUP = 0,
    RE_EX_SQUAT = 1
} RE_ExerciseType;

typedef struct {
    int rep_count;
    float last_rep_velocity;
    float estimated_rpe;
    int form_ok;
} RE_Output;

typedef struct RE_Context RE_Context;
```

### Main Functions

```c
// Lifecycle
RE_Context* re_create(RE_ExerciseType type);
void re_destroy(RE_Context* ctx);

// Processing
void re_process_frame(
    RE_Context* ctx,
    const RE_Landmark* landmarks,
    int landmark_count,
    float timestamp,
    RE_Output* out
);
```

## Modules

| Module | Purpose |
|--------|---------|
| `rep_engine` | Main orchestrator |
| `state_machine` | Rep detection state machine |
| `angle` | Elbow angle computation |
| `form` | Form validation |
| `velocity` | Velocity tracker for RPE |
| `rpe` | Rate of Perceived Exertion |
| `log` | Logging utilities |

## Integration with Android

The C library is built as a static library (`librepengine.a`) and linked into the Android app via JNI.

### JNI Bridge

Located in `../android-app/app/src/main/jni/repengine_jni.cpp`

### CMake Configuration

See `../android-app/CMakeLists.txt` for Android build configuration.

## Testing

All tests use the `TEST_ASSERT()` and `TEST_ASSERT_FLOAT_NEAR()` macros. Tests exit with code 0 on success, 1 on failure.

```bash
make test
```

## Performance

- **Real-time capable**: 30-60 FPS
- **Memory**: Minimal allocations
- **Deterministic**: No randomness or external dependencies

## License

MIT License
