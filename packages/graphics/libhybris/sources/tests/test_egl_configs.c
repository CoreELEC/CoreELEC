/**
 * test_egl_configs: List available EGL configurations
 * Copyright (c) 2013 Thomas Perl <m@thp.io>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 **/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>


#define FAIL_WITH(...) do { fprintf(stderr, __VA_ARGS__); exit(1); } while (0)
#define TEST_LOG(...) fprintf(stderr, __VA_ARGS__)

#define TEST_ASSERT(x) do { \
    int error = eglGetError(); \
    if (error != EGL_SUCCESS) { \
        FAIL_WITH("EGL Error %x at %s:%d\n", error, __FILE__, __LINE__); \
    } \
    if (!(x)) { \
        FAIL_WITH("Assertion failed: %s at %s:%d\n", (#x), __FILE__, __LINE__); \
    } \
} while (0)


static EGLDisplay display;
static EGLConfig config;

int
ATTRIB(EGLint attribute)
{
    EGLint value;
    EGLBoolean result;

    result = eglGetConfigAttrib(display, config, attribute, &value);
    if (result != EGL_TRUE) {
        TEST_LOG("For attribute: %d\n", attribute);
        TEST_ASSERT(result == EGL_TRUE); // this makes sure we exit()
    }

    return value;
}

#define TEST_LOG_FEATURE(x) TEST_LOG("  %s: %s\n", (#x), ATTRIB(x) ? "yes" : "no")
#define TEST_LOG_VALUE(x) TEST_LOG("  %s: %d\n", (#x), ATTRIB(x))
#define TEST_LOG_VALUE_HEX(x) TEST_LOG("  %s: %x\n", (#x), ATTRIB(x))

#define SINCE_EGL_VERSION(maj, min) (major >= (maj) && minor >= (min))

/* Syntactic sugar for decoding one-of-many option fields */
#define ATTRIB_SWITCH(x) TEST_LOG("  %s: ", (#x)); switch (ATTRIB(x))
#define ATTRIB_CASE(x) case x: TEST_LOG("%s\n", (#x)); break
#define ATTRIB_SWITCH_DEFAULT(x) default: TEST_LOG("%x\n", ATTRIB(x)); break

/* Syntactic sugar for decoding bitfields */
#define START_DECODE_BITFIELD(x) value = ATTRIB(x); TEST_LOG("  %s: %x (", (#x), value);
#define TEST_BITFIELD_VALUE(x) if (value & (x)) TEST_LOG(" %s ", (#x))
#define END_DECODE_BITFIELD(x) TEST_LOG(")\n")


/**
 * For reference, the values tested below have been obtained from:
 * http://www.khronos.org/registry/egl/sdk/docs/man/xhtml/eglGetConfigAttrib.html
 * http://www.khronos.org/registry/egl/sdk/docs/man/xhtml/eglChooseConfig.html
 **/

int
main(int argc, char *argv[])
{
    EGLBoolean result;
    EGLint major, minor;
    EGLint num_config;
    EGLint num_config_result;
    EGLConfig *configs;
    EGLint value;
    int i;

    TEST_LOG("Starting test (EGL_PLATFORM=%s)\n", getenv("EGL_PLATFORM"));

    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    TEST_ASSERT(display != EGL_NO_DISPLAY);

    result = eglInitialize(display, &major, &minor);
    TEST_ASSERT(result == EGL_TRUE);

    TEST_LOG("EGL Version %d.%d\n", major, minor);
    TEST_LOG("Extensions: %s\n", eglQueryString(display, EGL_EXTENSIONS));

    result = eglGetConfigs(display, NULL, 0, &num_config);
    TEST_ASSERT(result == EGL_TRUE);

    TEST_LOG("Available configurations: %d\n", num_config);
    configs = (EGLConfig *)calloc(num_config, sizeof(EGLConfig));

    result = eglGetConfigs(display, configs, num_config, &num_config_result);
    TEST_ASSERT(result == EGL_TRUE);

    for (i=0; i<num_config_result; i++) {
        config = configs[i];

        TEST_LOG("===== Configuration #%d =====\n", i);

        TEST_LOG_VALUE(EGL_RED_SIZE);
        TEST_LOG_VALUE(EGL_GREEN_SIZE);
        TEST_LOG_VALUE(EGL_BLUE_SIZE);
        TEST_LOG_VALUE(EGL_ALPHA_SIZE);
        TEST_LOG_VALUE(EGL_BUFFER_SIZE);
        TEST_LOG_VALUE(EGL_DEPTH_SIZE);
        TEST_LOG_VALUE(EGL_STENCIL_SIZE);

        TEST_LOG_VALUE(EGL_CONFIG_ID);
        TEST_LOG_VALUE(EGL_LEVEL);
        TEST_LOG_VALUE(EGL_SAMPLE_BUFFERS);
        TEST_LOG_VALUE(EGL_SAMPLES);

        TEST_LOG_VALUE(EGL_MAX_PBUFFER_WIDTH);
        TEST_LOG_VALUE(EGL_MAX_PBUFFER_HEIGHT);
        TEST_LOG_VALUE(EGL_MAX_PBUFFER_PIXELS);

        TEST_LOG_VALUE(EGL_MIN_SWAP_INTERVAL);
        TEST_LOG_VALUE(EGL_MAX_SWAP_INTERVAL);

        TEST_LOG_VALUE(EGL_NATIVE_VISUAL_ID);
        TEST_LOG_VALUE(EGL_NATIVE_VISUAL_TYPE);

        ATTRIB_SWITCH(EGL_CONFIG_CAVEAT) {
            ATTRIB_CASE(EGL_NONE);
            ATTRIB_CASE(EGL_SLOW_CONFIG);
            ATTRIB_CASE(EGL_NON_CONFORMANT_CONFIG);
            ATTRIB_SWITCH_DEFAULT(EGL_CONFIG_CAVEAT);
        }

        TEST_LOG_FEATURE(EGL_BIND_TO_TEXTURE_RGB);
        TEST_LOG_FEATURE(EGL_BIND_TO_TEXTURE_RGBA);
        TEST_LOG_FEATURE(EGL_NATIVE_RENDERABLE);

        START_DECODE_BITFIELD(EGL_SURFACE_TYPE) {
            TEST_BITFIELD_VALUE(EGL_MULTISAMPLE_RESOLVE_BOX_BIT);
            TEST_BITFIELD_VALUE(EGL_PBUFFER_BIT);
            TEST_BITFIELD_VALUE(EGL_PIXMAP_BIT);
            TEST_BITFIELD_VALUE(EGL_SWAP_BEHAVIOR_PRESERVED_BIT);
            TEST_BITFIELD_VALUE(EGL_VG_ALPHA_FORMAT_PRE_BIT);
            TEST_BITFIELD_VALUE(EGL_VG_COLORSPACE_LINEAR_BIT);
            TEST_BITFIELD_VALUE(EGL_WINDOW_BIT);
        } END_DECODE_BITFIELD(EGL_SURFACE_TYPE);

        TEST_LOG_VALUE_HEX(EGL_TRANSPARENT_RED_VALUE);
        TEST_LOG_VALUE_HEX(EGL_TRANSPARENT_GREEN_VALUE);
        TEST_LOG_VALUE_HEX(EGL_TRANSPARENT_BLUE_VALUE);

        ATTRIB_SWITCH(EGL_TRANSPARENT_TYPE) {
            ATTRIB_CASE(EGL_NONE);
            ATTRIB_CASE(EGL_TRANSPARENT_RGB);
            ATTRIB_SWITCH_DEFAULT(EGL_TRANSPARENT_TYPE);
        }

        // New features introduced in EGL 1.2
        if (SINCE_EGL_VERSION(1, 2)) {
#ifdef EGL_LUMINANCE_SIZE
            TEST_LOG_VALUE(EGL_LUMINANCE_SIZE);
#endif /* EGL_LUMINANCE_SIZE */

#ifdef EGL_COLOR_BUFFER_TYPE
            ATTRIB_SWITCH(EGL_COLOR_BUFFER_TYPE) {
                ATTRIB_CASE(EGL_RGB_BUFFER);
                ATTRIB_CASE(EGL_LUMINANCE_BUFFER);
                ATTRIB_SWITCH_DEFAULT(EGL_COLOR_BUFFER_TYPE);
            }
#endif /* EGL_COLOR_BUFFER_TYPE */

#ifdef EGL_RENDERABLE_TYPE
            START_DECODE_BITFIELD(EGL_RENDERABLE_TYPE) {
                TEST_BITFIELD_VALUE(EGL_OPENGL_BIT);
                TEST_BITFIELD_VALUE(EGL_OPENGL_ES_BIT);
                TEST_BITFIELD_VALUE(EGL_OPENGL_ES2_BIT);
                TEST_BITFIELD_VALUE(EGL_OPENVG_BIT);
            } END_DECODE_BITFIELD(EGL_RENDERABLE_TYPE);
#endif /* EGL_RENDERABLE_TYPE */
        }

        // New features introduced in EGL 1.3
        if (SINCE_EGL_VERSION(1, 3)) {
#ifdef EGL_CONFORMANT
            START_DECODE_BITFIELD(EGL_CONFORMANT) {
                TEST_BITFIELD_VALUE(EGL_OPENGL_BIT);
                TEST_BITFIELD_VALUE(EGL_OPENGL_ES_BIT);
                TEST_BITFIELD_VALUE(EGL_OPENGL_ES2_BIT);
                TEST_BITFIELD_VALUE(EGL_OPENVG_BIT);
            } END_DECODE_BITFIELD(EGL_CONFORMANT);
#endif /* EGL_CONFORMANT */
        }

        TEST_LOG("\n\n");
    }

    free(configs);

    result = eglTerminate(display);
    TEST_ASSERT(result == EGL_TRUE);

    return 0;
}
