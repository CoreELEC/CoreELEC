/*
 * Copyright (c) 2014 Canonical Ltd
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

#include <stdio.h>
#include <string.h>

#include <android-config.h>

int is_wifi_driver_loaded();
int wifi_load_driver();
int wifi_unload_driver();

#define COMMAND_LOAD_WIFI "1"
#define COMMAND_UNLOAD_WIFI "0"

int main(int argc, char **argv)
{
	if (argc < 2) {
		fprintf(stdout, "To load driver: %s " COMMAND_LOAD_WIFI "\n", argv[0]);
		fprintf(stdout, "To unload driver: %s " COMMAND_UNLOAD_WIFI "\n", argv[0]);
	} else {
		int ret;

		if (strcmp(argv[1], COMMAND_LOAD_WIFI) == 0) {
			if ((ret = wifi_load_driver()) < 0)
				fprintf(stderr, "Cannot load driver (err %d)\n", ret);
			else
				fprintf(stdout, "Driver loaded\n");
		} else if (strcmp(argv[1], COMMAND_UNLOAD_WIFI) == 0) {
			if ((ret = wifi_unload_driver()) < 0)
				fprintf(stderr, "Cannot unload driver (err %d)\n", ret);
			else
				fprintf(stdout, "Driver unloaded\n");
		} else {
			fprintf(stderr, "Wrong command\n");
			return 1;
		}
	}

	fprintf(stdout, "WiFi driver load state: %d\n", is_wifi_driver_loaded());
	return 0;
}

// vim:ts=4:sw=4:noexpandtab
