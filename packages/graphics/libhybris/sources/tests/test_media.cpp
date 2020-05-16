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
 *              Ricardo Salveti de Araujo <ricardo.salveti@canonical.com>
 */

#include <hybris/media/media_compatibility_layer.h>
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

enum {
	OK          = 0,
	NO_ERROR    = 0,
};

static float DestWidth = 0.0, DestHeight = 0.0;
// Actual video dimmensions
static int Width = 0, Height = 0;

static GLuint gProgram;
static GLuint gaPositionHandle, gaTexHandle, gsTextureHandle, gmTexMatrix;

static GLfloat positionCoordinates[8];

struct MediaPlayerWrapper *player = NULL;

void calculate_position_coordinates()
{
	// Assuming cropping output for now
	float x = 1, y = 1;

	// Black borders
	x = (float) (Width / DestWidth);
	y = (float) (Height / DestHeight);

	// Make the larger side be 1
	if (x > y) {
		y /= x;
		x = 1;
	} else {
		x /= y;
		y = 1;
	}

	positionCoordinates[0] = -x;
	positionCoordinates[1] = y;
	positionCoordinates[2] = -x;
	positionCoordinates[3] = -y;
	positionCoordinates[4] = x;
	positionCoordinates[5] = -y;
	positionCoordinates[6] = x;
	positionCoordinates[7] = y;
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

	DestWidth = sf_get_display_width(primary_display);
	DestHeight = sf_get_display_height(primary_display);
	printf("Primary display width: %f, height: %f\n", DestWidth, DestHeight);

	SfSurfaceCreationParameters params = {
		0,
		0,
		(int)DestWidth,
		(int)DestHeight,
		-1, //PIXEL_FORMAT_RGBA_8888,
		15000,
		0.5f,
		setup_surface_with_egl, // Do not associate surface with egl, will be done by camera HAL
		"MediaCompatLayerTestSurface"
	};

	cs.surface = sf_surface_create(cs.client, &params);

	if (!cs.surface) {
		printf("Problem creating surface ... aborting now.");
		return cs;
	}

	sf_surface_make_current(cs.surface);

	return cs;
}

static const char *vertex_shader()
{
	return
		"attribute vec4 a_position;                                  \n"
		"attribute vec2 a_texCoord;                                  \n"
		"uniform mat4 m_texMatrix;                                   \n"
		"varying vec2 v_texCoord;                                    \n"
		"varying float topDown;                                      \n"
		"void main()                                                 \n"
		"{                                                           \n"
		"   gl_Position = a_position;                                \n"
		"   v_texCoord = (m_texMatrix * vec4(a_texCoord, 0.0, 1.0)).xy;\n"
		"}                                                           \n";
}

static const char *fragment_shader()
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

static int setup_video_texture(struct ClientWithSurface *cs, GLuint *preview_texture_id)
{
	assert(cs != NULL);
	assert(preview_texture_id != NULL);

	sf_surface_make_current(cs->surface);

	glGenTextures(1, preview_texture_id);
	glClearColor(0, 0, 0, 0);
	glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	android_media_set_preview_texture(player, *preview_texture_id);

	return 0;
}

void set_video_size_cb(int height, int width, void *context)
{
	printf("Video height: %d, width: %d\n", height, width);
	printf("Video dest height: %f, width: %f\n", DestHeight, DestWidth);

	Height = height;
	Width = width;
}

void media_prepared_cb(void *context)
{
	printf("Media is prepared for playback.\n");
}

int main(int argc, char **argv)
{
	if (argc < 2) {
		printf("Usage: test_media <video_to_play>\n");
		return EXIT_FAILURE;
	}

	player = android_media_new_player();
	if (player == NULL) {
		printf("Problem creating new media player.\n");
		return EXIT_FAILURE;
	}

	// Set player event cb for when the video size is known:
	android_media_set_video_size_cb(player, set_video_size_cb, NULL);
	android_media_set_media_prepared_cb(player, media_prepared_cb, NULL);

	printf("Setting data source to: %s.\n", argv[1]);

	if (android_media_set_data_source(player, argv[1]) != OK) {
		printf("Failed to set data source: %s\n", argv[1]);
		return EXIT_FAILURE;
	}

	printf("Creating EGL surface.\n");
	struct ClientWithSurface cs = client_with_surface(true /* Associate surface with egl. */);
	if (!cs.surface) {
		printf("Problem acquiring surface for preview");
		return EXIT_FAILURE;
	}

	printf("Creating GL texture.\n");

	GLuint preview_texture_id;
	EGLDisplay disp = sf_client_get_egl_display(cs.client);
	EGLSurface surface = sf_surface_get_egl_surface(cs.surface);

	sf_surface_make_current(cs.surface);

	if (setup_video_texture(&cs, &preview_texture_id) != OK) {
		printf("Problem setting up GL texture for video surface.\n");
		return EXIT_FAILURE;
	}

	printf("Starting video playback.\n");
	android_media_play(player);

	while (android_media_is_playing(player)) {
		const GLfloat textureCoordinates[] = {
			1.0f,  1.0f,
			0.0f,  1.0f,
			0.0f,  0.0f,
			1.0f,  0.0f
		};

		android_media_update_surface_texture(player);

		calculate_position_coordinates();

		gProgram = create_program(vertex_shader(), fragment_shader());
		gaPositionHandle = glGetAttribLocation(gProgram, "a_position");
		gaTexHandle = glGetAttribLocation(gProgram, "a_texCoord");
		gsTextureHandle = glGetUniformLocation(gProgram, "s_texture");
		gmTexMatrix = glGetUniformLocation(gProgram, "m_texMatrix");

		glClear(GL_COLOR_BUFFER_BIT);

		// Use the program object
		glUseProgram(gProgram);
		// Enable attributes
		glEnableVertexAttribArray(gaPositionHandle);
		glEnableVertexAttribArray(gaTexHandle);
		// Load the vertex position
		glVertexAttribPointer(gaPositionHandle,
				2,
				GL_FLOAT,
				GL_FALSE,
				0,
				positionCoordinates);
		// Load the texture coordinate
		glVertexAttribPointer(gaTexHandle,
				2,
				GL_FLOAT,
				GL_FALSE,
				0,
				textureCoordinates);

		GLfloat matrix[16];
		android_media_surface_texture_get_transformation_matrix(player, matrix);

		glUniformMatrix4fv(gmTexMatrix, 1, GL_FALSE, matrix);

		glActiveTexture(GL_TEXTURE0);
		// Set the sampler texture unit to 0
		glUniform1i(gsTextureHandle, 0);
		glUniform1i(gmTexMatrix, 0);
		android_media_update_surface_texture(player);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		glDisableVertexAttribArray(gaPositionHandle);
		glDisableVertexAttribArray(gaTexHandle);

		eglSwapBuffers(disp, surface);
	}

	android_media_stop(player);

	return EXIT_SUCCESS;
}
