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
#include <hybris/properties/properties.h>

int main(int argc, char *argv[])
{
	if (argc != 3) {
		fprintf(stderr, "usage: setprop <key> <value>\n");
		return 1;
	}

	if (property_set(argv[1], argv[2])){
		fprintf(stderr, "could not set property\n");
		return 1;
	}

	return 0;
}
