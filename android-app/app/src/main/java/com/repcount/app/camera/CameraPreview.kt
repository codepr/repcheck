package com.repcount.app.camera

import android.net.Uri
import android.util.Log
import androidx.camera.core.CameraSelector
import androidx.camera.core.ImageAnalysis
import androidx.camera.core.ImageProxy
import androidx.camera.core.Preview
import androidx.camera.lifecycle.ProcessCameraProvider
import androidx.camera.view.PreviewView
import androidx.core.content.ContextCompat
import androidx.lifecycle.LifecycleOwner
import com.google.common.util.concurrent.MoreExecutors
import java.io.File
import java.io.FileOutputStream
import java.util.concurrent.ExecutorService
import java.util.concurrent.Executors

class CameraPreview(
    private val context: android.content.Context,
    private val lifecycleOwner: LifecycleOwner
) {
    private var cameraProvider: ProcessCameraProvider? = null
    private var camera: androidx.camera.core.Camera? = null
    private val cameraExecutor: ExecutorService = Executors.newSingleThreadExecutor()
    
    private var imageAnalysis: ImageAnalysis? = null
    private var imageAnalysisListener: ((ImageProxy) -> Unit)? = null
    
    fun setupCamera(previewView: PreviewView) {
        val cameraProviderFuture = ProcessCameraProvider.getInstance(context)
        
        cameraProviderFuture.addListener({
            cameraProvider = cameraProviderFuture.get()
            bindCameraPreview(previewView)
        }, ContextCompat.getMainExecutor(context))
    }
    
    private fun bindCameraPreview(previewView: PreviewView) {
        val preview = Preview.Builder()
            .build()
            .also {
                it.setSurfaceProvider(previewView.surfaceProvider)
            }
        
        imageAnalysis = ImageAnalysis.Builder()
            .setBackpressureStrategy(ImageAnalysis.STRATEGY_KEEP_ONLY_LATEST)
            .setOutputImageFormat(ImageAnalysis.OUTPUT_IMAGE_FORMAT_RGBA_8888)
            .build()
            .also {
                it.setAnalyzer(cameraExecutor) { imageProxy ->
                    imageAnalysisListener?.invoke(imageProxy)
                }
            }
        
        val cameraSelector = CameraSelector.DEFAULT_BACK_CAMERA
        
        try {
            cameraProvider?.unbindAll()
            camera = cameraProvider?.bindToLifecycle(
                lifecycleOwner,
                cameraSelector,
                preview,
                imageAnalysis!!
            )
            Log.d("CameraPreview", "Camera bound successfully")
        } catch (e: Exception) {
            Log.e("CameraPreview", "Camera binding failed", e)
        }
    }
    
    fun setImageAnalysisListener(listener: (ImageProxy) -> Unit) {
        imageAnalysisListener = listener
    }
    
    fun takePhoto(onPhotoTaken: (Uri) -> Unit) {
        // TODO: Implement photo capture
    }
    
    fun switchCamera() {
        // TODO: Implement camera switching
    }
    
    fun startPreview() {
        // Camera is already started when bound
    }
    
    fun stopPreview() {
        cameraProvider?.unbindAll()
    }
    
    fun release() {
        stopPreview()
        cameraExecutor.shutdown()
    }
    
    fun getImageAnalysis(): ImageAnalysis? = imageAnalysis
}

