/*
 * Copyright (c) 2013 Canonical Ltd
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
 *
 */

#include <android-config.h>
#include <memory.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <hardware/audio.h>
#include <hardware/hardware.h>

/* Workaround for MTK */
#define AUDIO_HARDWARE_MODULE_ID2 "libaudio"

int main(int argc, char **argv)
{
	struct hw_module_t *hwmod = 0;
	struct audio_hw_device *audiohw;

	/* Initializing HAL */
	hw_get_module_by_class(AUDIO_HARDWARE_MODULE_ID,
#if defined(AUDIO_HARDWARE_MODULE_ID_PRIMARY)
					AUDIO_HARDWARE_MODULE_ID_PRIMARY,
#else
					"primary",
#endif
					(const hw_module_t**) &hwmod);
	if (!hwmod) {
		fprintf(stderr, "Failed to get hw module id: %s name: %s, trying alternative.",
				AUDIO_HARDWARE_MODULE_ID, AUDIO_HARDWARE_MODULE_ID_PRIMARY);
		hw_get_module_by_class(AUDIO_HARDWARE_MODULE_ID2,
				AUDIO_HARDWARE_MODULE_ID_PRIMARY,
				(const hw_module_t**) &hwmod);
	}

	assert(hwmod != NULL);

	assert(audio_hw_device_open(hwmod, &audiohw) == 0);
	do {
#if defined(AUDIO_DEVICE_API_VERSION_MIN)
		if (audiohw->common.version < AUDIO_DEVICE_API_VERSION_MIN) {
			fprintf(stderr, "Audio device API version %04x failed to meet minimum requirement %0x4.",
					audiohw->common.version, AUDIO_DEVICE_API_VERSION_MIN);
		} else
#endif
		if (audiohw->common.version != AUDIO_DEVICE_API_VERSION_CURRENT) {
			fprintf(stderr, "Audio device API version %04x doesn't match platform current %0x4.",
					audiohw->common.version, AUDIO_DEVICE_API_VERSION_CURRENT);
		} else
			break;

#if defined(AUDIO_DEVICE_API_VERSION_MIN)
		assert(audiohw->common.version >= AUDIO_DEVICE_API_VERSION_MIN);
#endif
		assert(audiohw->common.version == AUDIO_DEVICE_API_VERSION_CURRENT);
	} while(0);

	assert(audiohw->init_check(audiohw) == 0);
	fprintf(stdout, "Audio Hardware Interface initialized.\n");

#if (ANDROID_VERSION_MAJOR == 4 && ANDROID_VERSION_MINOR >= 1) || (ANDROID_VERSION_MAJOR >= 5)
	/* Check volume function calls */
	if (audiohw->get_master_volume) {
		float volume;
		audiohw->get_master_volume(audiohw, &volume);
		fprintf(stdout, "Master Volume: %f\n", volume);
	}
#endif

#if (ANDROID_VERSION_MAJOR == 4 && ANDROID_VERSION_MINOR >= 2) || (ANDROID_VERSION_MAJOR >= 5)
	if (audiohw->get_master_mute) {
		bool mute;
		audiohw->get_master_mute(audiohw, &mute);
		fprintf(stdout, "Master Mute: %d\n", mute);
	}
#endif

	/* Check output and input streams */
	struct audio_config config_out = {
		.sample_rate = 44100,
		.channel_mask = AUDIO_CHANNEL_OUT_STEREO,
		.format = AUDIO_FORMAT_PCM_16_BIT
	};
	struct audio_stream_out *stream_out = NULL;

	audiohw->open_output_stream(audiohw, 0, AUDIO_DEVICE_OUT_DEFAULT,
			AUDIO_OUTPUT_FLAG_PRIMARY, &config_out, &stream_out
#if ANDROID_VERSION_MAJOR >= 5
			, NULL
#endif
			);

	/* Try it again */
	if (!stream_out)
		audiohw->open_output_stream(audiohw, 0, AUDIO_DEVICE_OUT_DEFAULT,
				AUDIO_OUTPUT_FLAG_PRIMARY, &config_out, &stream_out
#if ANDROID_VERSION_MAJOR >= 5
				, NULL
#endif
				);

	assert(stream_out != NULL);

	fprintf(stdout, "Successfully created audio output stream: sample rate: %u, channel_mask: %u, format: %u\n",
			config_out.sample_rate, config_out.channel_mask, config_out.format);

	struct audio_config config_in = {
		.sample_rate = 48000,
		.channel_mask = AUDIO_CHANNEL_IN_STEREO,
		.format = AUDIO_FORMAT_PCM_16_BIT
	};
	struct audio_stream_in *stream_in = NULL;

	audiohw->open_input_stream(audiohw, 0, AUDIO_DEVICE_IN_DEFAULT,
			&config_in, &stream_in
#if ANDROID_VERSION_MAJOR >= 5
			, AUDIO_INPUT_FLAG_NONE, NULL, AUDIO_SOURCE_DEFAULT
#endif
			);
	assert(stream_in != NULL);

	fprintf(stdout, "Successfully created audio input stream: sample rate: %u, channel_mask: %u, format: %u\n",
			config_in.sample_rate, config_in.channel_mask, config_in.format);

	/* Close streams and device */
	audiohw->close_output_stream(audiohw, stream_out);
	audiohw->close_input_stream(audiohw, stream_in);

	audio_hw_device_close(audiohw);

	return 0;
}

// vim:ts=4:sw=4:noexpandtab
