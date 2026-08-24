#include <android/input.h>
#include <android/log.h>
#include <android/native_activity.h>
#include <android_native_app_glue.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>

#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "hopf_math.h"

#define LOG_TAG "Hopf"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static const char *VERTEX_SHADER =
    "#version 300 es\n"
    "precision highp float;\n"
    "layout(location = 0) in vec3 a_position;\n"
    "layout(location = 1) in vec3 a_color;\n"
    "uniform mat4 u_mvp;\n"
    "out vec3 v_color;\n"
    "void main() {\n"
    "    gl_Position = u_mvp * vec4(a_position, 1.0);\n"
    "    v_color = a_color;\n"
    "}\n";

static const char *FRAGMENT_SHADER =
    "#version 300 es\n"
    "precision mediump float;\n"
    "in vec3 v_color;\n"
    "out vec4 out_color;\n"
    "void main() {\n"
    "    out_color = vec4(v_color, 1.0);\n"
    "}\n";

enum gesture_kind {
    GESTURE_NONE,
    GESTURE_ROTATE,
    GESTURE_PINCH,
    GESTURE_BLOCKED
};

struct matrix4 {
    float value[16];
};

struct engine {
    struct android_app *app;

    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
    int32_t width;
    int32_t height;

    GLuint program;
    GLuint vao;
    GLuint vbo;
    GLint mvp_location;

    struct hopf_state hopf;
    struct hopf_base_point *base_points;
    struct hopf_vertex *vertices;
    size_t base_point_count;
    size_t vertex_count;

    float yaw;
    float pitch;
    float distance;

    enum gesture_kind gesture;
    float last_x;
    float last_y;
    float pinch_last_distance;

    bool dirty;
};

static struct matrix4 matrix_identity(void) {
    struct matrix4 matrix = {{
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    }};
    return matrix;
}

static struct matrix4 matrix_multiply(struct matrix4 left, struct matrix4 right) {
    struct matrix4 result = {{0}};
    for (int column = 0; column < 4; ++column) {
        for (int row = 0; row < 4; ++row) {
            float value = 0.0f;
            for (int index = 0; index < 4; ++index) {
                value += left.value[index * 4 + row] * right.value[column * 4 + index];
            }
            result.value[column * 4 + row] = value;
        }
    }
    return result;
}

static struct matrix4 matrix_translation(float x, float y, float z) {
    struct matrix4 matrix = matrix_identity();
    matrix.value[12] = x;
    matrix.value[13] = y;
    matrix.value[14] = z;
    return matrix;
}

static struct matrix4 matrix_rotation_x(float angle) {
    struct matrix4 matrix = matrix_identity();
    float cosine = cosf(angle);
    float sine = sinf(angle);
    matrix.value[5] = cosine;
    matrix.value[6] = sine;
    matrix.value[9] = -sine;
    matrix.value[10] = cosine;
    return matrix;
}

static struct matrix4 matrix_rotation_y(float angle) {
    struct matrix4 matrix = matrix_identity();
    float cosine = cosf(angle);
    float sine = sinf(angle);
    matrix.value[0] = cosine;
    matrix.value[2] = -sine;
    matrix.value[8] = sine;
    matrix.value[10] = cosine;
    return matrix;
}

static struct matrix4 matrix_perspective(float fov_y, float aspect, float near_plane, float far_plane) {
    struct matrix4 matrix = {{0}};
    float scale = 1.0f / tanf(fov_y * 0.5f);
    matrix.value[0] = scale / aspect;
    matrix.value[5] = scale;
    matrix.value[10] = (far_plane + near_plane) / (near_plane - far_plane);
    matrix.value[11] = -1.0f;
    matrix.value[14] = (2.0f * far_plane * near_plane) / (near_plane - far_plane);
    return matrix;
}

static GLuint compile_shader(GLenum type, const char *source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    GLint compiled = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (compiled == GL_TRUE) return shader;

    GLint length = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
    char *log = length > 0 ? malloc((size_t)length) : NULL;
    if (log != NULL) {
        glGetShaderInfoLog(shader, length, NULL, log);
        LOGE("shader compilation failed: %s", log);
        free(log);
    } else {
        LOGE("shader compilation failed");
    }
    glDeleteShader(shader);
    return 0;
}

