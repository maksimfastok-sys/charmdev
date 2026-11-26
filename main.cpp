#include <jni.h>
#include <GLES3/gl3.h>

static bool g_wh = false, g_aim = false, g_menuOpen = true;
static float g_fov = 90.f, g_smooth = 0.35f;
static int g_bone = 0, g_screenW = 1080, g_screenH = 2400;
static GLuint g_program = 0, g_vbo = 0;

static void drawQuad(float x, float y, float w, float h, float r, float g, float b, float a) {
    if (g_program == 0) {
        const char* vs = "#version 300 es\nlayout(location=0) in vec2 pos;\nvoid main(){gl_Position=vec4(pos,0.0,1.0);}";
        const char* fs = "#version 300 es\nprecision mediump float;\nuniform vec4 color;\nout vec4 fragColor;\nvoid main(){fragColor=color;}";
        GLuint v = glCreateShader(GL_VERTEX_SHADER); glShaderSource(v, 1, &vs, 0); glCompileShader(v);
        GLuint f = glCreateShader(GL_FRAGMENT_SHADER); glShaderSource(f, 1, &fs, 0); glCompileShader(f);
        g_program = glCreateProgram(); glAttachShader(g_program, v); glAttachShader(g_program, f); glLinkProgram(g_program);
        glGenBuffers(1, &g_vbo);
    }
    glUseProgram(g_program);
    glUniform4f(glGetUniformLocation(g_program, "color"), r, g, b, a);
    float x1 = x / g_screenW * 2.0f - 1.0f, y1 = 1.0f - y / g_screenH * 2.0f;
    float x2 = (x + w) / g_screenW * 2.0f - 1.0f, y2 = 1.0f - (y + h) / g_screenH * 2.0f;
    float verts[] = { x1,y1, x2,y1, x2,y2, x1,y1, x2,y2, x1,y2 };
    glBindBuffer(GL_ARRAY_BUFFER, g_vbo); glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0); glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6); glDisableVertexAttribArray(0);
}

static bool hit(float px, float py, float x, float y, float w, float h) {
    return px >= x && px <= x + w && py >= y && py <= y + h;
}

extern "C" {
JNIEXPORT void JNICALL Java_com_mycompany_application_MainActivity_setWallhack(JNIEnv*, jclass, jboolean on) { g_wh = on; }
JNIEXPORT void JNICALL Java_com_mycompany_application_MainActivity_setAimBot(JNIEnv*, jclass, jboolean on, jfloat fov, jfloat smooth, jint bone) { g_aim = on; g_fov = fov; g_smooth = smooth; g_bone = bone; }

JNIEXPORT void JNICALL Java_com_mycompany_application_MainActivity_drawMenu(JNIEnv*, jclass, jint w, jint h) {
    if (!g_menuOpen) return; g_screenW = w; g_screenH = h;
    float mw = 460.f, mh = 380.f, mx = (w - mw) * 0.5f, my = (h - mh) * 0.5f;
    glDisable(GL_DEPTH_TEST); glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    drawQuad(mx, my, mw, mh, 0.07f, 0.07f, 0.09f, 0.94f);
    drawQuad(mx, my, mw, 42.f, 0.25f, 0.55f, 1.f, 1.f);
    drawQuad(mx + mw - 96.f, my + 84.f, 76.f, 28.f, g_wh ? 0.2f : 0.45f, g_wh ? 0.9f : 0.18f, 0.25f, 1.f);
    drawQuad(mx + mw - 96.f, my + 226.f, 76.f, 28.f, g_aim ? 0.2f : 0.45f, g_aim ? 0.9f : 0.18f, 0.25f, 1.f);
    glDisable(GL_BLEND);
}

JNIEXPORT jboolean JNICALL Java_com_mycompany_application_MainActivity_menuTouched(JNIEnv*, jclass, jfloat x, jfloat y, jint action) {
    if (!g_menuOpen) return JNI_FALSE;
    float mw = 460.f, mh = 380.f, mx = (g_screenW - mw) * 0.5f, my = (g_screenH - mh) * 0.5f;
    if (!hit(x, y, mx, my, mw, mh)) return JNI_FALSE;
    if (action == 0) {
        if (hit(x, y, mx + mw - 96.f, my + 84.f, 76.f, 28.f)) g_wh = !g_wh;
        else if (hit(x, y, mx + mw - 96.f, my + 226.f, 76.f, 28.f)) g_aim = !g_aim;
    }
    return JNI_TRUE;
}

JNIEXPORT void JNICALL Java_com_mycompany_application_GLES3JNIView_init(JNIEnv*, jclass) {}
JNIEXPORT void JNICALL Java_com_mycompany_application_GLES3JNIView_resize(JNIEnv*, jclass, jint, jint) {}
JNIEXPORT void JNICALL Java_com_mycompany_application_GLES3JNIView_step(JNIEnv*, jclass) {}
JNIEXPORT void JNICALL Java_com_mycompany_application_GLES3JNIView_imgui_1Shutdown(JNIEnv*, jclass) {}
JNIEXPORT void JNICALL Java_com_mycompany_application_GLES3JNIView_MotionEventClick(JNIEnv*, jclass, jboolean, jfloat, jfloat) {}
JNIEXPORT jstring JNICALL Java_com_mycompany_application_GLES3JNIView_getWindowRect(JNIEnv* env, jclass) { return env->NewStringUTF("0|0|1080|2400"); }
JNIEXPORT jstring JNICALL Java_com_mycompany_application_GLES3JNIView_getWindowRect2(JNIEnv* env, jclass) { return env->NewStringUTF("0|0|1080|2400"); }
}    
