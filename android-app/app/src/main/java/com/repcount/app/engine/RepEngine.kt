package com.repcount.app.engine

class RepEngine(exerciseType: ExerciseType = ExerciseType.PUSHUP) {
    
    private val context: Long
    
    enum class ExerciseType {
        PUSHUP,
        SQUAT
    }
    
    init {
        context = nativeCreate(exerciseType.ordinal)
    }
    
    fun processFrame(landmarks: List<PoseLandmark>, timestamp: Float): RepResult {
        if (landmarks.size != 33) {
            throw IllegalArgumentException("Expected 33 landmarks, got ${landmarks.size}")
        }
        
        val xCoords = FloatArray(33)
        val yCoords = FloatArray(33)
        val zCoords = FloatArray(33)
        
        for (i in landmarks.indices) {
            xCoords[i] = landmarks[i].x
            yCoords[i] = landmarks[i].y
            zCoords[i] = landmarks[i].z
        }
        
        val resultArray = nativeProcessFrame(context, xCoords, yCoords, zCoords, timestamp)
        
        if (resultArray == null || resultArray.size != 4) {
            throw IllegalStateException("Invalid result from native engine")
        }
        
        return RepResult(
            repCount = resultArray[0].toInt(),
            lastRepVelocity = resultArray[1],
            estimatedRpe = resultArray[2],
            formOk = resultArray[3] > 0.5f
        )
    }
    
    fun destroy() {
        if (context != 0L) {
            nativeDestroy(context)
        }
    }
    
    private external fun nativeCreate(exerciseType: Int): Long
    
    private external fun nativeProcessFrame(
        ctx: Long,
        xCoords: FloatArray,
        yCoords: FloatArray,
        zCoords: FloatArray,
        timestamp: Float
    ): FloatArray?
    
    private external fun nativeDestroy(ctx: Long)
    
    companion object {
        init {
            System.loadLibrary("repengine")
        }
    }
}
