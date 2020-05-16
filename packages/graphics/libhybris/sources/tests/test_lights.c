/*
 * Copyright (c) 2013 Simon Busch <morphis@gravedo.de>
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
#include <stdio.h>
#include <hardware/lights.h>

int main(int argc, char **argv)
{
	struct hw_module_t *hwmod = 0;
	struct light_device_t *notifications = 0;
	struct light_state_t notification_state;

	hw_get_module(LIGHTS_HARDWARE_MODULE_ID, (const hw_module_t**) &hwmod);
	assert(hwmod != NULL);

	hwmod->methods->open(hwmod, LIGHT_ID_NOTIFICATIONS, (hw_device_t **) &notifications);
	assert(notifications != NULL);

	memset(&notification_state, 0, sizeof(struct light_state_t));
	notification_state.color = 0xffffffff;
	notification_state.flashMode = LIGHT_FLASH_TIMED;
	notification_state.flashOnMS = 2000;
	notification_state.flashOffMS = 1000;
	notification_state.brightnessMode = BRIGHTNESS_MODE_USER;

	assert(notifications->set_light(notifications, &notification_state) == 0);

	return 0;
}

// vim:ts=4:sw=4:noexpandtab
