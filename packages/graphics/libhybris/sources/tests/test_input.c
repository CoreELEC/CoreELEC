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
 * Authored by: Thomas Voß <thomas.voss@canonical.com>
 *              Ricardo Salveti de Araujo <ricardo.salveti@canonical.com>
 */

#include <android-config.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <signal.h>
#include <inttypes.h>

#include <hybris/input/input_stack_compatibility_layer.h>

bool g_stop = false;

void signal_handler(int sig)
{
	g_stop = true;
}

void on_new_event(struct Event* event, void* context)
{
	printf("%s", __PRETTY_FUNCTION__);

	printf("\tEventType: %d \n", event->type);
	printf("\tdevice_id: %d \n", event->device_id);
	printf("\tsource_id: %d \n", event->source_id);
	printf("\taction: %d \n", event->action);
	printf("\tflags: %d \n", event->flags);
	printf("\tmeta_state: %d \n", event->meta_state);

	switch (event->type) {
	case MOTION_EVENT_TYPE:
		printf("\tdetails.motion.event_time: %" PRId64 "\n",
				event->details.motion.event_time);
		printf("\tdetails.motion.pointer_coords.x: %f\n",
				event->details.motion.pointer_coordinates[0].x);
		printf("\tdetails.motion.pointer_coords.y: %f\n",
				event->details.motion.pointer_coordinates[0].y);
		break;
	default:
		break;
	}
}

int main(int argc, char** argv)
{
	g_stop = false;
	signal(SIGINT, signal_handler);

	struct AndroidEventListener listener;
	listener.on_new_event = on_new_event;
	listener.context = NULL;

	struct InputStackConfiguration config = {
		enable_touch_point_visualization : true,
		default_layer_for_touch_point_visualization : 10000,
		input_area_width : 1024,
		input_area_height : 1024
	};

	android_input_stack_initialize(&listener, &config);
	android_input_stack_start_waiting_for_flag(&g_stop);

	android_input_stack_stop();
	android_input_stack_shutdown();
	return 0;
}
