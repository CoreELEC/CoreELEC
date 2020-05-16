/*
 * Copyright (c) 2017 Franz-Josef Haider <f_haider@gmx.at>
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

#include "wrappers.h"
#include "wrapper_code.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/mman.h>
#include <stdint.h>
#include <assert.h>

static int trace_dynhooked = 0;
static int trace_unhooked = 0;
static int trace_hooked = 0;
static int trace_env_checked = 0;

struct wrapper {
    char *name;
    void *ptr;
    unsigned int size;
    int type;
    struct wrapper *next;
};

struct wrapper *wrappers = NULL;

int wrapper_cmp(void *a, void *b) {
    struct wrapper *key = (struct wrapper*)a;
    struct wrapper *elem = (struct wrapper*)b;
    int c = strcmp(key->name, elem->name);
    if(0 == c) {
        return (key->type - elem->type);
    }
    return c;
}

void register_wrapper(void *wrapper_ptr, unsigned int size, const char *name, int type)
{
    struct wrapper *it = wrappers;
    struct wrapper *last = NULL;
    while(NULL != it)
    {
        last = it;
        it = it->next;
    }
    struct wrapper *w = (struct wrapper*)malloc(sizeof(struct wrapper));
    w->name = (char*)name;
    w->ptr = wrapper_ptr;
    w->type = type;
    w->size = size;
    w->next = NULL;
    if(NULL == last)
    {
        wrappers = w;
    }
    else
    {
        last->next = w;
    }
}

void trace_callback(void *function, char *name, char *msg)
{
    fprintf(stderr, "%s: %s@%p\n", msg, name, function);
}

const char *msg_unhooked = "calling unhooked method";
const char *msg_dynhook = "calling dynamically loaded method";
const char *msg_hooked = "calling hooked method";

static size_t
get_wrapper_code_size(void *wrapper)
{
    // Find first occurence of 0xFFFFFFFF in the code object,
    // which is the placeholder for the attached data words
    uint32_t *ptr = wrapper;
    while (*ptr != 0xFFFFFFFF) {
        ptr++;
    }
    return ((void *)ptr - (void *)wrapper);
}

void *create_wrapper(const char *symbol, void *function, int wrapper_type)
{
    size_t wrapper_size = 0;
    void *wrapper_code = (void*)((uint32_t)wrapper_code_generic & 0xFFFFFFFE);
    void *wrapper_addr = NULL;
    int helper = 0;

    const char *msg = NULL;

    if(!trace_env_checked)
    {
        trace_hooked = getenv("HYBRIS_TRACE_HOOKED") ? atoi(getenv("HYBRIS_TRACE_HOOKED")) : 0;
        trace_unhooked = getenv("HYBRIS_TRACE_UNHOOKED") ? atoi(getenv("HYBRIS_TRACE_UNHOOKED")) : 0;
        trace_dynhooked = getenv("HYBRIS_TRACE_DYNHOOKED") ? atoi(getenv("HYBRIS_TRACE_DYNHOOKED")) : 0;

        trace_env_checked = 1;
    }

    switch(wrapper_type)
    {
        case WRAPPER_HOOKED:
            if(!trace_hooked) return function;
            msg = msg_hooked;
            break;
        case WRAPPER_UNHOOKED:
            if(!trace_unhooked) return function;
            msg = msg_unhooked;
            break;
        case WRAPPER_DYNHOOK:
            if(!trace_dynhooked) return function;
            msg = msg_dynhook;
            break;
        default:
            assert(NULL == "ERROR: invalid wrapper type!\n");
    };

    wrapper_size = get_wrapper_code_size(wrapper_code);

    // 4 additional longs for data storage, see below
    wrapper_size += 4 * sizeof(uint32_t);

    // reserve memory for the generated wrapper
    wrapper_addr = mmap(NULL, wrapper_size,
        PROT_READ | PROT_WRITE | PROT_EXEC,
        MAP_ANONYMOUS | MAP_PRIVATE,
        0, 0);

    if(MAP_FAILED == wrapper_addr)
    {
        printf("ERROR: failed to create wrapper for %s@%p (mmap failed).\n", symbol, function);
        return function;
    }

    memcpy(wrapper_addr, wrapper_code, wrapper_size);

    // Helper = offset of data fields in wrapper_addr (interpreted as int32_t)
    helper = wrapper_size / sizeof(uint32_t) - 4;

    switch(wrapper_type)
    {
        case WRAPPER_HOOKED:
        case WRAPPER_UNHOOKED:
        case WRAPPER_DYNHOOK:
            ((int32_t*)wrapper_addr)[helper++] = (uint32_t)symbol;
            ((int32_t*)wrapper_addr)[helper++] = (uint32_t)function;
            ((int32_t*)wrapper_addr)[helper++] = (uint32_t)trace_callback;
            ((int32_t*)wrapper_addr)[helper++] = (uint32_t)msg;
            break;
        default:
            assert(0);
            break;
    };

    register_wrapper(wrapper_addr, wrapper_size, symbol, wrapper_type);

    return (void*)wrapper_addr;
}

void release_all_wrappers()
{
    struct wrapper *it = wrappers;
    while(NULL != it)
    {
        struct wrapper *next = it->next;
        munmap(it->ptr,it->size);
        free(it);
        it = next;
    }
}

