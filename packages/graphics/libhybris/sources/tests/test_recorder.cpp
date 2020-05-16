/*
 * Copyright (C) 2013 Canonical Ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Authored by: Jim Hodapp <jim.hodapp@canonical.com>
 *              Guenter Schwann <guenter.schwann@canonical.com>
 *              Ricardo Salveti de Araujo <ricardo.salveti@canonical.com>
 */

#include <hybris/camera/camera_compatibility_layer.h>
#include <hybris/camera/camera_compatibility_layer_capabilities.h>

#include <hybris/media/recorder_compatibility_layer.h>

#include <hybris/input/input_stack_compatibility_layer.h>
#include <hybris/input/input_stack_compatibility_layer_codes_key.h>
#include <hybris/input/input_stack_compatibility_layer_flags_key.h>

#include <hybris/surface_flinger/surface_flinger_compatibility_layer.h>

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "test_common.h"

int shot_counter = 1;
int32_t current_zoom_level = 1;
bool new_camera_frame_available = true;
struct MediaRecorderWrapper *mr = 0;
GLuint preview_texture_id = 0;

static GLuint gProgram;
static GLuint gaPositionHandle, gaTexHandle, gsTextureHandle, gmTexMatrix;

void error_msg_cb(void* context)
{
	printf("%s \n", __PRETTY_FUNCTION__);
}

void shutter_msg_cb(void* context)
{
	printf("%s \n", __PRETTY_FUNCTION__);
}

void zoom_msg_cb(void* context, int32_t new_zoom_level)
{
	printf("%s \n", __PRETTY_FUNCTION__);

	current_zoom_level = new_zoom_level;
}

void autofocus_msg_cb(void* context)
{
	printf("%s \n", __PRETTY_FUNCTION__);
}

void raw_data_cb(void* data, uint32_t data_size, void* context)
{
	printf("%s: %d \n", __PRETTY_FUNCTION__, data_size);
}

void jpeg_data_cb(void* data, uint32_t data_size, void* context)
{
	printf("%s: %d \n", __PRETTY_FUNCTION__, data_size);
	struct CameraControl* cc = (struct CameraControl*) context;
	android_camera_start_preview(cc);
}

void size_cb(void* ctx, int width, int height)
{
	printf("Supported size: [%d,%d]\n", width, height);
}

void preview_texture_needs_update_cb(void* ctx)
{
	new_camera_frame_available = true;
}

