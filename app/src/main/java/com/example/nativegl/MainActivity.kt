package com.example.nativegl

import android.os.Bundle
import android.view.SurfaceView
import androidx.appcompat.app.AppCompatActivity


class MainActivity : AppCompatActivity() {
    private var surfaceView: SurfaceView? = null
    private var surfaceHolder: SurfaceViewHolder? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

//        JNIProxy.calPixel()
        surfaceView = findViewById(R.id.glSurface)
        surfaceHolder = SurfaceViewHolder()
        surfaceView?.holder?.addCallback(surfaceHolder)
    }

    override fun onResume() {
        super.onResume()
    }

    override fun onDestroy() {
        super.onDestroy()
    }
}