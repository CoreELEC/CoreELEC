/*
 * Copyright (c) 2012 Carsten Munk <carsten.munk@gmail.com>
 *               2008 The Android Open Source Project
 *               2013 Simon Busch <morphis@gravedo.de>
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

#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#define __USE_GNU
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/select.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <poll.h>

#include <hybris/properties/properties.h>
#include "properties_p.h"

static const char property_service_socket[] = "/dev/socket/" PROP_SERVICE_NAME;
static int send_prop_msg_no_reply = 0;

typedef struct prop_msg_s {
        unsigned cmd;
        char name[PROP_NAME_MAX];
        char value[PROP_VALUE_MAX];
} prop_msg_t;

/* Get/Set a property from the Android Init property socket */
static int send_prop_msg(prop_msg_t *msg,
		void (*propfn)(const char *, const char *, void *),
		void *cookie)
{
	union {
		struct sockaddr_un addr;
		struct sockaddr addr_g;
	} addr;
	socklen_t alen;
	size_t namelen;
	int s;
	int r;
	int result = -1;
	int patched_init = 0;

	/* if we tried to talk to the server in the past and didn't get a reply,
	 * it's fairly safe to say that init is not patched and this is all
	 * hopeless, so we should just quit while we're ahead
	 */
	if (send_prop_msg_no_reply == 1)
		return -EIO;

	s = socket(AF_LOCAL, SOCK_STREAM, 0);
	if (s < 0) {
		return result;
	}

	memset(&addr, 0, sizeof(addr));
	namelen = strlen(property_service_socket);
	strncpy(addr.addr.sun_path, property_service_socket,
			sizeof(addr.addr.sun_path));
	addr.addr.sun_family = AF_LOCAL;
	alen = namelen + offsetof(struct sockaddr_un, sun_path) + 1;

	if (TEMP_FAILURE_RETRY(connect(s, &addr.addr_g, alen) < 0)) {
		close(s);
		return result;
	}

	r = TEMP_FAILURE_RETRY(send(s, msg, sizeof(prop_msg_t), 0));

	if (r == sizeof(prop_msg_t)) {
		// We successfully wrote to the property server, so use recv
		// in case we need to get a property. Once the other side is
		// finished, the socket is closed.
		while ((r = recv(s, msg, sizeof(prop_msg_t), 0)) > 0) {
			if (r != sizeof(prop_msg_t)) {
				close(s);
				return result;
			}

			/* If we got a reply, this is a patched init */
			if (!patched_init)
				patched_init = 1;

			if (propfn)
				propfn(msg->name, msg->value, cookie);
		}

		/* We also just get a close in case of setprop */
		if ((r >= 0) && (patched_init ||
				(msg->cmd == PROP_MSG_SETPROP))) {
			result = 0;
		} else {
			send_prop_msg_no_reply = 1;
		}
	}

	close(s);
	return result;
}

int my_property_list(void (*propfn)(const char *key, const char *value, void *cookie), void *cookie)
{
	int err;
	prop_msg_t msg;

	memset(&msg, 0, sizeof(msg));
	msg.cmd = PROP_MSG_LISTPROP;

	err = send_prop_msg(&msg, propfn, cookie);
	if (err < 0)
		/* fallback to property cache */
		hybris_propcache_list((hybris_propcache_list_cb) propfn, cookie);

	return 0;
}

static int property_get_socket(const char *key, char *value, const char *default_value)
{
	int err;
	prop_msg_t msg;

	memset(&msg, 0, sizeof(msg));
	msg.cmd = PROP_MSG_GETPROP;

	if (key) {
		strncpy(msg.name, key, sizeof(msg.name));
		err = send_prop_msg(&msg, NULL, NULL);
		if (err < 0)
			return err;
	}

	/* In case it's null, just use the default */
	if ((strlen(msg.value) == 0) && (default_value)) {
		if (strlen(default_value) > PROP_VALUE_MAX -1)	return -1;
		strcpy(msg.value, default_value);
	}

	strcpy(value, msg.value);

	return 0;
}

int my_property_get(const char *key, char *value, const char *default_value)
{
	char *ret = NULL;

	if ((key) && (strlen(key) > PROP_NAME_MAX -1)) return -1;
	if (value == NULL) return -1;


	// Runtime cache will serialize property lookups within the process.
	// This will increase latency if multiple threads are doing many
	// parallel lookups to new properties, but the overhead should
	// be offset with the caching eventually.
	runtime_cache_lock();
	if (runtime_cache_get(key, value) == 0) {
		ret = value;
	} else if (property_get_socket(key, value, default_value) == 0) {
		runtime_cache_insert(key, value);
		ret = value;
	}
	runtime_cache_unlock();

	if (ret)
		return strlen(ret);


	/* In case the socket is not available, search the property file cache by hand */
	ret = hybris_propcache_find(key);

	if (ret) {
		strcpy(value, ret);
		return strlen(value);
	} else if (default_value != NULL) {
		strcpy(value, default_value);
		return strlen(value);
	} else {
		value = '\0';
	}

	return 0;
}

int my_property_set(const char *key, const char *value)
{
	int err;
	prop_msg_t msg;

	if (key == 0) return -1;
	if (value == 0) value = "";
	if (strlen(key) > PROP_NAME_MAX -1) return -1;
	if (strlen(value) > PROP_VALUE_MAX -1) return -1;

	runtime_cache_lock();
	runtime_cache_remove(key);
	runtime_cache_unlock();

	memset(&msg, 0, sizeof(msg));
	msg.cmd = PROP_MSG_SETPROP;
	strncpy(msg.name, key, sizeof(msg.name));
	strncpy(msg.value, value, sizeof(msg.value));

	err = send_prop_msg(&msg, NULL, NULL);
	if (err < 0) {
		return err;
	}

	return 0;
}

// vim:ts=4:sw=4:noexpandtab
