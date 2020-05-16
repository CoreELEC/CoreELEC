/*
 * Copyright (c) 2012 Simon Busch <morphis@gravedo.de>
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
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hardware/sensors.h>

static void process_event(sensors_event_t *data)
{
    switch (data->type) {
        case SENSOR_TYPE_ACCELEROMETER:
            printf("Accelerometer: %+08.2f, %+08.2f, %+08.2f", data->acceleration.x,
                    data->acceleration.y, data->acceleration.z);
            break;
        case SENSOR_TYPE_ORIENTATION:
            printf("Orientation: %+08.2f, %+08.2f, %+08.2f", data->orientation.x,
                    data->orientation.y, data->orientation.z);
            break;
        case SENSOR_TYPE_GYROSCOPE:
            printf("Gyroscope: %+08.2f, %+08.2f, %+08.2f", data->gyro.x,
                    data->gyro.y, data->gyro.z);
            break;
        case SENSOR_TYPE_LIGHT:
            printf("Light: %+08.2f", data->light);
            break;
        case SENSOR_TYPE_PROXIMITY:
            printf("Proximity: %+08.2f", data->distance);
            break;
        default:
            printf("Other sensor data (not parsed yet, type=%d)", data->type);
    }
}

void float_to_char(float f,char * buffer, int maxChars){
    int beforeDot = (int)f;
    int afterDot = (int)((f-beforeDot)*1e6); /* rounding is not perfect, but that'll do */
    snprintf(buffer, maxChars, "%d.%d", beforeDot, afterDot);
}

static void print_sensor_info(int i, struct sensor_t const *s)
{
    char numStr[32];

    printf("=== Sensor %d ==\n", i);
    printf("Name: %s\n", s->name);
    printf("Vendor: %s\n", s->vendor);
    printf("Version: 0x%x\n", s->version);
    printf("Handle: 0x%x\n", s->handle);
    printf("Type: %d\n", s->type);
    // once the hw module is loaded, printf with %f will crash, so use integers
    float_to_char(s->maxRange, numStr, 30);
    printf("maxRange: %s\n", numStr);
    float_to_char(s->resolution, numStr, 30);
    printf("resolution: %s\n", numStr);
    float_to_char(s->power, numStr, 30);
    printf("power: %s mA\n", numStr);

    printf("minDelay: %d\n", s->minDelay);
    //printf("fifoReservedEventCount: %d\n", s->fifoReservedEventCount);
    //printf("fifoMaxEventCount: %d\n", s->fifoMaxEventCount);
    printf("\n\n\n");
}

int main(int argc, char **argv)
{
	struct hw_module_t *hwmod;
	struct sensors_poll_device_t *dev;

	hw_get_module(SENSORS_HARDWARE_MODULE_ID, (const hw_module_t**) &hwmod);
	assert(hwmod != NULL);

	if (sensors_open(hwmod, &dev) < 0) {
		printf("ERROR: failed to open sensors device\n");
		exit(1);
	}

        printf("Hardware module ID: %s\n", hwmod->id);
        printf("Hardware module Name: %s\n", hwmod->name);
        printf("Hardware module Author: %s\n", hwmod->author);
#ifdef HARDWARE_HAL_API_VERSION
        printf("Hardware module API version: 0x%x\n", hwmod->module_api_version);
        printf("Hardware HAL API version: 0x%x\n", hwmod->hal_api_version);
#else
        printf("Hardware module API version: 0x%x\n", hwmod->version_major);
        printf("Hardware HAL API version: 0x%x\n", hwmod->version_minor);
#endif
        printf("Poll device version: 0x%x\n", dev->common.version);

#ifndef SENSORS_MODULE_API_VERSION_0_1
#define SENSORS_MODULE_API_VERSION_0_1 0x0001
#endif
        printf("API VERSION 0.1 (legacy): 0x%x\n", SENSORS_MODULE_API_VERSION_0_1);
#ifdef SENSORS_DEVICE_API_VERSION_0_1
        printf("API VERSION 0.1: 0x%d\n", SENSORS_DEVICE_API_VERSION_0_1);
#endif
#ifdef SENSORS_DEVICE_API_VERSION_1_0
        printf("API VERSION 1.0: 0x%d\n", SENSORS_DEVICE_API_VERSION_1_0);
#endif
#ifdef SENSORS_DEVICE_API_VERSION_1_1
        printf("API VERSION 1.1: 0x%d\n", SENSORS_DEVICE_API_VERSION_1_1);
#endif
#ifdef SENSORS_DEVICE_API_VERSION_1_2
        printf("API VERSION 1.2: 0x%d\n", SENSORS_DEVICE_API_VERSION_1_2);
#endif
#ifdef SENSORS_DEVICE_API_VERSION_1_3
        printf("API VERSION 1.3: 0x%d\n", SENSORS_DEVICE_API_VERSION_1_3);
#endif
#ifdef SENSORS_DEVICE_API_VERSION_1_4
        printf("API VERSION 1.4: 0x%d\n", SENSORS_DEVICE_API_VERSION_1_4);
#endif

        struct sensors_module_t *smod = (struct sensors_module_t *)(hwmod);

        struct sensor_t const *sensors_list = NULL;
        int sensors = smod->get_sensors_list(smod, &sensors_list);
        printf("Got %d sensors\n", sensors);

        int res;
        int poll_sensor = ((argc == 2) ? atoi(argv[1]) : -1);

        if (poll_sensor != -1 && poll_sensor < sensors) {
            struct sensor_t const *s = sensors_list + poll_sensor;
            print_sensor_info(poll_sensor, s);

            res = dev->setDelay(dev, s->handle, s->minDelay);
            if (res != 0) {
                printf("Could not set delay: %s\n", strerror(-res));
            }
            res = dev->activate(dev, s->handle, 1);
            if (res != 0) {
                printf("Could not activate sensor: %s\n", strerror(-res));
            } else {
                printf("Reading events\n");
                while (1) {
                    sensors_event_t data;
                    data.sensor = -1;
                    printf("\rPolling... ");
                    fflush(stdout);
                    while (dev->poll(dev, &data, 1) != 1);
                    printf(" ");
                    if (data.sensor == poll_sensor) {
                        process_event(&data);
                    }
                    printf("\33[K");
                    fflush(stdout);
                }
                res = dev->activate(dev, s->handle, 0);
                if (res != 0) {
                    printf("Could not deactivate sensor: %s\n", strerror(-res));
                }
            }
        } else {
            int i;
            for (i=0; i<sensors; i++) {
                print_sensor_info(i, sensors_list + i);
            }
        }

	if (sensors_close(dev) < 0) {
		printf("ERROR: failed to close sensors device\n");
		exit(1);
	}

	return 0;
}

// vim:ts=4:sw=4:noexpandtab
