package com.repcount.app.ui.screens

import android.view.Surface
import androidx.camera.view.PreviewView
import androidx.compose.foundation.Canvas
import androidx.compose.foundation.background
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.size
import androidx.compose.foundation.layout.width
import androidx.compose.material3.Button
import androidx.compose.material3.Card
import androidx.compose.material3.CardDefaults
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.collectAsState
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.graphics.drawscope.Stroke
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.platform.LocalLifecycleOwner
import androidx.compose.ui.unit.dp
import androidx.lifecycle.ViewModelProvider
import com.repcount.app.engine.RepEngine
import com.repcount.app.mediapipe.PoseDetectorWrapper
import com.repcount.app.ui.components.RepStats
import com.repcount.app.viewmodel.RepCountUiState
import com.repcount.app.viewmodel.RepCountViewModel

@Composable
fun RepCountScreen() {
    val context = LocalContext.current
    val lifecycleOwner = LocalLifecycleOwner.current
    
    var poseDetector by remember { mutableStateOf<PoseDetectorWrapper?>(null) }
    var repEngine by remember { mutableStateOf<RepEngine?>(null) }
    var viewModel by remember { mutableStateOf<RepCountViewModel?>(null) }
    
    LaunchedEffect(Unit) {
        poseDetector = PoseDetectorWrapper.getInstance(context)
        repEngine = RepEngine(RepEngine.ExerciseType.PUSHUP)
        viewModel = ViewModelProvider(
            androidx.lifecycle.viewmodel.compose.viewModelViewModelStoreOwner(),
            androidx.lifecycle.viewmodel.compose.viewModelFactory {
                RepCountViewModel(poseDetector!!, repEngine!!)
            }
        )[RepCountViewModel::class.java]
    }
    
    val uiState by viewModel?.uiState?.collectAsState() ?: RepCountUiState()
    
    Box(modifier = Modifier.fillMaxSize()) {
        PreviewView(
            modifier = Modifier
                .fillMaxSize()
                .background(Color.Black)
        )
        
        Column(
            modifier = Modifier
                .fillMaxSize()
                .padding(16.dp)
        ) {
            Text(
                text = "RepCount - Push-up Tracker",
                style = MaterialTheme.typography.headlineSmall,
                color = Color.White
            )
            
            Spacer(modifier = Modifier.height(8.dp))
            
            RepStats(
                repCount = uiState.repCount,
                velocity = uiState.lastRepVelocity,
                rpe = uiState.estimatedRpe,
                formOk = uiState.formOk,
                fps = uiState.fps
            )
            
            Spacer(modifier = Modifier.height(16.dp))
            
            Row(
                modifier = Modifier.fillMaxWidth(),
                horizontalArrangement = Arrangement.spacedBy(8.dp)
            ) {
                Button(
                    onClick = { viewModel?.startProcessing() },
                    enabled = !uiState.isProcessing
                ) {
                    Text("Start")
                }
                
                Button(
                    onClick = { viewModel?.stopProcessing() },
                    enabled = uiState.isProcessing
                ) {
                    Text("Stop")
                }
                
                Button(
                    onClick = { viewModel?.resetCount() }
                ) {
                    Text("Reset")
                }
            }
            
            Spacer(modifier = Modifier.height(8.dp))
            
            Text(
                text = if (uiState.poseDetected) "Pose detected ✓" else "Position yourself for pose detection...",
                color = if (uiState.poseDetected) Color.Green else Color.Yellow
            )
            
            Text(
                text = "FPS: %.1f".format(uiState.fps),
                color = Color.Gray,
                style = MaterialTheme.typography.bodySmall
            )
        }
    }
}

