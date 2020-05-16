/*
 * Copyright (c) 2008 The Android Open Source Project
 *               2013 Canonical Ltd
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
#include <stdlib.h>
#include <string.h>

#include <hybris/properties/properties.h>

typedef struct {
	int count;
	char** items;
} list_t;

static void record_prop(const char* key, const char* name, void* opaque)
{
	list_t *list = (list_t *) opaque;

	char temp[PROP_VALUE_MAX + PROP_NAME_MAX + 16];
	snprintf(temp, sizeof(temp), "[%s]: [%s]", key, name);
	list->items = realloc(list->items, (list->count + 1) * sizeof(char *));
	list->items[list->count++] = strdup(temp);
}

static void list_properties(void)
{
	int n;

	list_t list;
	memset(&list, 0, sizeof(list_t));

	/* Record properties in the string list */
	if (property_list(record_prop, &list) < 0)
		return;

	for (n = 0; n < list.count; n++) {
		printf("%s\n", (char *) list.items[n]);
	}
}

int main(int argc, char *argv[])
{
	if (argc == 1) {
		list_properties();
	} else {
		char value[PROP_VALUE_MAX];
		char *default_value;
		if (argc > 2) {
			default_value = argv[2];
		} else {
			default_value = "";
		}

		property_get(argv[1], value, default_value);
		printf("%s\n", value);
	}
	return 0;
}
