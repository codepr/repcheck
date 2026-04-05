package com.repcount.app.mediapipe

import com.repcount.app.engine.PoseLandmark

data class PoseResult(
    val landmarks: List<PoseLandmark>,
    val timestamp: Float
)

