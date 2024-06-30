//
// Created by Zongcheng Chu on 4/16/24.
//

#pragma once
#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <thread>
#include <mutex>
#include <cstdlib>
#include <math.h>
#include <queue>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <unistd.h>
#include <sys/time.h>
#include <initializer_list>
#include <memory>
#include <jni.h>

struct EGLSetting {
    enum class RenderThreadMessage {
        MSG_DRAW = 0,
        MSG_WINDOW_SET,
        MSG_RENDER_LOOP_EXIT
    };
    std::mutex _mutex;
    RenderThreadMessage _msg;

    ANativeWindow* _window;

    EGLDisplay _display;
    EGLSurface _surface;
    EGLContext _context;
    EGLConfig  _config;

    int _width;
    int _height;

    void postMsg(RenderThreadMessage msg) {
        _msg = msg;
    }
};

struct TextureUpdateInfo {
    std::string textureName;
};

class GLBase {
private:
    EGLSetting eglSetting;
    std::thread _th;

    bool init{false};
    bool running{false};
    bool window_set{false};
    std::queue<TextureUpdateInfo> textureUpdateQueue;
public:
    GLBase();
    ~GLBase();
    int loadShader(GLenum shaderType, const char* pSource);
    int createProgram(const char* pVertexSource, const char* pFragmentSource);
    bool initEGL();
    void destroyRender();
    static void renderThread(GLBase* obj);
    void renderLoop();
    void notifyTextureUpdate();

    virtual void setWindow(JNIEnv *env, jobject surface);
    virtual void startRenderThread();
    virtual void stopRenderThread();

    virtual void surfaceCreate() = 0;
    virtual void surfaceChange(int width, int height) = 0;
    virtual void drawFrame() = 0;
    virtual void updateTexture() = 0;
};
