package com.example.nativegl

import android.os.Bundle
import android.view.SurfaceView
import android.widget.Button
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.example.nativegl.databinding.ActivityMainBinding


class MainActivity : AppCompatActivity() {
    private var surfaceView: SurfaceView? = null
    private var surfaceHolder: SurfaceViewHolder? = null
    private lateinit var mainBinding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        mainBinding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(mainBinding.root)

        surfaceView = findViewById(R.id.glSurface)
        surfaceHolder = SurfaceViewHolder()
        surfaceView?.holder?.addCallback(surfaceHolder)

        onNextClick()
        onHDRClick()
    }

    private fun onHDRClick() {
        mainBinding.btHDR.setOnClickListener{
            if ((it as Button).text == "SDR")
                (it as Button).text = "HDR"
            else
                (it as Button).text = "SDR"
        }
    }

    private fun onNextClick() {
        mainBinding.btNext.setOnClickListener{
            Toast.makeText(this, "next image", Toast.LENGTH_SHORT).show()
            JNIProxy.nextImage()
        }
    }

    override fun onResume() {
        super.onResume()
    }

    override fun onDestroy() {
        super.onDestroy()
    }
}