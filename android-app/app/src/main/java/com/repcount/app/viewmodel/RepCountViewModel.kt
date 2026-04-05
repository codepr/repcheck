package com.repcount.app.viewmodel

import android.util.Log
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.repcount.app.engine.RepEngine
import com.repcount.app.engine.RepResult
import com.repcount.app.mediapipe.PoseDetectorWrapper
import com.repcount.app.mediapipe.PoseResult
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asStateFlow
import kotlinx.coroutines.launch

data class RepCountUiState(
    val repCount: Int = 0,
    val lastRepVelocity: Float = 0f,
    val estimatedRpe: Float = 0f,
    val formOk: Boolean = false,
    val isProcessing: Boolean = false,
    val poseDetected: Boolean = false,
    val fps: Float = 0f
)

class RepCountViewModel(
    private val poseDetector: PoseDetectorWrapper,
    private val repEngine: RepEngine
) : ViewModel() {
    
    private val _uiState = MutableStateFlow(RepCountUiState())
    val uiState: StateFlow<RepCountUiState> = _uiState.asStateFlow()
    
    private var frameCount = 0
    private var lastFpsTime = 0L
    private var lastTimestamp: Float = 0f
    
    fun startProcessing() {
        viewModelScope.launch {
            _uiState.value = _uiState.value.copy(isProcessing = true)
            
            poseDetector.processImage(imageProxy).collect { poseResult ->
                processPoseResult(poseResult)
            }
        }
    }
    
    private fun processPoseResult(poseResult: PoseResult) {
        viewModelScope.launch {
            val timestamp = poseResult.timestamp.toFloat()
            
            if (lastTimestamp > 0f) {
                val deltaTime = timestamp - lastTimestamp
                if (deltaTime > 0f) {
                    frameCount++
                    val currentTime = System.currentTimeMillis()
                    if (currentTime - lastFpsTime >= 1000) {
                        val fps = frameCount.toFloat() / deltaTime
                        _uiState.value = _uiState.value.copy(fps = fps)
                        frameCount = 0
                        lastFpsTime = currentTime
                    }
                }
            }
            
            lastTimestamp = timestamp
            val landmarks = poseResult.landmarks
            
            if (landmarks.size == 33) {
                _uiState.value = _uiState.value.copy(poseDetected = true)
                
                try {
                    val repResult = repEngine.processFrame(landmarks, timestamp)
                    _uiState.value = _uiState.value.copy(
                        repCount = repResult.repCount,
                        lastRepVelocity = repResult.lastRepVelocity,
                        estimatedRpe = repResult.estimatedRpe,
                        formOk = repResult.formOk
                    )
                } catch (e: Exception) {
                    Log.e("RepCountViewModel", "Error processing frame", e)
                }
            } else {
                _uiState.value = _uiState.value.copy(poseDetected = false)
            }
        }
    }
    
    fun stopProcessing() {
        viewModelScope.launch {
            _uiState.value = _uiState.value.copy(isProcessing = false)
        }
    }
    
    fun resetCount() {
        repEngine.destroy()
        repEngine.nativeCreate(RepEngine.ExerciseType.PUSHUP.ordinal)
        _uiState.value = RepCountUiState(
            repCount = 0,
            lastRepVelocity = 0f,
            estimatedRpe = 0f,
            formOk = false,
            isProcessing = _uiState.value.isProcessing,
            poseDetected = _uiState.value.poseDetected,
            fps = _uiState.value.fps
        )
    }
    
    override fun onCleared() {
        super.onCleared()
        repEngine.destroy()
    }
}

