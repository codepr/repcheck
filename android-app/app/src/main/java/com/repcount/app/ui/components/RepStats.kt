package com.repcount.app.ui.components

import androidx.compose.foundation.background
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.width
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.material3.Card
import androidx.compose.material3.CardDefaults
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.unit.dp

@Composable
fun RepStats(
    repCount: Int,
    velocity: Float,
    rpe: Float,
    formOk: Boolean,
    fps: Float
) {
    Card(
        modifier = Modifier.fillMaxWidth(),
        colors = CardDefaults.cardColors(
            containerColor = MaterialTheme.colorScheme.surface
        ),
        elevation = CardDefaults.cardElevation(defaultElevation = 4.dp)
    ) {
        Column(
            modifier = Modifier.padding(16.dp)
        ) {
            Row(
                verticalAlignment = Alignment.Bottom
            ) {
                Column {
                    Text(
                        text = repCount.toString(),
                        style = MaterialTheme.typography.displayLarge,
                        color = Color(0xFF4CAF50),
                        fontWeight = FontWeight.Bold
                    )
                    Text(
                        text = "Reps",
                        style = MaterialTheme.typography.bodyMedium,
                        color = Color.Gray
                    )
                }
                
                Spacer(modifier = Modifier.width(24.dp))
                
                Column {
                    Text(
                        text = "%.1f".format(rpe),
                        style = MaterialTheme.typography.headlineMedium,
                        color = when {
                            rpe < 5 -> Color(0xFF4CAF50)
                            rpe < 7 -> Color(0xFFFFC107)
                            else -> Color(0xFFFF5722)
                        },
                        fontWeight = FontWeight.Bold
                    )
                    Text(
                        text = "RPE/10",
                        style = MaterialTheme.typography.bodyMedium,
                        color = Color.Gray
                    )
                }
                
                Spacer(modifier = Modifier.width(24.dp))
                
                Column {
                    Text(
                        text = if (formOk) "Good" else "Fix",
                        style = MaterialTheme.typography.titleLarge,
                        color = if (formOk) Color(0xFF4CAF50) else Color(0xFFFF5252),
                        fontWeight = FontWeight.Bold
                    )
                    Text(
                        text = "Form",
                        style = MaterialTheme.typography.bodyMedium,
                        color = Color.Gray
                    )
                }
            }
            
            Spacer(modifier = Modifier.height(12.dp))
            
            Row {
                Text(
                    text = "Velocity: %.1f deg/s".format(velocity),
                    style = MaterialTheme.typography.bodySmall,
                    color = Color.Gray
                )
                
                Spacer(modifier = Modifier.width(16.dp))
                
                Text(
                    text = "FPS: %.1f".format(fps),
                    style = MaterialTheme.typography.bodySmall,
                    color = Color.Gray
                )
            }
        }
    }
}