static GLuint link_program(GLuint vertex_shader, GLuint fragment_shader) {
    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    GLint linked = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (linked == GL_TRUE) return program;

    GLint length = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
    char *log = length > 0 ? malloc((size_t)length) : NULL;
    if (log != NULL) {
        glGetProgramInfoLog(program, length, NULL, log);
        LOGE("program link failed: %s", log);
        free(log);
    } else {
        LOGE("program link failed");
    }
    glDeleteProgram(program);
    return 0;
}

static void destroy_topology(struct engine *engine) {
    free(engine->vertices);
    free(engine->base_points);
    engine->vertices = NULL;
    engine->base_points = NULL;
    engine->vertex_count = 0;
    engine->base_point_count = 0;
}

static bool rebuild_topology(struct engine *engine) {
    destroy_topology(engine);

    engine->base_point_count = hopf_base_point_count(&engine->hopf);
    engine->vertex_count = hopf_vertex_count(&engine->hopf);
    if (engine->base_point_count == 0 || engine->vertex_count == 0) return false;

    engine->base_points = calloc(engine->base_point_count, sizeof(*engine->base_points));
    engine->vertices = calloc(engine->vertex_count, sizeof(*engine->vertices));
    if (engine->base_points == NULL || engine->vertices == NULL) {
        LOGE("could not allocate Hopf topology: %zu base points, %zu vertices",
             engine->base_point_count, engine->vertex_count);
        destroy_topology(engine);
        return false;
    }

    hopf_generate_base_points(&engine->hopf, engine->base_points, engine->base_point_count);
    hopf_generate_fibration(
        &engine->hopf,
        engine->base_points,
        engine->base_point_count,
        engine->vertices,
        engine->vertex_count
    );

    glBindBuffer(GL_ARRAY_BUFFER, engine->vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        (GLsizeiptr)(engine->vertex_count * sizeof(*engine->vertices)),
        engine->vertices,
        GL_STATIC_DRAW
    );
    engine->dirty = true;
    return true;
}

static bool create_renderer(struct engine *engine) {
    GLuint vertex_shader = compile_shader(GL_VERTEX_SHADER, VERTEX_SHADER);
    GLuint fragment_shader = compile_shader(GL_FRAGMENT_SHADER, FRAGMENT_SHADER);
    if (vertex_shader == 0 || fragment_shader == 0) {
        if (vertex_shader != 0) glDeleteShader(vertex_shader);
        if (fragment_shader != 0) glDeleteShader(fragment_shader);
        return false;
    }

    engine->program = link_program(vertex_shader, fragment_shader);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    if (engine->program == 0) return false;

    engine->mvp_location = glGetUniformLocation(engine->program, "u_mvp");

    glGenVertexArrays(1, &engine->vao);
    glBindVertexArray(engine->vao);

    glGenBuffers(1, &engine->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, engine->vbo);
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        (GLsizei)sizeof(struct hopf_vertex),
        (const void *)offsetof(struct hopf_vertex, position)
    );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        (GLsizei)sizeof(struct hopf_vertex),
        (const void *)offsetof(struct hopf_vertex, color)
    );
    glEnableVertexAttribArray(1);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_CULL_FACE);

    if (!rebuild_topology(engine)) return false;

    LOGI("renderer ready: GL_VERSION=%s GL_RENDERER=%s vertices=%zu",
         glGetString(GL_VERSION), glGetString(GL_RENDERER), engine->vertex_count);
    return true;
}

static bool initialize_display(struct engine *engine) {
    if (engine->app->window == NULL) return false;

    const EGLint config_attributes[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT_KHR,
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_DEPTH_SIZE, 24,
        EGL_NONE
    };
    const EGLint context_attributes[] = {
        EGL_CONTEXT_CLIENT_VERSION, 3,
        EGL_NONE
    };

    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (display == EGL_NO_DISPLAY || !eglInitialize(display, NULL, NULL)) {
        LOGE("eglInitialize failed: 0x%x", eglGetError());
        return false;
    }

    EGLConfig config = NULL;
    EGLint config_count = 0;
    if (!eglChooseConfig(display, config_attributes, &config, 1, &config_count) || config_count != 1) {
        LOGE("could not choose GLES3 EGL config: 0x%x", eglGetError());
        eglTerminate(display);
        return false;
    }

    EGLint format = 0;
    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);
    ANativeWindow_setBuffersGeometry(engine->app->window, 0, 0, format);

    EGLSurface surface = eglCreateWindowSurface(display, config, engine->app->window, NULL);
    EGLContext context = eglCreateContext(display, config, EGL_NO_CONTEXT, context_attributes);
    if (surface == EGL_NO_SURFACE || context == EGL_NO_CONTEXT) {
        LOGE("could not create EGL surface/context: 0x%x", eglGetError());
        if (surface != EGL_NO_SURFACE) eglDestroySurface(display, surface);
        if (context != EGL_NO_CONTEXT) eglDestroyContext(display, context);
        eglTerminate(display);
        return false;
    }

    if (!eglMakeCurrent(display, surface, surface, context)) {
        LOGE("eglMakeCurrent failed: 0x%x", eglGetError());
        eglDestroyContext(display, context);
        eglDestroySurface(display, surface);
        eglTerminate(display);
        return false;
    }

    engine->display = display;
    engine->surface = surface;
    engine->context = context;
    eglQuerySurface(display, surface, EGL_WIDTH, &engine->width);
    eglQuerySurface(display, surface, EGL_HEIGHT, &engine->height);
    glViewport(0, 0, engine->width, engine->height);

    if (!create_renderer(engine)) {
        eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        eglDestroyContext(display, context);
        eglDestroySurface(display, surface);
        eglTerminate(display);
        engine->display = EGL_NO_DISPLAY;
        engine->surface = EGL_NO_SURFACE;
        engine->context = EGL_NO_CONTEXT;
        return false;
    }

    engine->dirty = true;
    return true;
}

