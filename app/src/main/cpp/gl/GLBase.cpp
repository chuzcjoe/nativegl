//
// Created by Zongcheng Chu on 4/16/24.
//

#include "GLBase.h"
#include "log.h"

GLBase::GLBase() {

}

GLBase::~GLBase() {
    if (_th.joinable()) {
        _th.join();
    }
    destroyRender();
    running = 0;
    init = 0;
}

//编译shader
int GLBase::loadShader(GLenum shaderType, const char* pSource) {
    int shader = glCreateShader(shaderType);
    if (shader) {
        glShaderSource(shader, 1, &pSource, NULL);
        glCompileShader(shader);
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                char* buf = (char*) malloc(infoLen);
                if (buf) {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    free(buf);
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    return shader;
}

int GLBase::createProgram(const char* pVertexSource, const char* pFragmentSource) {
    int vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
    int fragmentShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
    int program = glCreateProgram();
    if (program) {
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength) {
                char* buf = (char*) malloc(bufLength);
                if (buf) {
                    glGetProgramInfoLog(program, bufLength, NULL, buf);
                    free(buf);
                }
            }
            glDeleteProgram(program);
            program = 0;
        }
    }
    return program;
}

void GLBase::setWindow(JNIEnv *env, jobject surface) {
    // notify render thread that window has changed
    if (surface != nullptr) {
        eglSetting._window  = ANativeWindow_fromSurface(env, surface);
        std::lock_guard<std::mutex> lock(eglSetting._mutex);
        eglSetting.postMsg(EGLSetting::RenderThreadMessage::MSG_WINDOW_SET);
    } else {
        ANativeWindow_release(eglSetting._window);
    }
}

bool GLBase::initEGL() {
    // 1. Get display
    eglSetting._display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if(eglSetting._display == EGL_NO_DISPLAY)
    {
        LOG_ERROR("eglGetDisplay error");
        return false;
    }

    // 2. Init EGL
    EGLint *version = new EGLint[2];
    if(!eglInitialize(eglSetting._display, &version[0], &version[1])) {
        LOG_ERROR("eglInitialize error");
        return false;
    }

    // 3. Choose Config
    const EGLint attribs[] = {
            EGL_RED_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE, 8,
            EGL_ALPHA_SIZE, 8,
            EGL_DEPTH_SIZE, 8,
            EGL_STENCIL_SIZE, 8,
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL_NONE
    };

    EGLint num_config;
    if(!eglChooseConfig(eglSetting._display, attribs, NULL, 1, &num_config)) {
        LOG_ERROR("eglChooseConfig  error 1");
        return false;
    }

    if(!eglChooseConfig(eglSetting._display, attribs, &eglSetting._config, num_config, &num_config)) {
        LOG_ERROR("eglChooseConfig  error 2");
        return false;
    }

    // 4. Create Context
    int attrib_list[] = {
            EGL_CONTEXT_CLIENT_VERSION, 3,
            EGL_NONE
    };

    eglSetting._context = eglCreateContext(eglSetting._display, eglSetting._config, EGL_NO_CONTEXT, attrib_list);

    if (eglSetting._context == EGL_NO_CONTEXT) {
        LOG_ERROR("eglCreateContext  error");
        return false;
    }

    // 5. Create Surface
    eglSetting._surface = eglCreateWindowSurface(eglSetting._display, eglSetting._config, eglSetting._window, NULL);
    if (eglSetting._surface == EGL_NO_SURFACE) {
        LOG_ERROR("eglCreateWindowSurface  error");
        return false;
    }

    // 6. Bind context to drawing surface
    if (!eglMakeCurrent(eglSetting._display, eglSetting._surface, eglSetting._surface, eglSetting._context)) {
        LOG_ERROR("eglMakeCurrent  error");
        return false;
    }
    LOG_INFO("egl init success! ");

    eglQuerySurface(eglSetting._display, eglSetting._surface, EGL_WIDTH, &eglSetting._width);
    eglQuerySurface(eglSetting._display, eglSetting._surface, EGL_HEIGHT, &eglSetting._height);

    glEnable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    surfaceChange(eglSetting._width, eglSetting._height);
    window_set = true;
    return true;
}

void GLBase::destroyRender() {

    eglMakeCurrent(eglSetting._display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroyContext(eglSetting._display, eglSetting._context);
    eglDestroySurface(eglSetting._display, eglSetting._surface);
    eglTerminate(eglSetting._display);

    eglSetting._display = EGL_NO_DISPLAY;
    eglSetting._surface = EGL_NO_SURFACE;
    eglSetting._context = EGL_NO_CONTEXT;

    return;
}

void GLBase::startRenderThread(){
    if (!init){
        _th = std::thread(renderThread, this);
        init = !init;
    }
    running = true;
}
void GLBase::stopRenderThread() {
    running = false;
    return;
}

void GLBase::renderThread(GLBase* obj){
    obj->renderLoop();
}

void GLBase::renderLoop(){
    bool rendering = true;
    while (rendering) {
        if (running){

            if (eglSetting._msg == EGLSetting::RenderThreadMessage::MSG_WINDOW_SET) {
                initEGL();
                surfaceCreate();
            }
            if (eglSetting._msg == EGLSetting::RenderThreadMessage::MSG_RENDER_LOOP_EXIT) {
                rendering = false;
                destroyRender();
            }
            eglSetting.postMsg(EGLSetting::RenderThreadMessage::MSG_DRAW);

            if (!window_set) {
                usleep(16000);
                continue;
            }

            if (eglSetting._display) {
                std::lock_guard<std::mutex> lock(eglSetting._mutex);
                LOG_INFO("Drawing Frame");
                drawFrame();
                if (!eglSwapBuffers(eglSetting._display, eglSetting._surface)) {
                    LOG_INFO("eglSwapBuffers() returned error %d", eglGetError());
                }
            } else {
                usleep(16000);
            }
        } else {
            usleep(16000);
        }
    }
}