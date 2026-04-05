package com.repcount.app.engine

data class RepResult(
    val repCount: Int,
    val lastRepVelocity: Float,
    val estimatedRpe: Float,
    val formOk: Boolean
)
