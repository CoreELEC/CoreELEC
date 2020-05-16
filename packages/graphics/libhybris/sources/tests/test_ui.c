/*
 * Copyright (C) 2013 Simon Busch <morphis@gravedo.de>
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
 */

#include <android-config.h>
#include <assert.h>

#include <hybris/ui/ui_compatibility_layer.h>
#include <hardware/gralloc.h>
#include <system/graphics.h>

int main(int argc, char **argv)
{
	struct graphic_buffer *buffer = 0;

	buffer = graphic_buffer_new();

	assert(buffer != NULL);

	graphic_buffer_free(buffer);

	buffer = graphic_buffer_new_sized(720, 1280, HAL_PIXEL_FORMAT_BGRA_8888, GRALLOC_USAGE_HW_RENDER | GRALLOC_USAGE_HW_TEXTURE);

	assert(buffer != NULL);
	assert(graphic_buffer_get_width(buffer) == 720);
	assert(graphic_buffer_get_height(buffer) == 1280);
	assert(graphic_buffer_get_usage(buffer) == (GRALLOC_USAGE_HW_TEXTURE | GRALLOC_USAGE_HW_RENDER));
	assert(graphic_buffer_get_pixel_format(buffer) == HAL_PIXEL_FORMAT_BGRA_8888);
	assert(graphic_buffer_get_native_buffer(buffer) != NULL);

	graphic_buffer_reallocate(buffer, 480, 640, HAL_PIXEL_FORMAT_BGRA_8888, GRALLOC_USAGE_HW_TEXTURE | GRALLOC_USAGE_HW_RENDER);

	assert(buffer != NULL);
	assert(graphic_buffer_get_width(buffer) == 480);
	assert(graphic_buffer_get_height(buffer) == 640);
	assert(graphic_buffer_get_usage(buffer) == (GRALLOC_USAGE_HW_TEXTURE | GRALLOC_USAGE_HW_RENDER));
	assert(graphic_buffer_get_pixel_format(buffer) == HAL_PIXEL_FORMAT_BGRA_8888);
	assert(graphic_buffer_get_native_buffer(buffer) != NULL);

	void *vaddr = NULL;
	graphic_buffer_lock(buffer, GRALLOC_USAGE_HW_RENDER, &vaddr);
	graphic_buffer_unlock(buffer);

#if ANDROID_VERSION_MAJOR==4 && ANDROID_VERSION_MINOR<=3
	graphic_buffer_set_index(buffer, 11);
	assert(graphic_buffer_get_index(buffer) == 11);
#endif

	graphic_buffer_free(buffer);

	return 0;
}
