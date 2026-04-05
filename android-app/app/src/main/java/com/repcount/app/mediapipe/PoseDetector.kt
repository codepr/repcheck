package com.repcount.app.mediapipe

import android.content.Context
import android.graphics.Bitmap
import android.graphics.RectF
import androidx.camera.core.ImageProxy
import com.google.mlkit.vision.common.InputImage
import com.google.mlkit.vision.pose.PoseDetection
import com.google.mlkit.vision.pose.PoseDetector
import com.google.mlkit.vision.pose.detectors.PoseDetectorOptions
import kotlinx.coroutines.channels.awaitClose
import kotlinx.coroutines.flow.Flow
import kotlinx.coroutines.flow.callbackFlow
import kotlinx.coroutines.tasks.await
import java.nio.ByteBuffer

class PoseDetectorWrapper private constructor(
    private val context: Context,
    private val detector: PoseDetector
) {
    fun processImage(imageProxy: ImageProxy): Flow<PoseResult> = callbackFlow {
        try {
            val bitmap = imageProxyToBitmap(imageProxy)
            val inputImage = InputImage.fromBitmap(bitmap, imageProxy.imageInfo.rotationDegrees)
            
            val pose = detector.process(inputImage).await()
            val landmarks = pose landmarks@{
                val list = pose.allKeyPoints.map { point ->
                    PoseLandmark(
                        x = point.x,
                        y = point.y,
                        z = point.z
                    )
                }
                list
            }
            
            trySend(PoseResult(landmarks = landmarks, timestamp = System.currentTimeMillis() / 1000f))
        } catch (e: Exception) {
            e.printStackTrace()
        } finally {
            imageProxy.close()
        }
        awaitClose { close() }
    }
    
    private fun imageProxyToBitmap(imageProxy: ImageProxy): Bitmap {
        val buffer: ByteBuffer = imageProxy.planes[0].buffer
        val bytes = ByteArray(buffer.remaining())
        buffer.get(bytes)
        
        return Bitmap.createBitmap(
            imageProxy.width,
            imageProxy.height,
            Bitmap.Config.ARGB_8888
        ).apply {
            copyPixelsFromBuffer(ByteBuffer.wrap(bytes))
        }
    }
    
    fun close() {
        detector.close()
    }
    
    companion object {
        @Volatile
        private var instance: PoseDetectorWrapper? = null
        
        suspend fun getInstance(context: Context): PoseDetectorWrapper {
            return instance ?: synchronized(this) {
                instance ?: run {
                    val options = PoseDetectorOptions.Builder()
                        .setPoseDetectionModel(PoseDetectorOptions.STUDIO_POSE_MODEL_LITE)
                        .build()
                    val detector = PoseDetection.getClient(options)
                    instance = PoseDetectorWrapper(context, detector)
                    instance!!
                }
            }
        }
    }
}

