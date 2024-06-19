#include <jni.h>
#include "GLRender.h"

GLRender *renderer;

extern "C"
JNIEXPORT void JNICALL
Java_com_example_nativegl_JNIProxy_calPixel(JNIEnv *env, jobject thiz) {
    renderer = new GLRender;
    renderer->cal_pixel();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_nativegl_JNIProxy_startRender(JNIEnv *env, jobject thiz) {
    renderer->StartRenderThread();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_nativegl_JNIProxy_stopRender(JNIEnv *env, jobject thiz) {
    renderer->StopRenderThread();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_nativegl_JNIProxy_setSurface(JNIEnv *env, jobject thiz, jobject surface) {
    renderer->SetWindow(env, surface);
}