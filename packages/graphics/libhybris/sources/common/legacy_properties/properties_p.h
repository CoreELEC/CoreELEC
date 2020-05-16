/*
 * Copyright (c) 2013 Jolla Ltd. <robin.burchell@jollamobile.com>
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

#ifndef HYBRIS_PROPERTIES
#define HYBRIS_PROPERTIES

typedef void (*hybris_propcache_list_cb)(const char *key, const char *value, void *cookie);

void hybris_propcache_list(hybris_propcache_list_cb cb, void *cookie);
char *hybris_propcache_find(const char *key);

#ifndef NO_RUNTIME_PROPERTY_CACHE
void runtime_cache_lock();
void runtime_cache_unlock();
int  runtime_cache_get(const char *key, char *value);
void runtime_cache_insert(const char *key, char *value);
void runtime_cache_remove(const char *key);
#else
#define runtime_cache_lock()
#define runtime_cache_unlock()
#define runtime_cache_get(K,V) (-1)
#define runtime_cache_insert(K,V)
#define runtime_cache_remove(K)
#endif

#endif
