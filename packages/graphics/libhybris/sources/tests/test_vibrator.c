/*
 * Copyright (C) 2014 Jolla Ltd.
 * Contact: Simonas Leleiva <simonas.leleiva@jollamobile.com>
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

#if HAS_VIBRATOR_HEADER

#include <android-config.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#if (ANDROID_VERSION_MAJOR >= 7)
#include <hardware/vibrator.h>
#else
#include <hardware_legacy/vibrator.h>
#endif

int main(int argc, char **argv)
{
	// Android mistakenly reports that vibrator does not exist:
	//assert(vibrator_exists() != 0);

#if (ANDROID_VERSION_MAJOR >= 7)
        struct hw_module_t *hwmod;
        vibrator_device_t *dev;

        hw_get_module(VIBRATOR_HARDWARE_MODULE_ID, (const hw_module_t **)(&hwmod));
        assert(hwmod != NULL);

        if (vibrator_open(hwmod, &dev) < 0) {
                printf("ERROR: failed to open vibrator device\n");
                exit(1);
        }

        if (dev->vibrator_on(dev, 1000) < 0) {
#else
        if (vibrator_on(1000) < 0) {
#endif
		printf("ERROR: vibrator failed to vibrate\n");
		exit(1);
	}

	return 0;
}

#else
#include <stdio.h>

int main(int argc, char *argv[])
{
    printf("test_vibrator is not supported in this build");
    return 0;
}
#endif

// vim:ts=4:sw=4:noexpandtab