void on_new_input_event(struct Event* event, void* context)
{
	assert(context);

	if (event->type == KEY_EVENT_TYPE && event->action == ISCL_KEY_EVENT_ACTION_UP) {
		printf("We have got a key event: %d \n", event->details.key.key_code);

		struct CameraControl* cc = (struct CameraControl*) context;

		int ret;
		switch (event->details.key.key_code) {
		case ISCL_KEYCODE_VOLUME_UP:
			printf("Starting video recording\n");

			android_camera_unlock(cc);

			ret = android_recorder_setCamera(mr, cc);
			if (ret < 0) {
				printf("android_recorder_setCamera() failed\n");
				return;
			}
			//state initial / idle
			ret = android_recorder_setAudioSource(mr, ANDROID_AUDIO_SOURCE_CAMCORDER);
			if (ret < 0) {
				printf("android_recorder_setAudioSource() failed\n");
				return;
			}
			ret = android_recorder_setVideoSource(mr, ANDROID_VIDEO_SOURCE_CAMERA);
			if (ret < 0) {
				printf("android_recorder_setVideoSource() failed\n");
				return;
			}
			//state initialized
			ret = android_recorder_setOutputFormat(mr, ANDROID_OUTPUT_FORMAT_MPEG_4);
			if (ret < 0) {
				printf("android_recorder_setOutputFormat() failed\n");
				return;
			}
			//state DataSourceConfigured
			ret = android_recorder_setAudioEncoder(mr, ANDROID_AUDIO_ENCODER_AAC);
			if (ret < 0) {
				printf("android_recorder_setAudioEncoder() failed\n");
				return;
			}
			ret = android_recorder_setVideoEncoder(mr, ANDROID_VIDEO_ENCODER_H264);
			if (ret < 0) {
				printf("android_recorder_setVideoEncoder() failed\n");
				return;
			}

			int fd;
			fd = open("/tmp/test_video_recorder.avi", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
			if (fd < 0) {
				printf("Could not open file for video recording\n");
				printf("FD: %i\n", fd);
				return;
			}
			ret = android_recorder_setOutputFile(mr, fd);
			if (ret < 0) {
				printf("android_recorder_setOutputFile() failed\n");
				return;
			}

			ret = android_recorder_setVideoSize(mr, 1280, 720);
			if (ret < 0) {
				printf("android_recorder_setVideoSize() failed\n");
				return;
			}
			ret = android_recorder_setVideoFrameRate(mr, 30);
			if (ret < 0) {
				printf("android_recorder_setVideoFrameRate() failed\n");
				return;
			}

			ret = android_recorder_prepare(mr);
			if (ret < 0) {
				printf("android_recorder_prepare() failed\n");
				return;
			}
			//state prepared
			ret = android_recorder_start(mr);
			if (ret < 0) {
				printf("android_recorder_start() failed\n");
				return;
			}
			break;
		case ISCL_KEYCODE_VOLUME_DOWN:
			printf("Stoping video recording\n");
			ret = android_recorder_stop(mr);

			printf("Stoping video recording returned\n");
			if (ret < 0) {
				printf("android_recorder_stop() failed\n");
				return;
			}
			printf("Stopped video recording\n");
			ret = android_recorder_reset(mr);
			if (ret < 0) {
				printf("android_recorder_reset() failed\n");
				return;
			}
			printf("Reset video recorder\n");
			break;
		}
	}
}

struct ClientWithSurface
{
	struct SfClient* client;
	struct SfSurface* surface;
};

struct ClientWithSurface client_with_surface(bool setup_surface_with_egl)
{
	struct ClientWithSurface cs;

	cs.client = sf_client_create();

	if (!cs.client) {
		printf("Problem creating client ... aborting now.");
		return cs;
	}

	static const size_t primary_display = 0;

	SfSurfaceCreationParameters params = {
		0,
		0,
		(int)sf_get_display_width(primary_display),
		(int)sf_get_display_height(primary_display),
		-1, //PIXEL_FORMAT_RGBA_8888,
		15000,
		0.5f,
		setup_surface_with_egl, // Do not associate surface with egl, will be done by camera HAL
		"CameraCompatLayerTestSurface"
	};

	cs.surface = sf_surface_create(cs.client, &params);

	if (!cs.surface) {
		printf("Problem creating surface ... aborting now.");
		return cs;
	}

	sf_surface_make_current(cs.surface);

	return cs;
}

static const char* vertex_shader()
{
	return
		"#extension GL_OES_EGL_image_external : require              \n"
		"attribute vec4 a_position;                                  \n"
		"attribute vec2 a_texCoord;                                  \n"
		"uniform mat4 m_texMatrix;                                   \n"
		"varying vec2 v_texCoord;                                    \n"
		"varying float topDown;                                      \n"
		"void main()                                                 \n"
		"{                                                           \n"
		"   gl_Position = a_position;                                \n"
		"   v_texCoord = a_texCoord;                                 \n"
		//                "   v_texCoord = (m_texMatrix * vec4(a_texCoord, 0.0, 1.0)).xy;\n"
		//"   topDown = v_texCoord.y;                                  \n"
		"}                                                           \n";
}

static const char* fragment_shader()
{
	return
		"#extension GL_OES_EGL_image_external : require      \n"
		"precision mediump float;                            \n"
		"varying vec2 v_texCoord;                            \n"
		"uniform samplerExternalOES s_texture;               \n"
		"void main()                                         \n"
		"{                                                   \n"
		"  gl_FragColor = texture2D( s_texture, v_texCoord );\n"
		"}                                                   \n";
}

int main(int argc, char** argv)
{
	printf("Test application for video recording using the camera\n");
	printf("Recording start with volume up button. And stops with volume down.\n");
	printf("The result is stored to /root/test_video.avi\n\n");

	struct CameraControlListener listener;
	memset(&listener, 0, sizeof(listener));
	listener.on_msg_error_cb = error_msg_cb;
	listener.on_msg_shutter_cb = shutter_msg_cb;
	listener.on_msg_focus_cb = autofocus_msg_cb;
	listener.on_msg_zoom_cb = zoom_msg_cb;

	listener.on_data_raw_image_cb = raw_data_cb;
	listener.on_data_compressed_image_cb = jpeg_data_cb;
	listener.on_preview_texture_needs_update_cb = preview_texture_needs_update_cb;
	struct CameraControl* cc = android_camera_connect_to(BACK_FACING_CAMERA_TYPE,
			&listener);
	if (cc == NULL) {
		printf("Problem connecting to camera");
		return 1;
	}

	listener.context = cc;

	mr = android_media_new_recorder();

	struct AndroidEventListener event_listener;
	event_listener.on_new_event = on_new_input_event;
	event_listener.context = cc;

	struct InputStackConfiguration input_configuration = { false, 25000 };

	android_input_stack_initialize(&event_listener, &input_configuration);
	android_input_stack_start();

	android_camera_dump_parameters(cc);

	printf("Supported video sizes:\n");
	android_camera_enumerate_supported_video_sizes(cc, size_cb, NULL);

	android_camera_set_preview_size(cc, 1280, 720);

	int width, height;
	android_camera_get_video_size(cc, &width, &height);
	printf("Current video size: [%d,%d]\n", width, height);

	struct ClientWithSurface cs = client_with_surface(true /* Associate surface with egl. */);

	if (!cs.surface) {
		printf("Problem acquiring surface for preview");
		return 1;
	}

	EGLDisplay disp = sf_client_get_egl_display(cs.client);
	EGLSurface surface = sf_surface_get_egl_surface(cs.surface);

	sf_surface_make_current(cs.surface);

	gProgram = create_program(vertex_shader(), fragment_shader());
	gaPositionHandle = glGetAttribLocation(gProgram, "a_position");
	gaTexHandle = glGetAttribLocation(gProgram, "a_texCoord");
	gsTextureHandle = glGetUniformLocation(gProgram, "s_texture");
	gmTexMatrix = glGetUniformLocation(gProgram, "m_texMatrix");

	glGenTextures(1, &preview_texture_id);
	glClearColor(1.0, 0., 0.5, 1.);
	glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(
			GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(
			GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	android_camera_set_preview_texture(cc, preview_texture_id);
	android_camera_start_preview(cc);

	GLfloat transformation_matrix[16];
	android_camera_get_preview_texture_transformation(cc, transformation_matrix);
	glUniformMatrix4fv(gmTexMatrix, 1, GL_FALSE, transformation_matrix);

	printf("Started camera preview.\n");

	while (true) {
		/*if (new_camera_frame_available)
		  {
		  printf("Updating texture");
		  new_camera_frame_available = false;
		  }*/
		static GLfloat vVertices[] = { 0.0f, 0.0f, 0.0f, // Position 0
			0.0f, 0.0f, // TexCoord 0
			0.0f, 1.0f, 0.0f, // Position 1
			0.0f, 1.0f, // TexCoord 1
			1.0f, 1.0f, 0.0f, // Position 2
			1.0f, 1.0f, // TexCoord 2
			1.0f, 0.0f, 0.0f, // Position 3
			1.0f, 0.0f // TexCoord 3
		};

		GLushort indices[] = { 0, 1, 2, 0, 2, 3 };

		// Set the viewport
		// Clear the color buffer
		glClear(GL_COLOR_BUFFER_BIT);
		// Use the program object
		glUseProgram(gProgram);
		// Enable attributes
		glEnableVertexAttribArray(gaPositionHandle);
		glEnableVertexAttribArray(gaTexHandle);
		// Load the vertex position
		glVertexAttribPointer(gaPositionHandle,
				3,
				GL_FLOAT,
				GL_FALSE,
				5 * sizeof(GLfloat),
				vVertices);
		// Load the texture coordinate
		glVertexAttribPointer(gaTexHandle,
				2,
				GL_FLOAT,
				GL_FALSE,
				5 * sizeof(GLfloat),
				vVertices+3);

		glActiveTexture(GL_TEXTURE0);
		// Set the sampler texture unit to 0
		glUniform1i(gsTextureHandle, 0);
		glUniform1i(gmTexMatrix, 0);
		android_camera_update_preview_texture(cc);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
		glDisableVertexAttribArray(gaPositionHandle);
		glDisableVertexAttribArray(gaTexHandle);

		eglSwapBuffers(disp, surface);
	}
}
