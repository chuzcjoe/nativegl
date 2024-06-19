package com.example.nativegl

import android.view.SurfaceHolder

class SurfaceViewHolder : SurfaceHolder.Callback {
    override fun surfaceChanged(holder: SurfaceHolder, format: Int, w: Int, h: Int) {
        JNIProxy.setSurface(holder.surface)
        JNIProxy.startRender()
    }

    override fun surfaceCreated(holder: SurfaceHolder) {
        JNIProxy.calPixel()
        JNIProxy.setSurface(holder.surface)
        JNIProxy.startRender()
    }

    override fun surfaceDestroyed(holder: SurfaceHolder) {
        JNIProxy.stopRender()
        JNIProxy.setSurface(null)
    }
}
