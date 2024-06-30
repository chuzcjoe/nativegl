package com.example.nativegl

import android.view.Surface

object JNIProxy {

    // Load native library
    init {
        System.loadLibrary("nativegl")
    }

    // Native methods
    external fun calPixel()
    external fun startRender()
    external fun stopRender()
    external fun setSurface(surface: Surface?)
    external fun nextImage();
    external fun setColorMode(isHDR : Boolean);
}