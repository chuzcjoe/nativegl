package com.example.nativegl

import android.content.Context
import android.content.pm.ActivityInfo
import android.os.Build
import android.os.Bundle
import android.view.Display
import android.view.SurfaceView
import android.view.WindowManager
import android.widget.Button
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.example.nativegl.databinding.ActivityMainBinding


class MainActivity : AppCompatActivity() {
    private var surfaceView: SurfaceView? = null
    private var surfaceHolder: SurfaceViewHolder? = null
    private lateinit var mainBinding: ActivityMainBinding
    private var isHDRSupported: Boolean = false

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        mainBinding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(mainBinding.root)

        surfaceView = findViewById(R.id.glSurface)
        surfaceHolder = SurfaceViewHolder()
        surfaceView?.holder?.addCallback(surfaceHolder)

        onNextClick()
        onHDRClick()

        checkHDRCapability()
    }

    private fun onHDRClick() {
        mainBinding.btHDR.setOnClickListener{
            if (!isHDRSupported) {
                Toast.makeText(this, "display does not support HDR", Toast.LENGTH_SHORT).show()
                return@setOnClickListener
            }
            if ((it as Button).text == "SDR") {
                (it as Button).text = "HDR"
                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
                    window.colorMode = ActivityInfo.COLOR_MODE_HDR
                }
                JNIProxy.setColorMode(true)
            } else {
                (it as Button).text = "SDR"
                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
                    window.colorMode = ActivityInfo.COLOR_MODE_DEFAULT
                }
                JNIProxy.setColorMode(false)
            }
        }
    }

    private fun onNextClick() {
        mainBinding.btNext.setOnClickListener{
            Toast.makeText(this, "next image", Toast.LENGTH_SHORT).show()
            JNIProxy.nextImage()
        }
    }

    private fun checkHDRCapability() {
        // Getting the default display
        val display = (getSystemService(Context.WINDOW_SERVICE) as WindowManager).defaultDisplay

        // Checking HDR capabilities
        val hdrCapabilities = display.hdrCapabilities
        if (hdrCapabilities != null &&
            hdrCapabilities.supportedHdrTypes.contains(Display.HdrCapabilities.HDR_TYPE_HDR10)) {
            Toast.makeText(this, "display supports HDR", Toast.LENGTH_SHORT).show()
            isHDRSupported = true;
        } else {
            Toast.makeText(this, "display does not support HDR", Toast.LENGTH_SHORT).show()
        }
    }

    override fun onResume() {
        super.onResume()
    }

    override fun onDestroy() {
        super.onDestroy()
    }
}