static void terminate_display(struct engine *engine) {
    if (engine->display == EGL_NO_DISPLAY) return;

    destroy_topology(engine);
    if (engine->vbo != 0) glDeleteBuffers(1, &engine->vbo);
    if (engine->vao != 0) glDeleteVertexArrays(1, &engine->vao);
    if (engine->program != 0) glDeleteProgram(engine->program);
    engine->vbo = 0;
    engine->vao = 0;
    engine->program = 0;

    eglMakeCurrent(engine->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    if (engine->context != EGL_NO_CONTEXT) eglDestroyContext(engine->display, engine->context);
    if (engine->surface != EGL_NO_SURFACE) eglDestroySurface(engine->display, engine->surface);
    eglTerminate(engine->display);

    engine->display = EGL_NO_DISPLAY;
    engine->surface = EGL_NO_SURFACE;
    engine->context = EGL_NO_CONTEXT;
}

static void update_surface_size(struct engine *engine) {
    if (engine->display == EGL_NO_DISPLAY || engine->surface == EGL_NO_SURFACE) return;
    eglQuerySurface(engine->display, engine->surface, EGL_WIDTH, &engine->width);
    eglQuerySurface(engine->display, engine->surface, EGL_HEIGHT, &engine->height);
    glViewport(0, 0, engine->width, engine->height);
    engine->dirty = true;
}

static void draw_frame(struct engine *engine) {
    if (engine->display == EGL_NO_DISPLAY || engine->program == 0) return;
    if (engine->width <= 0 || engine->height <= 0) return;

    float aspect = (float)engine->width / (float)engine->height;
    struct matrix4 projection = matrix_perspective(
        (float)(45.0 * M_PI / 180.0),
        aspect,
        0.1f,
        100.0f
    );
    struct matrix4 view = matrix_translation(0.0f, 0.0f, -engine->distance);
    struct matrix4 rotation_x = matrix_rotation_x(engine->pitch);
    struct matrix4 rotation_y = matrix_rotation_y(engine->yaw);
    struct matrix4 model = matrix_multiply(rotation_y, rotation_x);
    struct matrix4 view_model = matrix_multiply(view, model);
    struct matrix4 mvp = matrix_multiply(projection, view_model);

    glClearColor(0.035f, 0.04f, 0.055f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(engine->program);
    glUniformMatrix4fv(engine->mvp_location, 1, GL_FALSE, mvp.value);
    glBindVertexArray(engine->vao);

    GLsizei samples = (GLsizei)engine->hopf.samples_per_fiber;
    for (size_t fiber = 0; fiber < engine->base_point_count; ++fiber) {
        GLint first = (GLint)(fiber * (size_t)samples);
        glDrawArrays(GL_LINE_LOOP, first, samples);
    }

    if (!eglSwapBuffers(engine->display, engine->surface)) {
        LOGE("eglSwapBuffers failed: 0x%x", eglGetError());
    }
    engine->dirty = false;
}

static float pointer_distance(const AInputEvent *event) {
    float delta_x = AMotionEvent_getX(event, 0) - AMotionEvent_getX(event, 1);
    float delta_y = AMotionEvent_getY(event, 0) - AMotionEvent_getY(event, 1);
    return hypotf(delta_x, delta_y);
}

static int32_t handle_input(struct android_app *app, AInputEvent *event) {
    struct engine *engine = app->userData;
    if (AInputEvent_getType(event) != AINPUT_EVENT_TYPE_MOTION) return 0;

    int32_t action = AMotionEvent_getAction(event);
    int32_t masked_action = action & AMOTION_EVENT_ACTION_MASK;
    size_t pointer_count = AMotionEvent_getPointerCount(event);

    switch (masked_action) {
        case AMOTION_EVENT_ACTION_DOWN:
            engine->gesture = GESTURE_ROTATE;
            engine->last_x = AMotionEvent_getX(event, 0);
            engine->last_y = AMotionEvent_getY(event, 0);
            return 1;

        case AMOTION_EVENT_ACTION_POINTER_DOWN:
            if (pointer_count >= 2) {
                engine->gesture = GESTURE_PINCH;
                engine->pinch_last_distance = pointer_distance(event);
                return 1;
            }
            return 0;

        case AMOTION_EVENT_ACTION_MOVE:
            if (engine->gesture == GESTURE_ROTATE && pointer_count == 1) {
                float x = AMotionEvent_getX(event, 0);
                float y = AMotionEvent_getY(event, 0);
                engine->yaw += (x - engine->last_x) * 0.008f;
                engine->pitch += (y - engine->last_y) * 0.008f;
                if (engine->pitch < -1.5f) engine->pitch = -1.5f;
                if (engine->pitch > 1.5f) engine->pitch = 1.5f;
                engine->last_x = x;
                engine->last_y = y;
                engine->dirty = true;
                return 1;
            }

            if (engine->gesture == GESTURE_PINCH && pointer_count >= 2) {
                float distance = pointer_distance(event);
                if (distance > 1.0f && engine->pinch_last_distance > 1.0f) {
                    engine->distance *= engine->pinch_last_distance / distance;
                    if (engine->distance < 1.4f) engine->distance = 1.4f;
                    if (engine->distance > 12.0f) engine->distance = 12.0f;
                    engine->dirty = true;
                }
                engine->pinch_last_distance = distance;
                return 1;
            }
            return engine->gesture == GESTURE_BLOCKED ? 1 : 0;

        case AMOTION_EVENT_ACTION_POINTER_UP:
            if (engine->gesture == GESTURE_PINCH) {
                engine->gesture = GESTURE_BLOCKED;
                return 1;
            }
            return 0;

        case AMOTION_EVENT_ACTION_UP:
        case AMOTION_EVENT_ACTION_CANCEL:
            engine->gesture = GESTURE_NONE;
            return 1;

        default:
            return 0;
    }
}

static void handle_command(struct android_app *app, int32_t command) {
    struct engine *engine = app->userData;

    switch (command) {
        case APP_CMD_INIT_WINDOW:
            if (app->window != NULL && engine->display == EGL_NO_DISPLAY) {
                initialize_display(engine);
            }
            break;

        case APP_CMD_TERM_WINDOW:
            terminate_display(engine);
            break;

        case APP_CMD_WINDOW_RESIZED:
        case APP_CMD_CONTENT_RECT_CHANGED:
        case APP_CMD_CONFIG_CHANGED:
            update_surface_size(engine);
            break;

        case APP_CMD_GAINED_FOCUS:
            engine->dirty = true;
            break;

        default:
            break;
    }
}

void android_main(struct android_app *app) {
    struct engine engine;
    memset(&engine, 0, sizeof(engine));
    engine.app = app;
    engine.display = EGL_NO_DISPLAY;
    engine.surface = EGL_NO_SURFACE;
    engine.context = EGL_NO_CONTEXT;
    engine.distance = 2.8f;
    engine.pitch = 0.15f;
    engine.yaw = -0.6f;
    engine.gesture = GESTURE_NONE;
    engine.dirty = true;
    hopf_state_default(&engine.hopf);

    app->userData = &engine;
    app->onAppCmd = handle_command;
    app->onInputEvent = handle_input;

    while (true) {
        int events = 0;
        struct android_poll_source *source = NULL;
        int timeout = engine.dirty && engine.display != EGL_NO_DISPLAY ? 0 : -1;
        int ident = ALooper_pollOnce(timeout, NULL, &events, (void **)&source);

        if (ident >= 0 && source != NULL) source->process(app, source);

        if (app->destroyRequested != 0) {
            terminate_display(&engine);
            return;
        }

        if (engine.dirty) draw_frame(&engine);
    }
}
