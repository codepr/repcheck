package com.repcount.app.camera

import android.content.Context
import androidx.camera.core.Preview
import androidx.camera.lifecycle.ProcessCameraProvider
import androidx.camera.view.PreviewView
import androidx.core.content.ContextCompat
import java.util.concurrent.ExecutorService
import java.util.concurrent.Executors

class CameraProvider private constructor(
    private val context: Context,
    private val cameraPreview: Preview
) {
    private val cameraExecutor: ExecutorService = Executors.newSingleThreadExecutor()
    
    fun bindCameraUseCases(
        previewView: PreviewView,
        cameraProvider: ProcessCameraProvider,
        lifecycleOwner: androidx.lifecycle.LifecycleOwner
    ) {
        try {
            cameraProvider.unbindAll()
            
            val previewSelector = Preview.Builder()
                .build()
                .also {
                    it.setSurfaceProvider(previewView.surfaceProvider)
                }
            
            val camera = cameraProvider.bindToLifecycle(
                lifecycleOwner,
                CameraSelector.DEFAULT_BACK_CAMERA,
                previewSelector
            )
        } catch (e: Exception) {
            e.printStackTrace()
        }
    }
    
    fun getCameraProvider(): ProcessCameraProvider {
        return ProcessCameraProvider.getInstance(context).get()
    }
    
    fun shutdown() {
        cameraExecutor.shutdown()
    }
    
    companion object {
        @Volatile
        private var instance: CameraProvider? = null
        
        fun getInstance(context: Context, preview: Preview): CameraProvider {
            return instance ?: synchronized(this) {
                instance ?: CameraProvider(context, preview).also { instance = it }
            }
        }
    }
}

