/*
 * Copyright (c) 2012 Carsten Munk <carsten.munk@gmail.com>
 * Copyright (c) 2012 Canonical Ltd
 * Copyright (c) 2013 Christophe Chapuis <chris.chapuis@gmail.com>
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

#include "config.h"

#include <hybris/common/binding.h>

#include "hooks_shm.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdio_ext.h>
#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include <malloc.h>
#include <string.h>
#include <inttypes.h>
#include <strings.h>
#include <dlfcn.h>
#include <pthread.h>
#include <sys/xattr.h>
#include <grp.h>
#include <signal.h>
#include <errno.h>
#include <dirent.h>
#include <sys/types.h>
#include <stdarg.h>
#include <wchar.h>
#include <sched.h>
#include <pwd.h>
#include <signal.h>
#include <setjmp.h>
#include <sys/signalfd.h>
#include <sys/uio.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>

#include <linux/futex.h>
#include <sys/syscall.h>
#include <sys/time.h>

#include <netdb.h>
#include <unistd.h>
#include <syslog.h>
#include <locale.h>
#include <sys/syscall.h>
#include <sys/auxv.h>
#include <sys/prctl.h>
#include <sys/uio.h>

#include <sys/mman.h>
#include <libgen.h>
#include <mntent.h>

#include <hybris/properties/properties.h>

// using private implementations
extern int my_property_set(const char *key, const char *value);
extern int my_property_get(const char *key, char *value, const char *default_value);
extern int my_property_list(void (*propfn)(const char *key, const char *value, void *cookie), void *cookie);

#include <hybris/common/hooks.h>

#include <android-config.h>

// this is also used in bionic:
#define bool int

#ifdef WANT_ARM_TRACING
#include "wrappers.h"
#endif

static locale_t hybris_locale;
static int locale_inited = 0;
static hybris_hook_cb hook_callback = NULL;

#ifdef WANT_ARM_TRACING
static void (*_android_linker_init)(int sdk_version, void* (*get_hooked_symbol)(const char*, const char*), int enable_linker_gdb_support, void *(_create_wrapper)(const char*, void*, int)) = NULL;
#else
static void (*_android_linker_init)(int sdk_version, void* (*get_hooked_symbol)(const char*, const char*), int enable_linker_gdb_support) = NULL;
#endif

void *(*_android_dlopen)(const char* filename, int flag) = NULL;
char *(*_android_dlerror)() = NULL;
void *(*_android_dlsym)(void* handle, const char* symbol) = NULL;
void *(*_android_dlvsym)(void* handle, const char* symbol, const char* version) = NULL;
int (*_android_dladdr)(const void* addr, void* info) = NULL;
int (*_android_dlclose)(void* handle) = NULL;
void *(*_android_dl_unwind_find_exidx)(void *pc, int* pcount) = NULL;
int (*_android_dl_iterate_phdr)(int (*cb)(void* info, size_t size, void* data), void* data) = NULL;
void (*_android_get_LD_LIBRARY_PATH)(char* buffer, size_t buffer_size) = NULL;
void (*_android_update_LD_LIBRARY_PATH)(const char* ld_library_path) = NULL;
void *(*_android_dlopen_ext)(const char* filename, int flag, const void* extinfo) = NULL;
void (*_android_set_application_target_sdk_version)(uint32_t target) = NULL;
uint32_t (*_android_get_application_target_sdk_version)() = NULL;
void *(*_android_create_namespace)(const char* name,
                                 const char* ld_library_path,
                                 const char* default_library_path,
                                 uint64_t type,
                                 const char* permitted_when_isolated_path,
                                 void* parent) = NULL;
bool (*_android_init_anonymous_namespace)(const char* shared_libs_sonames,
                                      const char* library_search_path) = NULL;
void (*_android_dlwarning)(void* obj, void (*f)(void*, const char*)) = NULL;
void *(*_android_get_exported_namespace)(const char* name) = NULL;

/* TODO:
*  - Check if the int arguments at attr_set/get match the ones at Android
*  - Check how to deal with memory leaks (specially with static initializers)
*  - Check for shared rwlock
*/

/* Base address to check for Android specifics */
#define ANDROID_TOP_ADDR_VALUE_MUTEX  0xFFFF
#define ANDROID_TOP_ADDR_VALUE_COND   0xFFFF
#define ANDROID_TOP_ADDR_VALUE_RWLOCK 0xFFFF

#define ANDROID_MUTEX_SHARED_MASK      0x2000
#define ANDROID_COND_SHARED_MASK       0x0001
#define ANDROID_COND_COUNTER_INCREMENT 0x0002
#define ANDROID_COND_COUNTER_MASK      (~ANDROID_COND_SHARED_MASK)
#define ANDROID_RWLOCKATTR_SHARED_MASK 0x0010

#define ANDROID_COND_IS_SHARED(c)  (((c)->value & ANDROID_COND_SHARED_MASK) != 0)

/* For the static initializer types */
#define ANDROID_PTHREAD_MUTEX_INITIALIZER            0
#define ANDROID_PTHREAD_RECURSIVE_MUTEX_INITIALIZER  0x4000
#define ANDROID_PTHREAD_ERRORCHECK_MUTEX_INITIALIZER 0x8000
#define ANDROID_PTHREAD_COND_INITIALIZER             0
#define ANDROID_PTHREAD_RWLOCK_INITIALIZER           0

#define MALI_HIST_DUMP_THREAD_NAME "mali-hist-dump"

/* Debug */
#include "logging.h"
#define LOGD(message, ...) HYBRIS_DEBUG_LOG(HOOKS, message, ##__VA_ARGS__)

#define TRACE_HOOK(message, ...) \
        HYBRIS_DEBUG_LOG(HOOKS, message, ##__VA_ARGS__);

/*
 * symbols that can be hooked directly shall use HOOK_DIRECT
 * - during debug they will be redirected to a function that traces the calls
 * - during normal execution they will be redirected to the glibc equivalent
 */
#define HOOK_DIRECT(symbol) {#symbol, symbol, _hybris_hook_##symbol}

/*
 * same as above but for symbols who have no dedicated debug wrapper
 */
#define HOOK_DIRECT_NO_DEBUG(symbol) {#symbol, symbol, symbol}

/*
 * symbols that can only be hooked directly to a (symbol with a different name)
 * shall use HOOK_TO
 */
#define HOOK_TO(symbol, hook) {#symbol, hook, hook}

/*
 * symbols that can only be hooked indirectly shall use HOOK
 */
#define HOOK_INDIRECT(symbol) {#symbol, _hybris_hook_##symbol, _hybris_hook_##symbol}

/* we have a value p:
 *  - if p <= ANDROID_TOP_ADDR_VALUE_MUTEX then it is an android mutex, not one we processed
 *  - if p > VMALLOC_END, then the pointer is not a result of malloc ==> it is an shm offset
 */

struct _hook {
    const char *name;
    void *func;
    void *debug_func;
};

/* pthread cond struct as done in Android */
typedef struct {
    int volatile value;
} android_cond_t;

/* Helpers */
static int hybris_check_android_shared_mutex(uintptr_t mutex_addr)
{
    /* If not initialized or initialized by Android, it should contain a low
     * address, which is basically just the int values for Android's own
     * pthread_mutex_t */
    if ((mutex_addr <= ANDROID_TOP_ADDR_VALUE_MUTEX) &&
                    (mutex_addr & ANDROID_MUTEX_SHARED_MASK))
        return 1;

    return 0;
}

static int hybris_check_android_shared_cond(uintptr_t cond_addr)
{
    /* If not initialized or initialized by Android, it should contain a low
     * address, which is basically just the int values for Android's own
     * pthread_cond_t */
    if ((cond_addr <= ANDROID_TOP_ADDR_VALUE_COND) &&
                    (cond_addr & ANDROID_COND_SHARED_MASK))
        return 1;

    /* In case android is setting up cond_addr with a negative value,
     * used for error control */
    if (cond_addr > HYBRIS_SHM_MASK_TOP)
        return 1;

    return 0;
}

/* Based on Android's Bionic pthread implementation.
 * This is just needed when we have a shared cond with Android */
static int __android_pthread_cond_pulse(android_cond_t *cond, int counter)
{
    long flags;
    int fret;

    if (cond == NULL)
        return EINVAL;

    flags = (cond->value & ~ANDROID_COND_COUNTER_MASK);
    for (;;) {
        long oldval = cond->value;
        long newval = 0;
        /* In our case all we need to do is make sure the negative value
         * is under our range, which is the last 0xF from SHM_MASK */
        if (oldval < -12)
            newval = ((oldval + ANDROID_COND_COUNTER_INCREMENT) &
                            ANDROID_COND_COUNTER_MASK) | flags;
        else
            newval = ((oldval - ANDROID_COND_COUNTER_INCREMENT) &
                            ANDROID_COND_COUNTER_MASK) | flags;
        if (__sync_bool_compare_and_swap(&cond->value, oldval, newval))
            break;
    }

    int pshared = cond->value & ANDROID_COND_SHARED_MASK;
    fret = syscall(SYS_futex , &cond->value,
                   pshared ? FUTEX_WAKE : FUTEX_WAKE_PRIVATE, counter,
                   NULL, NULL, NULL);
    (void)fret;
    LOGD("futex based pthread_cond_*, value %d, counter %d, ret %d",
                                            cond->value, counter, fret);
    return 0;
}

int android_pthread_cond_broadcast(android_cond_t *cond)
{
    return __android_pthread_cond_pulse(cond, INT_MAX);
}

int android_pthread_cond_signal(android_cond_t *cond)
{
    return __android_pthread_cond_pulse(cond, 1);
}

static void hybris_set_mutex_attr(unsigned int android_value, pthread_mutexattr_t *attr)
{
    /* Init already sets as PTHREAD_MUTEX_NORMAL */
    pthread_mutexattr_init(attr);

    if (android_value & ANDROID_PTHREAD_RECURSIVE_MUTEX_INITIALIZER) {
        pthread_mutexattr_settype(attr, PTHREAD_MUTEX_RECURSIVE);
    } else if (android_value & ANDROID_PTHREAD_ERRORCHECK_MUTEX_INITIALIZER) {
        pthread_mutexattr_settype(attr, PTHREAD_MUTEX_ERRORCHECK);
    }
}

static pthread_mutex_t* hybris_alloc_init_mutex(unsigned int android_mutex)
{
    pthread_mutex_t *realmutex = malloc(sizeof(pthread_mutex_t));
    pthread_mutexattr_t attr;
    hybris_set_mutex_attr(android_mutex, &attr);
    pthread_mutex_init(realmutex, &attr);
    return realmutex;
}

static pthread_cond_t* hybris_alloc_init_cond(void)
{
    pthread_cond_t *realcond = malloc(sizeof(pthread_cond_t));
    pthread_condattr_t attr;
    pthread_condattr_init(&attr);
    pthread_cond_init(realcond, &attr);
    return realcond;
}

static pthread_rwlock_t* hybris_alloc_init_rwlock(void)
{
    pthread_rwlock_t *realrwlock = malloc(sizeof(pthread_rwlock_t));
    pthread_rwlockattr_t attr;
    pthread_rwlockattr_init(&attr);
    pthread_rwlock_init(realrwlock, &attr);
    return realrwlock;
}

/*
 * utils, such as malloc, memcpy
 *
 * Useful to handle hacks such as the one applied for Nvidia, and to
 * avoid crashes. Also we need to hook all memory allocation related
 * ones to make sure all are using the same allocator implementation.
 *
 * */

static void *_hybris_hook_malloc(size_t size)
{
    TRACE_HOOK("size %zu", size);

#ifdef WANT_ADRENO_QUIRKS
    if(size == 4) size = 5;
#endif

    void *res = malloc(size);

    TRACE_HOOK("res %p", res);

    return res;
}

static size_t _hybris_hook_malloc_usable_size (void *ptr)
{
    TRACE_HOOK("ptr %p", ptr);

    return malloc_usable_size(ptr);
}

static void *_hybris_hook_memcpy(void *dst, const void *src, size_t len)
{
    TRACE_HOOK("dst %p src %p len %zu", dst, src, len);

    if (src == NULL || dst == NULL)
        return dst;

    return memcpy(dst, src, len);
}

static int _hybris_hook_memcmp(const void *s1, const void *s2, size_t n)
{
    TRACE_HOOK("s1 %p '%s' s2 %p '%s' n %zu", s1, (char*) s1, s2, (char*) s2, n);

    return memcmp(s1, s2, n);
}

static size_t _hybris_hook_strlen(const char *s)
{
    TRACE_HOOK("s '%s'", s);

    if (s == NULL)
        return -1;

    return strlen(s);
}

static pid_t _hybris_hook_gettid(void)
{
    TRACE_HOOK("");

    return syscall(__NR_gettid);
}

/*
 * Main pthread functions
 *
 * Custom implementations to workaround difference between Bionic and Glibc.
 * Our own pthread_create helps avoiding direct handling of TLS.
 *
 * */

static int _hybris_hook_pthread_create(pthread_t *thread, const pthread_attr_t *__attr,
                             void *(*start_routine)(void*), void *arg)
{
    pthread_attr_t *realattr = NULL;

    TRACE_HOOK("thread %p attr %p", thread, __attr);

    if (__attr != NULL)
        realattr = (pthread_attr_t *) *(uintptr_t *) __attr;

    return pthread_create(thread, realattr, start_routine, arg);
}

static int _hybris_hook_pthread_kill(pthread_t thread, int sig)
{
    TRACE_HOOK("thread %llu sig %d", (unsigned long long) thread, sig);

    if (thread == 0)
        return ESRCH;

    return pthread_kill(thread, sig);
}

static int _hybris_hook_pthread_setspecific(pthread_key_t key, const void *ptr)
{
    TRACE_HOOK("key %d ptr %" PRIdPTR, key, (intptr_t) ptr);

    return pthread_setspecific(key, ptr);
}

static void* _hybris_hook_pthread_getspecific(pthread_key_t key)
{
    TRACE_HOOK("key %d", key);

    // see android_bionic/tests/pthread_test.cpp, test static_pthread_key_used_before_creation
    if(!key) return NULL;

    return pthread_getspecific(key);
}

/*
 * pthread_attr_* functions
 *
 * Specific implementations to workaround the differences between at the
 * pthread_attr_t struct differences between Bionic and Glibc.
 *
 * */

static int _hybris_hook_pthread_attr_init(pthread_attr_t *__attr)
{
    pthread_attr_t *realattr;

    TRACE_HOOK("attr %p", __attr);

    realattr = malloc(sizeof(pthread_attr_t));
    *((uintptr_t *)__attr) = (uintptr_t) realattr;

    return pthread_attr_init(realattr);
}

static int _hybris_hook_pthread_attr_destroy(pthread_attr_t *__attr)
{
    int ret;
    pthread_attr_t *realattr = (pthread_attr_t *) *(uintptr_t *) __attr;

    TRACE_HOOK("attr %p", __attr);

    ret = pthread_attr_destroy(realattr);
    /* We need to release the memory allocated at _hybris_hook_pthread_attr_init
     * Possible side effects if destroy is called without our init */
    free(realattr);

    return ret;
}

static int _hybris_hook_pthread_attr_setdetachstate(pthread_attr_t *__attr, int state)
{
    pthread_attr_t *realattr = (pthread_attr_t *) *(uintptr_t *) __attr;

    TRACE_HOOK("attr %p state %d", __attr, state);

    return pthread_attr_setdetachstate(realattr, state);
}

static int _hybris_hook_pthread_attr_getdetachstate(pthread_attr_t const *__attr, int *state)
{
    pthread_attr_t *realattr = (pthread_attr_t *) *(uintptr_t *) __attr;

    TRACE_HOOK("attr %p state %p", __attr, state);

    return pthread_attr_getdetachstate(realattr, state);
}

static int _hybris_hook_pthread_attr_setschedpolicy(pthread_attr_t *__attr, int policy)
{
    pthread_attr_t *realattr = (pthread_attr_t *) *(uintptr_t *) __attr;

    TRACE_HOOK("attr %p policy %d", __attr, policy);

    return pthread_attr_setschedpolicy(realattr, policy);
}

static int _hybris_hook_pthread_attr_getschedpolicy(pthread_attr_t const *__attr, int *policy)
{
    pthread_attr_t *realattr = (pthread_attr_t *) *(uintptr_t *) __attr;

    TRACE_HOOK("attr %p policy %p", __attr, policy);

    return pthread_attr_getschedpolicy(realattr, policy);
}

static int _hybris_hook_pthread_attr_setschedparam(pthread_attr_t *__attr, struct sched_param const *param)
{
    pthread_attr_t *realattr = (pthread_attr_t *) *(uintptr_t *) __attr;

    TRACE_HOOK("attr %p param %p", __attr, param);

    return pthread_attr_setschedparam(realattr, param);
}

static int _hybris_hook_pthread_attr_getschedparam(pthread_attr_t const *__attr, struct sched_param *param)
{
    pthread_attr_t *realattr = (pthread_attr_t *) *(uintptr_t *) __attr;

    TRACE_HOOK("attr %p param %p", __attr, param);

    return pthread_attr_getschedparam(realattr, param);
}

static int _hybris_hook_pthread_attr_setstacksize(pthread_attr_t *__attr, size_t stack_size)
{
    pthread_attr_t *realattr = (pthread_attr_t *) *(uintptr_t *) __attr;

    TRACE_HOOK("attr %p stack size %zu", __attr, stack_size);

    return pthread_attr_setstacksize(realattr, stack_size);
}

static int _hybris_hook_pthread_attr_getstacksize(pthread_attr_t const *__attr, size_t *stack_size)
{
    pthread_attr_t *realattr = (pthread_attr_t *) *(uintptr_t *) __attr;

    TRACE_HOOK("attr %p stack size %p", __attr, stack_size);

    return pthread_attr_getstacksize(realattr, stack_size);
}

static int _hybris_hook_pthread_attr_setstackaddr(pthread_attr_t *__attr, void *stack_addr)
{
    pthread_attr_t *realattr = (pthread_attr_t *) *(uintptr_t *) __attr;

    TRACE_HOOK("attr %p stack addr %p", __attr, stack_addr);

    return pthread_attr_setstackaddr(realattr, stack_addr);
}

static int _hybris_hook_pthread_attr_getstackaddr(pthread_attr_t const *__attr, void **stack_addr)
{
    pthread_attr_t *realattr = (pthread_attr_t *) *(uintptr_t *) __attr;

    TRACE_HOOK("attr %p stack addr %p", __attr, stack_addr);

    return pthread_attr_getstackaddr(realattr, stack_addr);
}

static int _hybris_hook_pthread_attr_setstack(pthread_attr_t *__attr, void *stack_base, size_t stack_size)
{
    pthread_attr_t *realattr = (pthread_attr_t *) *(uintptr_t *) __attr;

    TRACE_HOOK("attr %p stack base %p stack size %zu", __attr,
               stack_base, stack_size);

    return pthread_attr_setstack(realattr, stack_base, stack_size);
}

static int _hybris_hook_pthread_attr_getstack(pthread_attr_t const *__attr, void **stack_base, size_t *stack_size)
{
    pthread_attr_t *realattr = (pthread_attr_t *) *(uintptr_t *) __attr;

    TRACE_HOOK("attr %p stack base %p stack size %p", __attr,
               stack_base, stack_size);

    return pthread_attr_getstack(realattr, stack_base, stack_size);
}

static int _hybris_hook_pthread_attr_setguardsize(pthread_attr_t *__attr, size_t guard_size)
{
    pthread_attr_t *realattr = (pthread_attr_t *) *(uintptr_t *) __attr;

    TRACE_HOOK("attr %p guard size %zu", __attr, guard_size);

    return pthread_attr_setguardsize(realattr, guard_size);
}

static int _hybris_hook_pthread_attr_getguardsize(pthread_attr_t const *__attr, size_t *guard_size)
{
    pthread_attr_t *realattr = (pthread_attr_t *) *(uintptr_t *) __attr;

    TRACE_HOOK("attr %p guard size %p", __attr, guard_size);

    return pthread_attr_getguardsize(realattr, guard_size);
}

static int _hybris_hook_pthread_attr_setscope(pthread_attr_t *__attr, int scope)
{
    pthread_attr_t *realattr = (pthread_attr_t *) *(uintptr_t *) __attr;

    TRACE_HOOK("attr %p scope %d", __attr, scope);

    return pthread_attr_setscope(realattr, scope);
}

static int _hybris_hook_pthread_attr_getscope(pthread_attr_t const *__attr)
{
    int scope;
    pthread_attr_t *realattr = (pthread_attr_t *) *(uintptr_t *) __attr;

    TRACE_HOOK("attr %p", __attr);

    /* Android doesn't have the scope attribute because it always
     * returns PTHREAD_SCOPE_SYSTEM */
    pthread_attr_getscope(realattr, &scope);

    return scope;
}

static int _hybris_hook_pthread_getattr_np(pthread_t thid, pthread_attr_t *__attr)
{
    pthread_attr_t *realattr;

    TRACE_HOOK("attr %p", __attr);

    realattr = malloc(sizeof(pthread_attr_t));
    *((uintptr_t *)__attr) = (uintptr_t) realattr;

    return pthread_getattr_np(thid, realattr);
}

/*
 * pthread_mutex* functions
 *
 * Specific implementations to workaround the differences between at the
 * pthread_mutex_t struct differences between Bionic and Glibc.
 *
 * */

static int _hybris_hook_pthread_mutex_init(pthread_mutex_t *__mutex,
                          __const pthread_mutexattr_t *__mutexattr)
{
    pthread_mutex_t *realmutex = NULL;

    TRACE_HOOK("mutex %p attr %p", __mutex, __mutexattr);

    int pshared = 0;
    if (__mutexattr)
        pthread_mutexattr_getpshared(__mutexattr, &pshared);

    if (!pshared) {
        /* non shared, standard mutex: use malloc */
        realmutex = malloc(sizeof(pthread_mutex_t));

        *((uintptr_t *)__mutex) = (uintptr_t) realmutex;
    }
    else {
        /* process-shared mutex: use the shared memory segment */
        hybris_shm_pointer_t handle = hybris_shm_alloc(sizeof(pthread_mutex_t));

        *((hybris_shm_pointer_t *)__mutex) = handle;

        if (handle)
            realmutex = (pthread_mutex_t *)hybris_get_shmpointer(handle);
    }

    return pthread_mutex_init(realmutex, __mutexattr);
}

static int _hybris_hook_pthread_mutex_destroy(pthread_mutex_t *__mutex)
{
    int ret;

    TRACE_HOOK("mutex %p", __mutex);

    if (!__mutex)
        return EINVAL;

    pthread_mutex_t *realmutex = (pthread_mutex_t *) *(uintptr_t *) __mutex;

    if (!realmutex)
        return EINVAL;

    if (!hybris_is_pointer_in_shm((void*)realmutex)) {
        ret = pthread_mutex_destroy(realmutex);
        free(realmutex);
    }
    else {
        realmutex = (pthread_mutex_t *)hybris_get_shmpointer((hybris_shm_pointer_t)realmutex);
        ret = pthread_mutex_destroy(realmutex);
    }

    *((uintptr_t *)__mutex) = 0;

    return ret;
}

static int _hybris_hook_pthread_mutex_lock(pthread_mutex_t *__mutex)
{
    TRACE_HOOK("mutex %p", __mutex);

    if (!__mutex) {
        LOGD("Null mutex lock, not locking.");
        return 0;
    }

    uintptr_t value = (*(uintptr_t *) __mutex);
    if (hybris_check_android_shared_mutex(value)) {
        LOGD("Shared mutex with Android, not locking.");
        return 0;
    }

    pthread_mutex_t *realmutex = (pthread_mutex_t *) value;
    if (hybris_is_pointer_in_shm((void*)value))
        realmutex = (pthread_mutex_t *)hybris_get_shmpointer((hybris_shm_pointer_t)value);

    if (value <= ANDROID_TOP_ADDR_VALUE_MUTEX) {
        TRACE("value %p <= ANDROID_TOP_ADDR_VALUE_MUTEX 0x%x",
              (void*) value, ANDROID_TOP_ADDR_VALUE_MUTEX);
        realmutex = hybris_alloc_init_mutex(value);
        *((uintptr_t *)__mutex) = (uintptr_t) realmutex;
    }

    return pthread_mutex_lock(realmutex);
}

static int _hybris_hook_pthread_mutex_trylock(pthread_mutex_t *__mutex)
{
    uintptr_t value = (*(uintptr_t *) __mutex);

    TRACE_HOOK("mutex %p", __mutex);

    if (hybris_check_android_shared_mutex(value)) {
        LOGD("Shared mutex with Android, not try locking.");
        return 0;
    }

    pthread_mutex_t *realmutex = (pthread_mutex_t *) value;
    if (hybris_is_pointer_in_shm((void*)value))
        realmutex = (pthread_mutex_t *)hybris_get_shmpointer((hybris_shm_pointer_t)value);

    if (value <= ANDROID_TOP_ADDR_VALUE_MUTEX) {
        realmutex = hybris_alloc_init_mutex(value);
        *((uintptr_t *)__mutex) = (uintptr_t) realmutex;
    }

    return pthread_mutex_trylock(realmutex);
}

static int _hybris_hook_pthread_mutex_unlock(pthread_mutex_t *__mutex)
{
    TRACE_HOOK("mutex %p", __mutex);

    if (!__mutex) {
        LOGD("Null mutex lock, not unlocking.");
        return 0;
    }

    uintptr_t value = (*(uintptr_t *) __mutex);
    if (hybris_check_android_shared_mutex(value)) {
        LOGD("Shared mutex with Android, not unlocking.");
        return 0;
    }

    if (value <= ANDROID_TOP_ADDR_VALUE_MUTEX) {
        LOGD("Trying to unlock a lock that's not locked/initialized"
               " by Hybris, not unlocking.");
        return 0;
    }

    pthread_mutex_t *realmutex = (pthread_mutex_t *) value;
    if (hybris_is_pointer_in_shm((void*)value))
        realmutex = (pthread_mutex_t *)hybris_get_shmpointer((hybris_shm_pointer_t)value);

    return pthread_mutex_unlock(realmutex);
}

static int _hybris_hook_pthread_mutex_lock_timeout_np(pthread_mutex_t *__mutex, unsigned __msecs)
{
    struct timespec tv;
    pthread_mutex_t *realmutex;
    uintptr_t value = (*(uintptr_t *) __mutex);

    TRACE_HOOK("mutex %p msecs %u", __mutex, __msecs);

    if (hybris_check_android_shared_mutex(value)) {
        LOGD("Shared mutex with Android, not lock timeout np.");
        return 0;
    }

    realmutex = (pthread_mutex_t *) value;

    if (value <= ANDROID_TOP_ADDR_VALUE_MUTEX) {
        realmutex = hybris_alloc_init_mutex(value);
        *((uintptr_t *)__mutex) = (uintptr_t) realmutex;
    }

    clock_gettime(CLOCK_REALTIME, &tv);
    tv.tv_sec += __msecs/1000;
    tv.tv_nsec += (__msecs % 1000) * 1000000;
    if (tv.tv_nsec >= 1000000000) {
      tv.tv_sec++;
      tv.tv_nsec -= 1000000000;
    }

    return pthread_mutex_timedlock(realmutex, &tv);
}

static int _hybris_hook_pthread_mutex_timedlock(pthread_mutex_t *__mutex,
                                      const struct timespec *__abs_timeout)
{
    TRACE_HOOK("mutex %p abs timeout %p", __mutex, __abs_timeout);

    if (!__mutex) {
        LOGD("Null mutex lock, not unlocking.");
        return 0;
    }

    uintptr_t value = (*(uintptr_t *) __mutex);
    if (hybris_check_android_shared_mutex(value)) {
        LOGD("Shared mutex with Android, not lock timeout np.");
        return 0;
    }

    pthread_mutex_t *realmutex = (pthread_mutex_t *) value;
    if (value <= ANDROID_TOP_ADDR_VALUE_MUTEX) {
        realmutex = hybris_alloc_init_mutex(value);
        *((uintptr_t *)__mutex) = (uintptr_t) realmutex;
    }

    return pthread_mutex_timedlock(realmutex, __abs_timeout);
}

static int _hybris_hook_pthread_mutexattr_setpshared(pthread_mutexattr_t *__attr,
                                           int pshared)
{
    TRACE_HOOK("attr %p pshared %d", __attr, pshared);

    return pthread_mutexattr_setpshared(__attr, pshared);
}

/*
 * pthread_cond* functions
 *
 * Specific implementations to workaround the differences between at the
 * pthread_cond_t struct differences between Bionic and Glibc.
 *
 * */

static int _hybris_hook_pthread_cond_init(pthread_cond_t *cond,
                                const pthread_condattr_t *attr)
{
    pthread_cond_t *realcond = NULL;

    TRACE_HOOK("cond %p attr %p", cond, attr);

    int pshared = 0;

    if (attr)
        pthread_condattr_getpshared(attr, &pshared);

    if (!pshared) {
        /* non shared, standard cond: use malloc */
        realcond = malloc(sizeof(pthread_cond_t));

        *((uintptr_t *) cond) = (uintptr_t) realcond;
    }
    else {
        /* process-shared condition: use the shared memory segment */
        hybris_shm_pointer_t handle = hybris_shm_alloc(sizeof(pthread_cond_t));

        *((uintptr_t *)cond) = (uintptr_t) handle;

        if (handle)
            realcond = (pthread_cond_t *)hybris_get_shmpointer(handle);
    }

    return pthread_cond_init(realcond, attr);
}

static int _hybris_hook_pthread_cond_destroy(pthread_cond_t *cond)
{
    int ret;
    pthread_cond_t *realcond = (pthread_cond_t *) *(uintptr_t *) cond;

    TRACE_HOOK("cond %p", cond);

    if (!realcond) {
      return EINVAL;
    }

    if (!hybris_is_pointer_in_shm((void*)realcond)) {
        ret = pthread_cond_destroy(realcond);
        free(realcond);
    }
    else {
        realcond = (pthread_cond_t *)hybris_get_shmpointer((hybris_shm_pointer_t)realcond);
        ret = pthread_cond_destroy(realcond);
    }

    *((uintptr_t *)cond) = 0;

    return ret;
}

static int _hybris_hook_pthread_cond_broadcast(pthread_cond_t *cond)
{
    uintptr_t value = (*(uintptr_t *) cond);

    TRACE_HOOK("cond %p", cond);

    if (hybris_check_android_shared_cond(value)) {
        LOGD("Shared condition with Android, broadcasting with futex.");
        return android_pthread_cond_broadcast((android_cond_t *) cond);
    }

    pthread_cond_t *realcond = (pthread_cond_t *) value;
    if (hybris_is_pointer_in_shm((void*)value))
        realcond = (pthread_cond_t *)hybris_get_shmpointer((hybris_shm_pointer_t)value);

    if (value <= ANDROID_TOP_ADDR_VALUE_COND) {
        realcond = hybris_alloc_init_cond();
        *((uintptr_t *) cond) = (uintptr_t) realcond;
    }

    return pthread_cond_broadcast(realcond);
}

static int _hybris_hook_pthread_cond_signal(pthread_cond_t *cond)
{
    uintptr_t value = (*(uintptr_t *) cond);

    TRACE_HOOK("cond %p", cond);

    if (hybris_check_android_shared_cond(value)) {
        LOGD("Shared condition with Android, broadcasting with futex.");
        return android_pthread_cond_signal((android_cond_t *) cond);
    }

    pthread_cond_t *realcond = (pthread_cond_t *) value;
    if (hybris_is_pointer_in_shm((void*)value))
        realcond = (pthread_cond_t *)hybris_get_shmpointer((hybris_shm_pointer_t)value);

    if (value <= ANDROID_TOP_ADDR_VALUE_COND) {
        realcond = hybris_alloc_init_cond();
        *((uintptr_t *) cond) = (uintptr_t) realcond;
    }

    return pthread_cond_signal(realcond);
}

static int _hybris_hook_pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)
{
    /* Both cond and mutex can be statically initialized, check for both */
    uintptr_t cvalue = (*(uintptr_t *) cond);
    uintptr_t mvalue = (*(uintptr_t *) mutex);

    TRACE_HOOK("cond %p mutex %p", cond, mutex);

    if (hybris_check_android_shared_cond(cvalue) ||
        hybris_check_android_shared_mutex(mvalue)) {
        LOGD("Shared condition/mutex with Android, not waiting.");
        return 0;
    }

    pthread_cond_t *realcond = (pthread_cond_t *) cvalue;
    if (hybris_is_pointer_in_shm((void*)cvalue))
        realcond = (pthread_cond_t *)hybris_get_shmpointer((hybris_shm_pointer_t)cvalue);

    if (cvalue <= ANDROID_TOP_ADDR_VALUE_COND) {
        realcond = hybris_alloc_init_cond();
        *((uintptr_t *) cond) = (uintptr_t) realcond;
    }

    pthread_mutex_t *realmutex = (pthread_mutex_t *) mvalue;
    if (hybris_is_pointer_in_shm((void*)mvalue))
        realmutex = (pthread_mutex_t *)hybris_get_shmpointer((hybris_shm_pointer_t)mvalue);

    if (mvalue <= ANDROID_TOP_ADDR_VALUE_MUTEX) {
        realmutex = hybris_alloc_init_mutex(mvalue);
        *((uintptr_t *) mutex) = (uintptr_t) realmutex;
    }

    return pthread_cond_wait(realcond, realmutex);
}

static int _hybris_hook_pthread_cond_timedwait(pthread_cond_t *cond,
                pthread_mutex_t *mutex, const struct timespec *abstime)
{
    /* Both cond and mutex can be statically initialized, check for both */
    uintptr_t cvalue = (*(uintptr_t *) cond);
    uintptr_t mvalue = (*(uintptr_t *) mutex);

    TRACE_HOOK("cond %p mutex %p abstime %p", cond, mutex, abstime);

    if (hybris_check_android_shared_cond(cvalue) ||
         hybris_check_android_shared_mutex(mvalue)) {
        LOGD("Shared condition/mutex with Android, not waiting.");
        return 0;
    }

    pthread_cond_t *realcond = (pthread_cond_t *) cvalue;
    if (hybris_is_pointer_in_shm((void*)cvalue))
        realcond = (pthread_cond_t *)hybris_get_shmpointer((hybris_shm_pointer_t)cvalue);

    if (cvalue <= ANDROID_TOP_ADDR_VALUE_COND) {
        realcond = hybris_alloc_init_cond();
        *((uintptr_t *) cond) = (uintptr_t) realcond;
    }

    pthread_mutex_t *realmutex = (pthread_mutex_t *) mvalue;
    if (hybris_is_pointer_in_shm((void*)mvalue))
        realmutex = (pthread_mutex_t *)hybris_get_shmpointer((hybris_shm_pointer_t)mvalue);

    if (mvalue <= ANDROID_TOP_ADDR_VALUE_MUTEX) {
        realmutex = hybris_alloc_init_mutex(mvalue);
        *((uintptr_t *) mutex) = (uintptr_t) realmutex;
    }

    return pthread_cond_timedwait(realcond, realmutex, abstime);
}

static int _hybris_hook_pthread_cond_timedwait_relative_np(pthread_cond_t *cond,
                pthread_mutex_t *mutex, const struct timespec *reltime)
{
    /* Both cond and mutex can be statically initialized, check for both */
    uintptr_t cvalue = (*(uintptr_t *) cond);
    uintptr_t mvalue = (*(uintptr_t *) mutex);

    TRACE_HOOK("cond %p mutex %p reltime %p", cond, mutex, reltime);

    if (hybris_check_android_shared_cond(cvalue) ||
         hybris_check_android_shared_mutex(mvalue)) {
        LOGD("Shared condition/mutex with Android, not waiting.");
        return 0;
    }

    pthread_cond_t *realcond = (pthread_cond_t *) cvalue;
    if( hybris_is_pointer_in_shm((void*)cvalue) )
        realcond = (pthread_cond_t *)hybris_get_shmpointer((hybris_shm_pointer_t)cvalue);

    if (cvalue <= ANDROID_TOP_ADDR_VALUE_COND) {
        realcond = hybris_alloc_init_cond();
        *((uintptr_t *) cond) = (uintptr_t) realcond;
    }

    pthread_mutex_t *realmutex = (pthread_mutex_t *) mvalue;
    if (hybris_is_pointer_in_shm((void*)mvalue))
        realmutex = (pthread_mutex_t *)hybris_get_shmpointer((hybris_shm_pointer_t)mvalue);

    if (mvalue <= ANDROID_TOP_ADDR_VALUE_MUTEX) {
        realmutex = hybris_alloc_init_mutex(mvalue);
        *((uintptr_t *) mutex) = (uintptr_t) realmutex;
    }

    struct timespec tv;
    clock_gettime(CLOCK_REALTIME, &tv);
    tv.tv_sec += reltime->tv_sec;
    tv.tv_nsec += reltime->tv_nsec;
    if (tv.tv_nsec >= 1000000000) {
      tv.tv_sec++;
      tv.tv_nsec -= 1000000000;
    }
    return pthread_cond_timedwait(realcond, realmutex, &tv);
}

int _hybris_hook_pthread_setname_np(pthread_t thread, const char *name)
{
    TRACE_HOOK("thread %llu name %s", (unsigned long long) thread, name);

#ifdef MALI_QUIRKS
    if (strcmp(name, MALI_HIST_DUMP_THREAD_NAME) == 0) {
        HYBRIS_DEBUG_LOG(HOOKS, "%s: Found mali-hist-dump thread, killing it ...",
                         __FUNCTION__);

        if (thread != pthread_self()) {
            HYBRIS_DEBUG_LOG(HOOKS, "%s: -> Failed, as calling thread is not mali-hist-dump itself",
                             __FUNCTION__);
            return 0;
        }

        pthread_exit((void*) thread);

        return 0;
    }
#endif

    return pthread_setname_np(thread, name);
}

/*
 * pthread_rwlockattr_* functions
 *
 * Specific implementations to workaround the differences between at the
 * pthread_rwlockattr_t struct differences between Bionic and Glibc.
 *
 * */

static int _hybris_hook_pthread_rwlockattr_init(pthread_rwlockattr_t *__attr)
{
    pthread_rwlockattr_t *realattr;

    TRACE_HOOK("attr %p", __attr);

    realattr = malloc(sizeof(pthread_rwlockattr_t));
    *((uintptr_t *)__attr) = (uintptr_t) realattr;

    return pthread_rwlockattr_init(realattr);
}

static int _hybris_hook_pthread_rwlockattr_destroy(pthread_rwlockattr_t *__attr)
{
    int ret;
    pthread_rwlockattr_t *realattr = (pthread_rwlockattr_t *) *(uintptr_t *) __attr;

    TRACE_HOOK("attr %p", __attr);

    ret = pthread_rwlockattr_destroy(realattr);
    free(realattr);

    return ret;
}

static int _hybris_hook_pthread_rwlockattr_setpshared(pthread_rwlockattr_t *__attr,
                                            int pshared)
{
    pthread_rwlockattr_t *realattr = (pthread_rwlockattr_t *) *(uintptr_t *) __attr;

    TRACE_HOOK("attr %p pshared %d", __attr, pshared);

    return pthread_rwlockattr_setpshared(realattr, pshared);
}

static int _hybris_hook_pthread_rwlockattr_getpshared(pthread_rwlockattr_t *__attr,
                                            int *pshared)
{
    pthread_rwlockattr_t *realattr = (pthread_rwlockattr_t *) *(uintptr_t *) __attr;

    TRACE_HOOK("attr %p pshared %p", __attr, pshared);

    return pthread_rwlockattr_getpshared(realattr, pshared);
}

int _hybris_hook_pthread_rwlockattr_setkind_np(pthread_rwlockattr_t *attr, int pref)
{
    pthread_rwlockattr_t *realattr = (pthread_rwlockattr_t *) *(uintptr_t *) attr;

    TRACE_HOOK("attr %p pref %i", attr, pref);

    return pthread_rwlockattr_setkind_np(realattr, pref);
}

int _hybris_hook_pthread_rwlockattr_getkind_np(const pthread_rwlockattr_t *attr, int *pref)
{
    pthread_rwlockattr_t *realattr = (pthread_rwlockattr_t *) *(uintptr_t *) attr;

    TRACE_HOOK("attr %p pref %p", attr, pref);

    return pthread_rwlockattr_getkind_np(realattr, pref);
}

/*
 * pthread_rwlock_* functions
 *
 * Specific implementations to workaround the differences between at the
 * pthread_rwlock_t struct differences between Bionic and Glibc.
 *
 * */

static int _hybris_hook_pthread_rwlock_init(pthread_rwlock_t *__rwlock,
                                  __const pthread_rwlockattr_t *__attr)
{
    pthread_rwlock_t *realrwlock = NULL;
    pthread_rwlockattr_t *realattr = NULL;
    int pshared = 0;

    TRACE_HOOK("rwlock %p attr %p", __rwlock, __attr);

    if (__attr != NULL)
        realattr = (pthread_rwlockattr_t *) *(uintptr_t *) __attr;

    if (realattr)
        pthread_rwlockattr_getpshared(realattr, &pshared);

    if (!pshared) {
        /* non shared, standard rwlock: use malloc */
        realrwlock = malloc(sizeof(pthread_rwlock_t));

        *((uintptr_t *) __rwlock) = (uintptr_t) realrwlock;
    }
    else {
        /* process-shared condition: use the shared memory segment */
        hybris_shm_pointer_t handle = hybris_shm_alloc(sizeof(pthread_rwlock_t));

        *((uintptr_t *)__rwlock) = (uintptr_t) handle;

        if (handle)
            realrwlock = (pthread_rwlock_t *)hybris_get_shmpointer(handle);
    }

    return pthread_rwlock_init(realrwlock, realattr);
}

static int _hybris_hook_pthread_rwlock_destroy(pthread_rwlock_t *__rwlock)
{
    int ret;
    pthread_rwlock_t *realrwlock = (pthread_rwlock_t *) *(uintptr_t *) __rwlock;

    TRACE_HOOK("rwlock %p", __rwlock);

    if (!hybris_is_pointer_in_shm((void*)realrwlock)) {
        ret = pthread_rwlock_destroy(realrwlock);
        free(realrwlock);
    }
    else {
        ret = pthread_rwlock_destroy(realrwlock);
        realrwlock = (pthread_rwlock_t *)hybris_get_shmpointer((hybris_shm_pointer_t)realrwlock);
    }

    return ret;
}

static pthread_rwlock_t* hybris_set_realrwlock(pthread_rwlock_t *rwlock)
{
    uintptr_t value = (*(uintptr_t *) rwlock);
    pthread_rwlock_t *realrwlock = (pthread_rwlock_t *) value;

    if (hybris_is_pointer_in_shm((void*)value))
        realrwlock = (pthread_rwlock_t *)hybris_get_shmpointer((hybris_shm_pointer_t)value);

    if ((uintptr_t)realrwlock <= ANDROID_TOP_ADDR_VALUE_RWLOCK) {
        realrwlock = hybris_alloc_init_rwlock();
        *((uintptr_t *)rwlock) = (uintptr_t) realrwlock;
    }
    return realrwlock;
}

static int _hybris_hook_pthread_rwlock_rdlock(pthread_rwlock_t *__rwlock)
{
    TRACE_HOOK("rwlock %p", __rwlock);

    pthread_rwlock_t *realrwlock = hybris_set_realrwlock(__rwlock);

    return pthread_rwlock_rdlock(realrwlock);
}

static int _hybris_hook_pthread_rwlock_tryrdlock(pthread_rwlock_t *__rwlock)
{
    TRACE_HOOK("rwlock %p", __rwlock);

    pthread_rwlock_t *realrwlock = hybris_set_realrwlock(__rwlock);

    return pthread_rwlock_tryrdlock(realrwlock);
}

static int _hybris_hook_pthread_rwlock_timedrdlock(pthread_rwlock_t *__rwlock,
                                         __const struct timespec *abs_timeout)
{
    TRACE_HOOK("rwlock %p abs timeout %p", __rwlock, abs_timeout);

    pthread_rwlock_t *realrwlock = hybris_set_realrwlock(__rwlock);

    return pthread_rwlock_timedrdlock(realrwlock, abs_timeout);
}

static int _hybris_hook_pthread_rwlock_wrlock(pthread_rwlock_t *__rwlock)
{
    TRACE_HOOK("rwlock %p", __rwlock);

    pthread_rwlock_t *realrwlock = hybris_set_realrwlock(__rwlock);

    return pthread_rwlock_wrlock(realrwlock);
}

static int _hybris_hook_pthread_rwlock_trywrlock(pthread_rwlock_t *__rwlock)
{
    TRACE_HOOK("rwlock %p", __rwlock);

    pthread_rwlock_t *realrwlock = hybris_set_realrwlock(__rwlock);

    return pthread_rwlock_trywrlock(realrwlock);
}

static int _hybris_hook_pthread_rwlock_timedwrlock(pthread_rwlock_t *__rwlock,
                                         __const struct timespec *abs_timeout)
{
    TRACE_HOOK("rwlock %p abs timeout %p", __rwlock, abs_timeout);

    pthread_rwlock_t *realrwlock = hybris_set_realrwlock(__rwlock);

    return pthread_rwlock_timedwrlock(realrwlock, abs_timeout);
}

static int _hybris_hook_pthread_rwlock_unlock(pthread_rwlock_t *__rwlock)
{
    uintptr_t value = (*(uintptr_t *) __rwlock);

    TRACE_HOOK("rwlock %p", __rwlock);

    if (value <= ANDROID_TOP_ADDR_VALUE_RWLOCK) {
        LOGD("Trying to unlock a rwlock that's not locked/initialized"
               " by Hybris, not unlocking.");
        return 0;
    }

    pthread_rwlock_t *realrwlock = (pthread_rwlock_t *) value;
    if (hybris_is_pointer_in_shm((void*)value))
        realrwlock = (pthread_rwlock_t *)hybris_get_shmpointer((hybris_shm_pointer_t)value);

    return pthread_rwlock_unlock(realrwlock);
}

/* Bionic implementation of pthread_cleanup_push/pop doesn't support C++ exceptions
   and thread cancelation. We only make sure to call the cleanup routine when
   requested. We duplicate the bionic cleanup struct here for our purposes. */

typedef void (*bionic___pthread_cleanup_func_t)(void*);

typedef struct bionic___pthread_cleanup_t {
    struct bionic___pthread_cleanup_t*  __cleanup_prev;     /* unused */
    bionic___pthread_cleanup_func_t     __cleanup_routine;
    void*                               __cleanup_arg;
} bionic___pthread_cleanup_t;

static void _hybris_hook___pthread_cleanup_push(void *bionic_cleanup, void *routine, void *arg)
{
    bionic___pthread_cleanup_t *cleanup = bionic_cleanup;

    TRACE_HOOK("cleanup %p routine %p arg %p", cleanup, routine, arg);
    cleanup->__cleanup_routine = routine;
    cleanup->__cleanup_arg = arg;
}

static void _hybris_hook___pthread_cleanup_pop(void *bionic_cleanup, int execute)
{
    bionic___pthread_cleanup_t *cleanup = bionic_cleanup;

    TRACE_HOOK("cleanup %p execute %d", cleanup, execute);
    if (execute)
        cleanup->__cleanup_routine(cleanup->__cleanup_arg);
}

#define min(X,Y) (((X) < (Y)) ? (X) : (Y))

static pid_t _hybris_hook_pthread_gettid_np(pthread_t t)
{
    TRACE_HOOK("thread %lu", (unsigned long) t);

    // glibc doesn't offer us a way to retrieve the thread id for a
    // specific thread. However pthread_t is defined as unsigned
    // long int and is the thread id so we can just copy it over
    // into a pid_t.
    pid_t tid;
    memcpy(&tid, &t, min(sizeof(tid), sizeof(t)));
    return tid;
}

static int _hybris_hook___set_errno(int oi_errno)
{
    TRACE_HOOK("errno %d", oi_errno);

    errno = oi_errno;

    return -1;
}

/*
 * __isthreaded is used in bionic's stdio.h to choose between a fast internal implementation
 * and a more classic stdio function call.
 * For example:
 * #define  __sfeof(p)  (((p)->_flags & __SEOF) != 0)
 * #define  feof(p)     (!__isthreaded ? __sfeof(p) : (feof)(p))
 *
 * We see here that if __isthreaded is false, then it will use directly the bionic's FILE structure
 * instead of calling one of the hooked methods.
 * Therefore we need to set __isthreaded to true, even if we are not in a multi-threaded context.
 */
static int _hybris_hook___isthreaded = 1;

/* "struct __sbuf" from bionic/libc/include/stdio.h */
#if defined(__LP64__)
struct bionic_sbuf {
    unsigned char* _base;
    size_t _size;
};
#else
struct bionic_sbuf {
    unsigned char *_base;
    int _size;
};
#endif

typedef off_t bionic_fpos_t;

/* "struct __sFILE" from bionic/libc/include/stdio.h */
struct __attribute__((packed)) bionic_file {
    unsigned char *_p;      /* current position in (some) buffer */
    int _r;                 /* read space left for getc() */
    int _w;                 /* write space left for putc() */
#if defined(__LP64__)
    int _flags;             /* flags, below; this FILE is free if 0 */
    int _file;              /* fileno, if Unix descriptor, else -1 */
#else
    short _flags;           /* flags, below; this FILE is free if 0 */
    short _file;            /* fileno, if Unix descriptor, else -1 */
#endif
    struct bionic_sbuf _bf; /* the buffer (at least 1 byte, if !NULL) */
    int _lbfsize;           /* 0 or -_bf._size, for inline putc */

    /* operations */
    void *_cookie;          /* cookie passed to io functions */
    int (*_close)(void *);
    int (*_read)(void *, char *, int);
    bionic_fpos_t (*_seek)(void *, bionic_fpos_t, int);
    int (*_write)(void *, const char *, int);

    /* extension data, to avoid further ABI breakage */
    struct bionic_sbuf _ext;
    /* data for long sequences of ungetc() */
    unsigned char *_up;     /* saved _p when _p is doing ungetc data */
    int _ur;                /* saved _r when _r is counting ungetc data */

    /* tricks to meet minimum requirements even when malloc() fails */
    unsigned char _ubuf[3]; /* guarantee an ungetc() buffer */
    unsigned char _nbuf[1]; /* guarantee a getc() buffer */

    /* separate buffer for fgetln() when line crosses buffer boundary */
    struct bionic_sbuf _lb; /* buffer for fgetln() */

    /* Unix stdio files get aligned to block boundaries on fseek() */
    int _blksize;           /* stat.st_blksize (may be != _bf._size) */
    bionic_fpos_t _offset;         /* current lseek offset */
};

/*
 * redirection for bionic's __sF, which is defined as:
 *   FILE __sF[3];
 *   #define stdin  &__sF[0];
 *   #define stdout &__sF[1];
 *   #define stderr &__sF[2];
 *   So the goal here is to catch the call to file methods where the FILE* pointer
 *   is either stdin, stdout or stderr, and translate that pointer to a valid glibc
 *   pointer.
 *   Currently, only fputs is managed.
 */
static char _hybris_hook_sF[3 * sizeof(struct bionic_file)] = {0};
static FILE *_get_actual_fp(FILE *fp)
{
    char *c_fp = (char*)fp;
    if (c_fp == &_hybris_hook_sF[0])
        return stdin;
    else if (c_fp == &_hybris_hook_sF[sizeof(struct bionic_file)])
        return stdout;
    else if (c_fp == &_hybris_hook_sF[sizeof(struct bionic_file) * 2])
        return stderr;

    return fp;
}

static void _hybris_hook_clearerr(FILE *fp)
{
    TRACE_HOOK("fp %p", fp);

    clearerr(_get_actual_fp(fp));
}

static int _hybris_hook_fclose(FILE *fp)
{
    TRACE_HOOK("fp %p", fp);

    return fclose(_get_actual_fp(fp));
}

static int _hybris_hook_feof(FILE *fp)
{
    TRACE_HOOK("fp %p", fp);

    return feof(_get_actual_fp(fp));
}

static int _hybris_hook_ferror(FILE *fp)
{
    TRACE_HOOK("fp %p", fp);

    return ferror(_get_actual_fp(fp));
}

static int _hybris_hook_fflush(FILE *fp)
{
    TRACE_HOOK("fp %p", fp);

    if(fileno(_get_actual_fp(fp)) < 0) {
        return 0;
    }

    return fflush(_get_actual_fp(fp));
}

static int _hybris_hook_fgetc(FILE *fp)
{
    TRACE_HOOK("fp %p", fp);

    return fgetc(_get_actual_fp(fp));
}

static int _hybris_hook_fgetpos(FILE *fp, bionic_fpos_t *pos)
{
    TRACE_HOOK("fp %p pos %p", fp, pos);

    fpos_t my_fpos;
    int ret = fgetpos(_get_actual_fp(fp), &my_fpos);

    *pos = my_fpos.__pos;

    return ret;
}

static char* _hybris_hook_fgets(char *s, int n, FILE *fp)
{
    TRACE_HOOK("s %s n %d fp %p", s, n, fp);

    return fgets(s, n, _get_actual_fp(fp));
}

FP_ATTRIB static int _hybris_hook_fprintf(FILE *fp, const char *fmt, ...)
{
    int ret = 0;

    TRACE_HOOK("fp %p fmt '%s'", fp, fmt);

    va_list args;
    va_start(args,fmt);
    ret = vfprintf(_get_actual_fp(fp), fmt, args);
    va_end(args);

    return ret;
}

static int _hybris_hook_fputc(int c, FILE *fp)
{
    TRACE_HOOK("c %d fp %p", c, fp);

    return fputc(c, _get_actual_fp(fp));
}

static int _hybris_hook_fputs(const char *s, FILE *fp)
{
    TRACE_HOOK("s '%s' fp %p", s, fp);

    return fputs(s, _get_actual_fp(fp));
}

static size_t _hybris_hook_fread(void *ptr, size_t size, size_t nmemb, FILE *fp)
{
    TRACE_HOOK("ptr %p size %zu nmemb %zu fp %p", ptr, size, nmemb, fp);

    return fread(ptr, size, nmemb, _get_actual_fp(fp));
}

static FILE* _hybris_hook_freopen(const char *filename, const char *mode, FILE *fp)
{
    TRACE_HOOK("filename '%s' mode '%s' fp %p", filename, mode, fp);

    return freopen(filename, mode, _get_actual_fp(fp));
}

FP_ATTRIB static int _hybris_hook_fscanf(FILE *fp, const char *fmt, ...)
{
    int ret = 0;

    TRACE_HOOK("fp %p fmt '%s'", fp, fmt);

    va_list args;
    va_start(args,fmt);
    ret = vfscanf(_get_actual_fp(fp), fmt, args);
    va_end(args);

    return ret;
}

static int _hybris_hook_fseek(FILE *fp, long offset, int whence)
{
    TRACE_HOOK("fp %p offset %ld whence %d", fp, offset, whence);

    return fseek(_get_actual_fp(fp), offset, whence);
}

static int _hybris_hook_fseeko(FILE *fp, off_t offset, int whence)
{
    TRACE_HOOK("fp %p offset %ld whence %d", fp, offset, whence);

    return fseeko(_get_actual_fp(fp), offset, whence);
}

static int _hybris_hook_fsetpos(FILE *fp, const bionic_fpos_t *pos)
{
    TRACE_HOOK("fp %p pos %p", fp, pos);

    fpos_t my_fpos;
    my_fpos.__pos = *pos;
    memset(&my_fpos.__state, 0, sizeof(mbstate_t));

    return fsetpos(_get_actual_fp(fp), &my_fpos);
}

static long _hybris_hook_ftell(FILE *fp)
{
    TRACE_HOOK("fp %p", fp);

    return ftell(_get_actual_fp(fp));
}

static off_t _hybris_hook_ftello(FILE *fp)
{
    TRACE_HOOK("fp %p", fp);

    return ftello(_get_actual_fp(fp));
}

static size_t _hybris_hook_fwrite(const void *ptr, size_t size, size_t nmemb, FILE *fp)
{
    TRACE_HOOK("ptr %p size %zu nmemb %zu fp %p", ptr, size, nmemb, fp);

    return fwrite(ptr, size, nmemb, _get_actual_fp(fp));
}

static int _hybris_hook_getc(FILE *fp)
{
    TRACE_HOOK("fp %p", fp);

    return getc(_get_actual_fp(fp));
}

static ssize_t _hybris_hook_getdelim(char ** lineptr, size_t *n, int delimiter, FILE * fp)
{
    TRACE_HOOK("lineptr %p n %p delimiter %d fp %p", lineptr, n, delimiter, fp);

    return getdelim(lineptr, n, delimiter, _get_actual_fp(fp));
}

static ssize_t _hybris_hook_getline(char **lineptr, size_t *n, FILE *fp)
{
    TRACE_HOOK("lineptr %p n %p fp %p", lineptr, n, fp);

    return getline(lineptr, n, _get_actual_fp(fp));
}

static int _hybris_hook_putc(int c, FILE *fp)
{
    TRACE_HOOK("c %d fp %p", c, fp);

    return putc(c, _get_actual_fp(fp));
}

static void _hybris_hook_rewind(FILE *fp)
{
    TRACE_HOOK("fp %p", fp);

    rewind(_get_actual_fp(fp));
}

static void _hybris_hook_setbuf(FILE *fp, char *buf)
{
    TRACE_HOOK("fp %p buf '%s'", fp, buf);

    setbuf(_get_actual_fp(fp), buf);
}

static int _hybris_hook_setvbuf(FILE *fp, char *buf, int mode, size_t size)
{
    TRACE_HOOK("fp %p buf '%s' mode %d size %zu", fp, buf, mode, size);

    return setvbuf(_get_actual_fp(fp), buf, mode, size);
}

static int _hybris_hook_ungetc(int c, FILE *fp)
{
    TRACE_HOOK("c %d fp %p", c, fp);

    return ungetc(c, _get_actual_fp(fp));
}

static int _hybris_hook_vfprintf(FILE *fp, const char *fmt, va_list arg)
{
    TRACE_HOOK("fp %p fmt '%s'", fp, fmt);

    return vfprintf(_get_actual_fp(fp), fmt, arg);
}

static int _hybris_hook_vfscanf(FILE *fp, const char *fmt, va_list arg)
{
    TRACE_HOOK("fp %p fmt '%s'", fp, fmt);

    return vfscanf(_get_actual_fp(fp), fmt, arg);
}

static int _hybris_hook_fileno(FILE *fp)
{
    TRACE_HOOK("fp %p", fp);

    return fileno(_get_actual_fp(fp));
}

static int _hybris_hook_pclose(FILE *fp)
{
    TRACE_HOOK("fp %p", fp);

    return pclose(_get_actual_fp(fp));
}

static void _hybris_hook_flockfile(FILE *fp)
{
    TRACE_HOOK("fp %p", fp);

    return flockfile(_get_actual_fp(fp));
}

static int _hybris_hook_ftrylockfile(FILE *fp)
{
    TRACE_HOOK("fp %p", fp);

    return ftrylockfile(_get_actual_fp(fp));
}

static void _hybris_hook_funlockfile(FILE *fp)
{
    TRACE_HOOK("fp %p", fp);

    return funlockfile(_get_actual_fp(fp));
}

static int _hybris_hook_getc_unlocked(FILE *fp)
{
    TRACE_HOOK("fp %p", fp);

    return getc_unlocked(_get_actual_fp(fp));
}

static int _hybris_hook_putc_unlocked(int c, FILE *fp)
{
    TRACE_HOOK("fp %p", fp);

    return putc_unlocked(c, _get_actual_fp(fp));
}

/* exists only on the BSD platform
static char* _hybris_hook_fgetln(FILE *fp, size_t *len)
{
    return fgetln(_get_actual_fp(fp), len);
}
*/

static int _hybris_hook_fpurge(FILE *fp)
{
    TRACE_HOOK("fp %p", fp);

    __fpurge(_get_actual_fp(fp));

    return 0;
}

static int _hybris_hook_getw(FILE *fp)
{
    TRACE_HOOK("fp %p", fp);

    return getw(_get_actual_fp(fp));
}

static int _hybris_hook_putw(int w, FILE *fp)
{
    TRACE_HOOK("w %d fp %p", w, fp);

    return putw(w, _get_actual_fp(fp));
}

static void _hybris_hook_setbuffer(FILE *fp, char *buf, int size)
{
    TRACE_HOOK("fp %p buf '%s' size %d", fp, buf, size);

    setbuffer(_get_actual_fp(fp), buf, size);
}

static int _hybris_hook_setlinebuf(FILE *fp)
{
    TRACE_HOOK("fp %p", fp);

    setlinebuf(_get_actual_fp(fp));

    return 0;
}

/* "struct dirent" from bionic/libc/include/dirent.h */
struct bionic_dirent {
    uint64_t         d_ino;
    int64_t          d_off;
    unsigned short   d_reclen;
    unsigned char    d_type;
    char             d_name[256];
};

static struct bionic_dirent *_hybris_hook_readdir(DIR *dirp)
{
    /**
     * readdir(3) manpage says:
     *  The data returned by readdir() may be overwritten by subsequent calls
     *  to readdir() for the same directory stream.
     *
     * XXX: At the moment, for us, the data will be overwritten even by
     * subsequent calls to /different/ directory streams. Eventually fix that
     * (e.g. by storing per-DIR * bionic_dirent structs, and removing them on
     * closedir, requires hooking of all funcs returning/taking DIR *) and
     * handling the additional data attachment there)
     **/

    static struct bionic_dirent result;

    TRACE_HOOK("dirp %p", dirp);

    struct dirent *real_result = readdir(dirp);
    if (!real_result) {
        return NULL;
    }

    result.d_ino = real_result->d_ino;
    result.d_off = real_result->d_off;
    result.d_reclen = real_result->d_reclen;
    result.d_type = real_result->d_type;
    memcpy(result.d_name, real_result->d_name, sizeof(result.d_name));

    // Make sure the string is zero-terminated, even if cut off (which
    // shouldn't happen, as both bionic and glibc have d_name defined
    // as fixed array of 256 chars)
    result.d_name[sizeof(result.d_name)-1] = '\0';
    return &result;
}

static int _hybris_hook_readdir_r(DIR *dir, struct bionic_dirent *entry,
        struct bionic_dirent **result)
{
    struct dirent entry_r;
    struct dirent *result_r;

    TRACE_HOOK("dir %p entry %p result %p", dir, entry, result);

    int res = readdir_r(dir, &entry_r, &result_r);

    if (res == 0) {
        if (result_r != NULL) {
            *result = entry;

            entry->d_ino = entry_r.d_ino;
            entry->d_off = entry_r.d_off;
            entry->d_reclen = entry_r.d_reclen;
            entry->d_type = entry_r.d_type;
            memcpy(entry->d_name, entry_r.d_name, sizeof(entry->d_name));

            // Make sure the string is zero-terminated, even if cut off (which
            // shouldn't happen, as both bionic and glibc have d_name defined
            // as fixed array of 256 chars)
            entry->d_name[sizeof(entry->d_name) - 1] = '\0';
        } else {
            *result = NULL;
        }
    }

    return res;
}

static int _hybris_hook_alphasort(struct bionic_dirent **a,
                                  struct bionic_dirent **b)
{
    return strcoll((*a)->d_name, (*b)->d_name);
}

static int _hybris_hook_versionsort(struct bionic_dirent **a,
                          struct bionic_dirent **b)
{
    return strverscmp((*a)->d_name, (*b)->d_name);
}

static int _hybris_hook_scandirat(int fd, const char *dir,
                      struct bionic_dirent ***namelist,
                      int (*filter) (const struct bionic_dirent *),
                      int (*compar) (const struct bionic_dirent **,
                                     const struct bionic_dirent **))
{
    struct dirent **namelist_r;
    struct bionic_dirent **result;
    struct bionic_dirent *filter_r;

    int i = 0;
    size_t nItems = 0;

    int res = scandirat(fd, dir, &namelist_r, NULL, NULL);

    if (res != 0 && namelist_r != NULL) {

        result = malloc(res * sizeof(struct bionic_dirent));
        if (!result)
            return -1;

        for (i = 0; i < res; i++) {
            filter_r = malloc(sizeof(struct bionic_dirent));
            if (!filter_r) {
                while (i-- > 0)
                        free(result[i]);
                    free(result);
                    return -1;
            }
            filter_r->d_ino = namelist_r[i]->d_ino;
            filter_r->d_off = namelist_r[i]->d_off;
            filter_r->d_reclen = namelist_r[i]->d_reclen;
            filter_r->d_type = namelist_r[i]->d_type;

            strcpy(filter_r->d_name, namelist_r[i]->d_name);
            filter_r->d_name[sizeof(namelist_r[i]->d_name) - 1] = '\0';

            if (filter != NULL && !(*filter)(filter_r)) {//apply filter
                free(filter_r);
                continue;
            }

            result[nItems++] = filter_r;
        }
        if (nItems && compar != NULL) // sort
            qsort(result, nItems, sizeof(struct bionic_dirent *), compar);

        *namelist = result;
    }

    return res;
}

static int _hybris_hook_scandir(const char *dir,
                      struct bionic_dirent ***namelist,
                      int (*filter) (const struct bionic_dirent *),
                      int (*compar) (const struct bionic_dirent **,
                                     const struct bionic_dirent **))
{
    return _hybris_hook_scandirat(AT_FDCWD, dir, namelist, filter, compar);
}

static inline void swap(void **a, void **b)
{
    void *tmp = *a;
    *a = *b;
    *b = tmp;
}

static int _hybris_hook_getaddrinfo(const char *hostname, const char *servname,
    const struct addrinfo *hints, struct addrinfo **res)
{
    struct addrinfo *fixed_hints = NULL;

    TRACE_HOOK("hostname '%s' servname '%s' hints %p res %p",
               hostname, servname, hints, res);

    if (hints) {
        fixed_hints = (struct addrinfo*) malloc(sizeof(struct addrinfo));
        memcpy(fixed_hints, hints, sizeof(struct addrinfo));
        // fix bionic -> glibc missmatch
        swap((void**)&(fixed_hints->ai_canonname), (void**)&(fixed_hints->ai_addr));
    }

    int result = getaddrinfo(hostname, servname, fixed_hints, res);

    if (fixed_hints)
        free(fixed_hints);

    if(result == 0) {
        // fix bionic <- glibc missmatch
        struct addrinfo *it = *res;
        while (it) {
            swap((void**) &(it->ai_canonname), (void**) &(it->ai_addr));
            it = it->ai_next;
        }
    }

    return result;
}

static void _hybris_hook_freeaddrinfo(struct addrinfo *__ai)
{
    TRACE_HOOK("ai %p", __ai);

    if (__ai == NULL)
        return;

    struct addrinfo *it = __ai;
    while (it) {
        swap((void**) &(it->ai_canonname), (void**) &(it->ai_addr));
        it = it->ai_next;
    }

    freeaddrinfo(__ai);
}

extern long _hybris_map_sysconf(int name);

long _hybris_hook_sysconf(int name)
{
    TRACE_HOOK("name %d", name);

    return _hybris_map_sysconf(name);
}

FP_ATTRIB static double _hybris_hook_strtod(const char *nptr, char **endptr)
{
    TRACE_HOOK("nptr '%s' endptr %p", nptr, endptr);

    if (locale_inited == 0) {
            hybris_locale = newlocale(LC_ALL_MASK, "C", 0);
            locale_inited = 1;
    }

    return strtod_l(nptr, endptr, hybris_locale);
}

static long int _hybris_hook_strtol(const char* str, char** endptr, int base)
{
    TRACE_HOOK("str '%s' endptr %p base %i", str, endptr, base);

    return strtol(str, endptr, base);
}

static int _hybris_hook___system_property_read(const void *pi, char *name, char *value)
{
    TRACE_HOOK("pi %p name '%s' value '%s'", pi, name, value);

    return my_property_get(name, value, NULL);
}

static int _hybris_hook___system_property_foreach(void (*propfn)(const void *pi, void *cookie), void *cookie)
{
    TRACE_HOOK("propfn %p cookie %p", propfn, cookie);

    return 0;
}

static const void *_hybris_hook___system_property_find(const char *name)
{
    TRACE_HOOK("name '%s'", name);

    return NULL;
}

static unsigned int _hybris_hook___system_property_serial(const void *pi)
{
    TRACE_HOOK("pi %p", pi);

    return 0;
}

static int _hybris_hook___system_property_wait(const void *pi)
{
    TRACE_HOOK("pi %p", pi);

    return 0;
}

static int _hybris_hook___system_property_update(void *pi, const char *value, unsigned int len)
{
    TRACE_HOOK("pi %p value '%s' len %d", pi, value, len);

    return 0;
}

static int _hybris_hook___system_property_add(const char *name, unsigned int namelen, const char *value, unsigned int valuelen)
{
    TRACE_HOOK("name '%s' namelen %d value '%s' valuelen %d",
               name, namelen, value, valuelen);
    return 0;
}

static unsigned int _hybris_hook___system_property_wait_any(unsigned int serial)
{
    TRACE_HOOK("serial %d", serial);

    return 0;
}

static const void *_hybris_hook___system_property_find_nth(unsigned n)
{
    TRACE_HOOK("n %d", n);

    return NULL;
}

/**
 * NOTE: Normally we don't have to wrap __system_property_get (libc.so) as it is only used
 * through the property_get (libcutils.so) function. However when property_get is used
 * internally in libcutils.so we don't have any chance to hook our replacement in.
 * Therefore we have to hook __system_property_get too and just replace it with the
 * implementation of our internal property handling
 */

int _hybris_hook___system_property_get(const char *name, const char *value)
{
    TRACE_HOOK("name '%s' value '%s'", name, value);

    return my_property_get(name, (char*) value, NULL);
}

int _hybris_hook_property_get(const char *key, char *value, const char *default_value)
{
    TRACE_HOOK("key '%s' value '%s' default value '%s'",
               key, value, default_value);

    return my_property_get(key, value, default_value);
}

int _hybris_hook_property_list(void (*propfn)(const char *key, const char *value, void *cookie), void *cookie)
{
    TRACE_HOOK("propfn '%p' cookie '%p'",
               propfn, cookie);

    return my_property_list(propfn, cookie);
}

int _hybris_hook_property_set(const char *key, const char *value)
{
    TRACE_HOOK("key '%s' value '%s'", key, value);

    return my_property_set(key, value);
}

char *_hybris_hook_getenv(const char *name)
{
    TRACE_HOOK("name '%s'", name);

    return getenv(name);
}

int _hybris_hook_setenv(const char *name, const char *value, int overwrite)
{
    TRACE_HOOK("name '%s' value '%s' overwrite %d", name, value, overwrite);

    return setenv(name, value, overwrite);
}

int _hybris_hook_putenv(char *string)
{
    TRACE_HOOK("string '%s'", string);

    return putenv(string);
}

int _hybris_hook_clearenv(void)
{
    TRACE_HOOK("");

    return clearenv();
}

extern int __cxa_atexit(void (*)(void*), void*, void*);
extern void __cxa_finalize(void * d);

struct open_redirect {
    const char *from;
    const char *to;
};

struct open_redirect open_redirects[] = {
    { "/dev/log/main", "/dev/alog/main" },
    { "/dev/log/radio", "/dev/alog/radio" },
    { "/dev/log/system", "/dev/alog/system" },
    { "/dev/log/events", "/dev/alog/events" },
    { NULL, NULL }
};

int _hybris_hook_open(const char *pathname, int flags, ...)
{
    va_list ap;
    mode_t mode = 0;
    const char *target_path = pathname;

    TRACE_HOOK("pathname '%s' flags %d", pathname, flags);

    if (pathname != NULL) {
            struct open_redirect *entry = &open_redirects[0];
            while (entry->from != NULL) {
                    if (strcmp(pathname, entry->from) == 0) {
                            target_path = entry->to;
                            break;
                    }
                    entry++;
            }
    }

    if (flags & O_CREAT) {
            va_start(ap, flags);
            mode = va_arg(ap, mode_t);
            va_end(ap);
    }

    return open(target_path, flags, mode);
}

/**
 * Wrap some GCC builtin functions, which don't have any address
 */
__THROW int _hybris_hook___sprintf_chk (char *__restrict __s, int __flag, size_t __slen,
			  const char *__restrict __format, ...)
{
    int ret = 0;
    va_list args;
    va_start(args,__format);
    ret = __vsprintf_chk (__s, __flag, __slen, __format, args);
    va_end(args);

    return ret;
}
__THROW int _hybris_hook___snprintf_chk (char *__restrict __s, size_t __n, int __flag,
			   size_t __slen, const char *__restrict __format, ...)
{
    int ret = 0;
    va_list args;
    va_start(args,__format);
    ret = __vsnprintf_chk (__s, __n, __flag, __slen, __format, args);
    va_end(args);

    return ret;
}

static __thread void *tls_hooks[16];

static void *_hybris_hook___get_tls_hooks()
{
    TRACE_HOOK("");
    return tls_hooks;
}

int _hybris_hook_prctl(int option, unsigned long arg2, unsigned long arg3,
             unsigned long arg4, unsigned long arg5)
{
    TRACE_HOOK("option %d arg2 %lu arg3 %lu arg4 %lu arg5 %lu",
               option, arg2, arg3, arg4, arg5);

#ifdef MALI_QUIRKS
    if (option == PR_SET_NAME) {
        char *name = (char*) arg2;

        if (strcmp(name, MALI_HIST_DUMP_THREAD_NAME) == 0) {

            // This can only work because prctl with PR_SET_NAME
            // can be only called for the current thread and not
            // for another thread so we can safely pause things.

            HYBRIS_DEBUG_LOG(HOOKS, "%s: Found mali-hist-dump, killing thread ...",
                             __FUNCTION__);

            pthread_exit(NULL);
        }
    }
#endif

    return prctl(option, arg2, arg3, arg4, arg5);
}

static char* _hybris_hook_basename(const char *path)
{
    char buf[PATH_MAX];

    TRACE_HOOK("path '%s'", path);

    memset(buf, 0, sizeof(buf));

    if (path)
        strncpy(buf, path, sizeof(buf));

    buf[sizeof buf - 1] = '\0';

    return basename(buf);
}

static char* _hybris_hook_dirname(char *path)
{
    char buf[PATH_MAX];

    TRACE_HOOK("path '%s'", path);

    memset(buf, 0, sizeof(buf));

    if (path)
        strncpy(buf, path, sizeof(buf));

    buf[sizeof buf - 1] = '\0';

    return dirname(path);
}

static char* _hybris_hook_strerror(int errnum)
{
    TRACE_HOOK("errnum %d", errnum);

    return strerror(errnum);
}

static char* _hybris_hook__gnu_strerror_r(int errnum, char *buf, size_t buf_len)
{
    TRACE_HOOK("errnum %d buf '%s' buf len %zu", errnum, buf, buf_len);

    return strerror_r(errnum, buf, buf_len);
}

static int _hybris_hook_mprotect(void *addr, size_t len, int prot)
{
    TRACE_HOOK("addr %p len %zu prot %d", addr, len, prot);

    return mprotect(addr, len, prot);
}

static int _hybris_hook_posix_memalign(void **memptr, size_t alignment, size_t size)
{
    TRACE_HOOK("memptr %p alignment %zu size %zu", memptr, alignment, size);

    return posix_memalign(memptr, alignment, size);
}

static pid_t _hybris_hook_fork(void)
{
    TRACE_HOOK("");

    return fork();
}

static locale_t _hybris_hook_newlocale(int category_mask, const char *locale, locale_t base)
{
    TRACE_HOOK("category mask %i locale '%s'", category_mask, locale);

    return newlocale(category_mask, locale, base);
}

static void _hybris_hook_freelocale(locale_t locobj)
{
    TRACE_HOOK("");

    return freelocale(locobj);
}

static locale_t _hybris_hook_duplocale(locale_t locobj)
{
    TRACE_HOOK("");

    return duplocale(locobj);
}

static locale_t _hybris_hook_uselocale(locale_t newloc)
{
    TRACE_HOOK("");

    return uselocale(newloc);
}

static struct lconv* _hybris_hook_localeconv(void)
{
    TRACE_HOOK("");

    return localeconv();
}

static char* _hybris_hook_setlocale(int category, const char *locale)
{
    TRACE_HOOK("category %i locale '%s'", category, locale);

    return setlocale(category, locale);
}

static void* _hybris_hook_mmap(void *addr, size_t len, int prot,
                  int flags, int fd, off_t offset)
{
    TRACE_HOOK("addr %p len %zu prot %i flags %i fd %i offset %ld",
               addr, len, prot, flags, fd, offset);

    return mmap(addr, len, prot, flags, fd, offset);
}

static int _hybris_hook_munmap(void *addr, size_t length)
{
    TRACE_HOOK("addr %p length %zu", addr, length);

    return munmap(addr, length);
}

extern size_t strlcat(char *dst, const char *src, size_t siz);
extern size_t strlcpy(char *dst, const char *src, size_t siz);

static int _hybris_hook_strcmp(const char *s1, const char *s2)
{
    TRACE_HOOK("s1 '%s' s2 '%s'", s1, s2);

    if ( s1 == NULL || s2 == NULL)
        return -1;

    return strcmp(s1, s2);
}

static FILE* _hybris_hook_setmntent(const char *filename, const char *type)
{
    TRACE_HOOK("filename %s type %s", filename, type);

    return setmntent(filename, type);
}

static struct mntent* _hybris_hook_getmntent(FILE *fp)
{
    TRACE_HOOK("fp %p", fp);

    /* glibc doesn't allow NULL fp here, but bionic does. */
    if (fp == NULL)
        return NULL;

    return getmntent(_get_actual_fp(fp));
}

static struct mntent* _hybris_hook_getmntent_r(FILE *fp, struct mntent *e, char *buf, int buf_len)
{
    TRACE_HOOK("fp %p e %p buf '%s' buf len %i",
               fp, e, buf, buf_len);

    /* glibc doesn't allow NULL fp here, but bionic does. */
    if (fp == NULL)
        return NULL;

    return getmntent_r(_get_actual_fp(fp), e, buf, buf_len);
}

int _hybris_hook_endmntent(FILE *fp)
{
    TRACE_HOOK("fp %p", fp);

    return endmntent(_get_actual_fp(fp));
}

static int _hybris_hook_fputws(const wchar_t *ws, FILE *stream)
{
    TRACE_HOOK("stream %p", stream);

    return fputws(ws, _get_actual_fp(stream));
}

static int _hybris_hook_vfwprintf(FILE *stream, const wchar_t *format, va_list args)
{
    TRACE_HOOK("stream %p", stream);

    return vfwprintf(_get_actual_fp(stream), format, args);
}

static wint_t _hybris_hook_fputwc(wchar_t wc, FILE *stream)
{
    TRACE_HOOK("stream %p", stream);

    return fputwc(wc, _get_actual_fp(stream));
}

static wint_t _hybris_hook_putwc(wchar_t wc, FILE *stream)
{
    TRACE_HOOK("stream %p", stream);

    return putwc(wc, _get_actual_fp(stream));
}

static wint_t _hybris_hook_fgetwc(FILE *stream)
{
    TRACE_HOOK("stream %p", stream);

    return fgetwc(_get_actual_fp(stream));
}

static wint_t _hybris_hook_getwc(FILE *stream)
{
    TRACE_HOOK("stream %p", stream);

    return getwc(_get_actual_fp(stream));
}

static size_t _hybris_hook___fbufsize(FILE *stream)
{
    TRACE_HOOK("__fbufsize");
    return __fbufsize(_get_actual_fp(stream));
}

static size_t _hybris_hook___fpending(FILE *stream)
{
    TRACE_HOOK("__fpending");
    return __fpending(_get_actual_fp(stream));
}

static int _hybris_hook___flbf(FILE *stream)
{
    TRACE_HOOK("__flbf");
    return __flbf(_get_actual_fp(stream));
}

static int _hybris_hook___freadable(FILE *stream)
{
    TRACE_HOOK("__freadable");
    return __freadable(_get_actual_fp(stream));
}

static int _hybris_hook___fwritable(FILE *stream)
{
    TRACE_HOOK("__fwritable");
    return __fwritable(_get_actual_fp(stream));
}

static int _hybris_hook___freading(FILE *stream)
{
    TRACE_HOOK("__freading");
    return __freading(_get_actual_fp(stream));
}

static int _hybris_hook___fwriting(FILE *stream)
{
    TRACE_HOOK("__fwriting");
    return __fwriting(_get_actual_fp(stream));
}

static int _hybris_hook___fsetlocking(FILE *stream, int type)
{
    TRACE_HOOK("__fsetlocking");
    return __fsetlocking(_get_actual_fp(stream), type);
}

static void _hybris_hook__flushlbf(void)
{
    TRACE_HOOK("_flushlbf");
    _flushlbf();
}

static void _hybris_hook___fpurge(FILE *stream)
{
    TRACE_HOOK("__fpurge");
    __fpurge(_get_actual_fp(stream));
}

static void *_hybris_hook_dlopen(const char *filename, int flag)
{
    TRACE("filename %s flag %i", filename, flag);

    return _android_dlopen(filename,flag);
}

static void *_hybris_hook_dlsym(void *handle, const char *symbol)
{
    TRACE("handle %p symbol %s", handle, symbol);

    return _android_dlsym(handle,symbol);
}

static void *_hybris_hook_dlvsym(void *handle, const char *symbol, const char* version)
{
    TRACE("handle %p symbol %s version %s", handle, symbol, version);

    return _android_dlvsym(handle,symbol,version);
}

static void* _hybris_hook_dladdr(void *addr, Dl_info *info)
{
    TRACE("addr %p info %p", addr, info);

    return (void *)_android_dladdr(addr, info);
}

static int _hybris_hook_dlclose(void *handle)
{
    TRACE("handle %p", handle);

    return _android_dlclose(handle);
}

static const char *_hybris_hook_dlerror(void)
{
    TRACE("");

    return android_dlerror();
}

void *_hybris_hook_dl_unwind_find_exidx(void* pc, int* pcount)
{
    TRACE("pc %p, pcount %p", pc, pcount);

    return _android_dl_unwind_find_exidx(pc, pcount);
}

int _hybris_hook_dl_iterate_phdr(int (*cb)(void* info, size_t size, void* data), void* data)
{
    TRACE("cb %p, data %p", cb, data);

    return _android_dl_iterate_phdr(cb, data);
}

void _hybris_hook_android_get_LD_LIBRARY_PATH(char* buffer, size_t buffer_size)
{
    TRACE("buffer %p, buffer_size %zu\n", buffer, buffer_size);

    _android_get_LD_LIBRARY_PATH(buffer, buffer_size);
}

void _hybris_hook_android_update_LD_LIBRARY_PATH(const char* ld_library_path)
{
    TRACE("ld_library_path %s", ld_library_path);

    _android_update_LD_LIBRARY_PATH(ld_library_path);
}

void* _hybris_hook_android_dlopen_ext(const char* filename, int flag, const void* extinfo)
{
    TRACE("filename %s, flag %d, extinfo %p", filename, flag, extinfo);

    return _android_dlopen_ext(filename, flag, extinfo);
}

void _hybris_hook_android_set_application_target_sdk_version(uint32_t target)
{
    TRACE("target %d", target);

    _android_set_application_target_sdk_version(target);
}

uint32_t _hybris_hook_android_get_application_target_sdk_version()
{
    TRACE("");

    return _android_get_application_target_sdk_version();
}

void* _hybris_hook_android_create_namespace(const char* name,
                                                     const char* ld_library_path,
                                                     const char* default_library_path,
                                                     uint64_t type,
                                                     const char* permitted_when_isolated_path,
                                                     void* parent)
{
    TRACE("name %s, ld_library_path %s, default_library_path %s, type %" PRIu64 ", permitted_when_isolated_path %s, parent %p", name, ld_library_path, default_library_path, type, permitted_when_isolated_path, parent);

    return _android_create_namespace(name, ld_library_path, default_library_path, type, permitted_when_isolated_path, parent);
}

bool _hybris_hook_android_init_anonymous_namespace(const char* shared_libs_sonames,
                                      const char* library_search_path)
{
    TRACE("shared_libs_sonames %s, library_search_path %s", shared_libs_sonames, library_search_path);

    return _android_init_anonymous_namespace(shared_libs_sonames, library_search_path);
}

void _hybris_hook_android_dlwarning(void* obj, void (*f)(void*, const char*))
{
    TRACE("obj %p, f %p", obj, f);

    _android_dlwarning(obj, f);
}

void* _hybris_hook_android_get_exported_namespace(const char* name)
{
    TRACE("name %s", name);

    return _android_get_exported_namespace(name);
}

/* this was added while debugging in the hopes to get a backtrace from a double
 * free crash. Unfortunately it fixes the problem so we cannot get a proper
 * backtrace to fix the underlying problem. */
void _hybris_hook_free(void *ptr)
{
    if (ptr) ((char*)ptr)[0] = 0;
    free(ptr);
}

#if !defined(cfree)
#define cfree free
#endif

// old property hooks for pre-android 8 approach
static struct _hook hooks_properties[] = {
    HOOK_INDIRECT(property_get),
    HOOK_INDIRECT(property_set),
    HOOK_INDIRECT(property_list),
    HOOK_INDIRECT(__system_property_get),
    HOOK_INDIRECT(__system_property_read),
    HOOK_TO(__system_property_set, _hybris_hook_property_set),
    HOOK_INDIRECT(__system_property_foreach),
    HOOK_INDIRECT(__system_property_find),
    HOOK_INDIRECT(__system_property_serial),
    HOOK_INDIRECT(__system_property_wait),
    HOOK_INDIRECT(__system_property_update),
    HOOK_INDIRECT(__system_property_add),
    HOOK_INDIRECT(__system_property_wait_any),
    HOOK_INDIRECT(__system_property_find_nth),
};

static struct _hook hooks_common[] = {

    HOOK_DIRECT(getenv),
    HOOK_DIRECT_NO_DEBUG(printf),
    HOOK_INDIRECT(malloc),
    HOOK_INDIRECT(free),
    HOOK_DIRECT_NO_DEBUG(calloc),
    HOOK_DIRECT_NO_DEBUG(free),
    HOOK_DIRECT_NO_DEBUG(realloc),
    HOOK_DIRECT_NO_DEBUG(memalign),
    HOOK_DIRECT_NO_DEBUG(valloc),
    HOOK_DIRECT_NO_DEBUG(pvalloc),
    HOOK_DIRECT(fread),
    HOOK_DIRECT_NO_DEBUG(getxattr),
    HOOK_DIRECT(mprotect),
    /* string.h */
    HOOK_DIRECT_NO_DEBUG(memccpy),
    HOOK_DIRECT_NO_DEBUG(memchr),
    HOOK_DIRECT_NO_DEBUG(memrchr),
    HOOK_DIRECT(memcmp),
    HOOK_INDIRECT(memcpy),
    HOOK_DIRECT_NO_DEBUG(memmove),
    HOOK_DIRECT_NO_DEBUG(memset),
    HOOK_DIRECT_NO_DEBUG(memmem),
    HOOK_DIRECT_NO_DEBUG(getlogin),
    // HOOK_DIRECT(memswap),
    HOOK_DIRECT_NO_DEBUG(index),
    HOOK_DIRECT_NO_DEBUG(rindex),
    HOOK_DIRECT_NO_DEBUG(strchr),
    HOOK_DIRECT_NO_DEBUG(strrchr),
    HOOK_INDIRECT(strlen),
    HOOK_INDIRECT(strcmp),
    HOOK_DIRECT_NO_DEBUG(strcpy),
    HOOK_DIRECT_NO_DEBUG(strcat),
    HOOK_DIRECT_NO_DEBUG(strcasecmp),
    HOOK_DIRECT_NO_DEBUG(strncasecmp),
    HOOK_DIRECT_NO_DEBUG(strdup),
    HOOK_DIRECT_NO_DEBUG(strstr),
    HOOK_DIRECT_NO_DEBUG(strtok),
    HOOK_DIRECT_NO_DEBUG(strtok_r),
    HOOK_DIRECT(strerror),
    HOOK_DIRECT_NO_DEBUG(strerror_r),
    HOOK_DIRECT_NO_DEBUG(strnlen),
    HOOK_DIRECT_NO_DEBUG(strncat),
    HOOK_DIRECT_NO_DEBUG(strndup),
    HOOK_DIRECT_NO_DEBUG(strncmp),
    HOOK_DIRECT_NO_DEBUG(strncpy),
    HOOK_INDIRECT(strtod),
    HOOK_DIRECT_NO_DEBUG(strcspn),
    HOOK_DIRECT_NO_DEBUG(strpbrk),
    HOOK_DIRECT_NO_DEBUG(strsep),
    HOOK_DIRECT_NO_DEBUG(strspn),
    HOOK_DIRECT_NO_DEBUG(strsignal),
    HOOK_DIRECT_NO_DEBUG(getgrnam),
    HOOK_DIRECT_NO_DEBUG(strcoll),
    HOOK_DIRECT_NO_DEBUG(strxfrm),
    /* strings.h */
    HOOK_DIRECT_NO_DEBUG(bcmp),
    HOOK_DIRECT_NO_DEBUG(bcopy),
    HOOK_DIRECT_NO_DEBUG(bzero),
    HOOK_DIRECT_NO_DEBUG(ffs),
    HOOK_INDIRECT(__sprintf_chk),
    HOOK_INDIRECT(__snprintf_chk),
    /* pthread.h */
    HOOK_DIRECT_NO_DEBUG(getauxval),
    HOOK_INDIRECT(gettid),
    HOOK_DIRECT_NO_DEBUG(getpid),
    HOOK_DIRECT_NO_DEBUG(pthread_atfork),
    HOOK_INDIRECT(pthread_create),
    HOOK_INDIRECT(pthread_kill),
    HOOK_DIRECT_NO_DEBUG(pthread_exit),
    HOOK_DIRECT_NO_DEBUG(pthread_join),
    HOOK_DIRECT_NO_DEBUG(pthread_detach),
    HOOK_DIRECT_NO_DEBUG(pthread_self),
    HOOK_DIRECT_NO_DEBUG(pthread_equal),
    HOOK_DIRECT_NO_DEBUG(pthread_getschedparam),
    HOOK_DIRECT_NO_DEBUG(pthread_setschedparam),
    HOOK_INDIRECT(pthread_mutex_init),
    HOOK_INDIRECT(pthread_mutex_destroy),
    HOOK_INDIRECT(pthread_mutex_lock),
    HOOK_INDIRECT(pthread_mutex_unlock),
    HOOK_INDIRECT(pthread_mutex_trylock),
    HOOK_INDIRECT(pthread_mutex_lock_timeout_np),
    HOOK_INDIRECT(pthread_mutex_timedlock),
    HOOK_DIRECT_NO_DEBUG(pthread_mutexattr_init),
    HOOK_DIRECT_NO_DEBUG(pthread_mutexattr_destroy),
    HOOK_DIRECT_NO_DEBUG(pthread_mutexattr_gettype),
    HOOK_DIRECT_NO_DEBUG(pthread_mutexattr_settype),
    HOOK_DIRECT_NO_DEBUG(pthread_mutexattr_getpshared),
    HOOK_DIRECT(pthread_mutexattr_setpshared),
    HOOK_DIRECT_NO_DEBUG(pthread_condattr_init),
    HOOK_DIRECT_NO_DEBUG(pthread_condattr_getpshared),
    HOOK_DIRECT_NO_DEBUG(pthread_condattr_setpshared),
    HOOK_DIRECT_NO_DEBUG(pthread_condattr_destroy),
    HOOK_DIRECT_NO_DEBUG(pthread_condattr_getclock),
    HOOK_DIRECT_NO_DEBUG(pthread_condattr_setclock),
    HOOK_INDIRECT(pthread_cond_init),
    HOOK_INDIRECT(pthread_cond_destroy),
    HOOK_INDIRECT(pthread_cond_broadcast),
    HOOK_INDIRECT(pthread_cond_signal),
    HOOK_INDIRECT(pthread_cond_wait),
    HOOK_INDIRECT(pthread_cond_timedwait),
    HOOK_TO(pthread_cond_timedwait_monotonic, _hybris_hook_pthread_cond_timedwait),
    HOOK_TO(pthread_cond_timedwait_monotonic_np, _hybris_hook_pthread_cond_timedwait),
    HOOK_INDIRECT(pthread_cond_timedwait_relative_np),
    HOOK_DIRECT_NO_DEBUG(pthread_key_delete),
    HOOK_INDIRECT(pthread_setname_np),
    HOOK_DIRECT_NO_DEBUG(pthread_once),
    HOOK_DIRECT_NO_DEBUG(pthread_key_create),
    HOOK_DIRECT(pthread_setspecific),
    HOOK_INDIRECT(pthread_getspecific),
    HOOK_INDIRECT(pthread_attr_init),
    HOOK_INDIRECT(pthread_attr_destroy),
    HOOK_INDIRECT(pthread_attr_setdetachstate),
    HOOK_INDIRECT(pthread_attr_getdetachstate),
    HOOK_INDIRECT(pthread_attr_setschedpolicy),
    HOOK_INDIRECT(pthread_attr_getschedpolicy),
    HOOK_INDIRECT(pthread_attr_setschedparam),
    HOOK_INDIRECT(pthread_attr_getschedparam),
    HOOK_INDIRECT(pthread_attr_setstacksize),
    HOOK_INDIRECT(pthread_attr_getstacksize),
    HOOK_INDIRECT(pthread_attr_setstackaddr),
    HOOK_INDIRECT(pthread_attr_getstackaddr),
    HOOK_INDIRECT(pthread_attr_setstack),
    HOOK_INDIRECT(pthread_attr_getstack),
    HOOK_INDIRECT(pthread_attr_setguardsize),
    HOOK_INDIRECT(pthread_attr_getguardsize),
    HOOK_INDIRECT(pthread_attr_setscope),
    HOOK_INDIRECT(pthread_attr_getscope),
    HOOK_INDIRECT(pthread_getattr_np),
    HOOK_INDIRECT(pthread_rwlockattr_init),
    HOOK_INDIRECT(pthread_rwlockattr_destroy),
    HOOK_INDIRECT(pthread_rwlockattr_setpshared),
    HOOK_INDIRECT(pthread_rwlockattr_getpshared),
    HOOK_INDIRECT(pthread_rwlock_init),
    HOOK_INDIRECT(pthread_rwlock_destroy),
    HOOK_INDIRECT(pthread_rwlock_unlock),
    HOOK_INDIRECT(pthread_rwlock_wrlock),
    HOOK_INDIRECT(pthread_rwlock_rdlock),
    HOOK_INDIRECT(pthread_rwlock_tryrdlock),
    HOOK_INDIRECT(pthread_rwlock_trywrlock),
    HOOK_INDIRECT(pthread_rwlock_timedrdlock),
    HOOK_INDIRECT(pthread_rwlock_timedwrlock),
    HOOK_INDIRECT(__pthread_cleanup_push),
    HOOK_INDIRECT(__pthread_cleanup_pop),
    /* bionic-only pthread */
    HOOK_TO(__pthread_gettid, _hybris_hook_pthread_gettid_np),
    HOOK_INDIRECT(pthread_gettid_np),
    /* stdio.h */
    HOOK_TO(__isthreaded, &_hybris_hook___isthreaded),
    HOOK_TO(__sF, _hybris_hook_sF),
    HOOK_DIRECT_NO_DEBUG(fopen),
    HOOK_DIRECT_NO_DEBUG(fdopen),
    HOOK_DIRECT_NO_DEBUG(popen),
    HOOK_DIRECT_NO_DEBUG(puts),
    HOOK_DIRECT_NO_DEBUG(sprintf),
    HOOK_DIRECT_NO_DEBUG(asprintf),
    HOOK_DIRECT_NO_DEBUG(vasprintf),
    HOOK_DIRECT_NO_DEBUG(snprintf),
    HOOK_DIRECT_NO_DEBUG(vsprintf),
    HOOK_DIRECT_NO_DEBUG(vsnprintf),
    HOOK_INDIRECT(clearerr),
    HOOK_INDIRECT(fclose),
    HOOK_INDIRECT(feof),
    HOOK_INDIRECT(ferror),
    HOOK_INDIRECT(fflush),
    HOOK_INDIRECT(fgetc),
    HOOK_INDIRECT(fgetpos),
    HOOK_INDIRECT(fgets),
    HOOK_INDIRECT(fprintf),
    HOOK_INDIRECT(fputc),
    HOOK_INDIRECT(fputs),
    HOOK_INDIRECT(fread),
    HOOK_INDIRECT(freopen),
    HOOK_INDIRECT(fscanf),
    HOOK_INDIRECT(fseek),
    HOOK_INDIRECT(fseeko),
    HOOK_INDIRECT(fsetpos),
    HOOK_INDIRECT(ftell),
    HOOK_INDIRECT(ftello),
    HOOK_INDIRECT(fwrite),
    HOOK_INDIRECT(getc),
    HOOK_INDIRECT(getdelim),
    HOOK_INDIRECT(getline),
    HOOK_INDIRECT(putc),
    HOOK_INDIRECT(rewind),
    HOOK_INDIRECT(setbuf),
    HOOK_INDIRECT(setvbuf),
    HOOK_INDIRECT(ungetc),
    HOOK_INDIRECT(vfprintf),
    HOOK_INDIRECT(vfscanf),
    HOOK_INDIRECT(fileno),
    HOOK_INDIRECT(pclose),
    HOOK_INDIRECT(flockfile),
    HOOK_INDIRECT(ftrylockfile),
    HOOK_INDIRECT(funlockfile),
    HOOK_INDIRECT(getc_unlocked),
    HOOK_INDIRECT(putc_unlocked),
    //HOOK(fgetln),
    HOOK_INDIRECT(fpurge),
    HOOK_INDIRECT(getw),
    HOOK_INDIRECT(putw),
    HOOK_INDIRECT(setbuffer),
    HOOK_INDIRECT(setlinebuf),
    HOOK_TO(__errno, __errno_location),
    HOOK_INDIRECT(__set_errno),
    HOOK_TO(__set_errno_internal, _hybris_hook___set_errno),
    HOOK_TO(__progname, &program_invocation_name),
    /* net specifics, to avoid __res_get_state */
    HOOK_INDIRECT(getaddrinfo),
    HOOK_INDIRECT(freeaddrinfo),
    HOOK_DIRECT_NO_DEBUG(gethostbyaddr),
    HOOK_DIRECT_NO_DEBUG(gethostbyname),
    HOOK_DIRECT_NO_DEBUG(gethostbyname2),
    HOOK_DIRECT_NO_DEBUG(gethostent),
    HOOK_DIRECT_NO_DEBUG(strftime),
    HOOK_INDIRECT(sysconf),
    HOOK_INDIRECT(dlopen),
    HOOK_INDIRECT(dlerror),
    HOOK_INDIRECT(dlsym),
    HOOK_INDIRECT(dlvsym),
    HOOK_INDIRECT(dladdr),
    HOOK_INDIRECT(dlclose),
    HOOK_INDIRECT(dl_unwind_find_exidx),
    HOOK_INDIRECT(dl_iterate_phdr),
    HOOK_INDIRECT(android_get_LD_LIBRARY_PATH),
    HOOK_INDIRECT(android_update_LD_LIBRARY_PATH),
    HOOK_INDIRECT(android_dlopen_ext),
    HOOK_INDIRECT(android_set_application_target_sdk_version),
    HOOK_INDIRECT(android_get_application_target_sdk_version),
    HOOK_INDIRECT(android_create_namespace),
    HOOK_INDIRECT(android_init_anonymous_namespace),
    HOOK_INDIRECT(android_dlwarning),
    HOOK_INDIRECT(android_get_exported_namespace),
    /* dirent.h */
    HOOK_DIRECT_NO_DEBUG(opendir),
    HOOK_DIRECT_NO_DEBUG(fdopendir),
    HOOK_DIRECT_NO_DEBUG(closedir),
    HOOK_INDIRECT(readdir),
    HOOK_INDIRECT(readdir_r),
    HOOK_DIRECT_NO_DEBUG(rewinddir),
    HOOK_DIRECT_NO_DEBUG(seekdir),
    HOOK_DIRECT_NO_DEBUG(telldir),
    HOOK_DIRECT_NO_DEBUG(dirfd),
    HOOK_INDIRECT(scandir),
    HOOK_INDIRECT(scandirat),
    HOOK_INDIRECT(alphasort),
    HOOK_INDIRECT(versionsort),
    /* fcntl.h */
    HOOK_INDIRECT(open),
    // TODO: scandir, scandirat, alphasort, versionsort
    HOOK_INDIRECT(__get_tls_hooks),
    HOOK_DIRECT_NO_DEBUG(sscanf),
    HOOK_DIRECT_NO_DEBUG(scanf),
    HOOK_DIRECT_NO_DEBUG(vscanf),
    HOOK_DIRECT_NO_DEBUG(vsscanf),
    HOOK_DIRECT_NO_DEBUG(openlog),
    HOOK_DIRECT_NO_DEBUG(syslog),
    HOOK_DIRECT_NO_DEBUG(closelog),
    HOOK_DIRECT_NO_DEBUG(vsyslog),
    HOOK_DIRECT_NO_DEBUG(timer_create),
    HOOK_DIRECT_NO_DEBUG(timer_settime),
    HOOK_DIRECT_NO_DEBUG(timer_gettime),
    HOOK_DIRECT_NO_DEBUG(timer_delete),
    HOOK_DIRECT_NO_DEBUG(timer_getoverrun),
    HOOK_DIRECT_NO_DEBUG(localtime),
    HOOK_DIRECT_NO_DEBUG(localtime_r),
    HOOK_DIRECT_NO_DEBUG(gmtime),
    HOOK_DIRECT_NO_DEBUG(abort),
    HOOK_DIRECT_NO_DEBUG(writev),
    /* unistd.h */
    HOOK_DIRECT_NO_DEBUG(access),
    /* grp.h */
    HOOK_DIRECT_NO_DEBUG(getgrgid),
    HOOK_DIRECT_NO_DEBUG(__cxa_atexit),
    HOOK_DIRECT_NO_DEBUG(__cxa_finalize),
    /* sys/prctl.h */
    HOOK_INDIRECT(prctl),
    /* stdio_ext.h */
    HOOK_INDIRECT(__fbufsize),
    HOOK_INDIRECT(__fpending),
    HOOK_INDIRECT(__flbf),
    HOOK_INDIRECT(__freadable),
    HOOK_INDIRECT(__fwritable),
    HOOK_INDIRECT(__freading),
    HOOK_INDIRECT(__fwriting),
    HOOK_INDIRECT(__fsetlocking),
    HOOK_INDIRECT(_flushlbf),
    HOOK_INDIRECT(__fpurge),
};

static struct _hook hooks_mm[] = {
    HOOK_DIRECT(strtol),
    HOOK_DIRECT_NO_DEBUG(strlcat),
    HOOK_DIRECT_NO_DEBUG(strlcpy),
    HOOK_DIRECT(setenv),
    HOOK_DIRECT(putenv),
    HOOK_DIRECT(clearenv),
    HOOK_DIRECT_NO_DEBUG(dprintf),
    HOOK_DIRECT_NO_DEBUG(mallinfo),
    HOOK_DIRECT(malloc_usable_size),
    HOOK_DIRECT(posix_memalign),
    HOOK_DIRECT(mprotect),
    HOOK_TO(__gnu_strerror_r, _hybris_hook__gnu_strerror_r),
    HOOK_INDIRECT(pthread_rwlockattr_getkind_np),
    HOOK_INDIRECT(pthread_rwlockattr_setkind_np),
    /* unistd.h */
    HOOK_DIRECT(fork),
    HOOK_DIRECT_NO_DEBUG(ttyname),
    HOOK_DIRECT_NO_DEBUG(swprintf),
    HOOK_DIRECT_NO_DEBUG(fmemopen),
    HOOK_DIRECT_NO_DEBUG(open_memstream),
    HOOK_DIRECT_NO_DEBUG(open_wmemstream),
    HOOK_DIRECT_NO_DEBUG(ptsname),
    HOOK_TO(__hybris_set_errno_internal, _hybris_hook___set_errno),
    HOOK_DIRECT_NO_DEBUG(getservbyname),
    /* libgen.h */
    HOOK_INDIRECT(basename),
    HOOK_INDIRECT(dirname),
    /* locale.h */
    HOOK_DIRECT(newlocale),
    HOOK_DIRECT(freelocale),
    HOOK_DIRECT(duplocale),
    HOOK_DIRECT(uselocale),
    HOOK_DIRECT(localeconv),
    HOOK_DIRECT(setlocale),
    /* sys/mman.h */
    HOOK_DIRECT(mmap),
    HOOK_DIRECT(munmap),
    /* wchar.h */
    HOOK_DIRECT_NO_DEBUG(wmemchr),
    HOOK_DIRECT_NO_DEBUG(wmemcmp),
    HOOK_DIRECT_NO_DEBUG(wmemcpy),
    HOOK_DIRECT_NO_DEBUG(wmemmove),
    HOOK_DIRECT_NO_DEBUG(wmemset),
    HOOK_DIRECT_NO_DEBUG(wmempcpy),
    HOOK_INDIRECT(fputws),
    // It's enough to hook vfwprintf here as fwprintf will call it with a
    // proper va_list in place so we don't have to handle this here.
    HOOK_INDIRECT(vfwprintf),
    HOOK_INDIRECT(fputwc),
    HOOK_INDIRECT(putwc),
    HOOK_INDIRECT(fgetwc),
    HOOK_INDIRECT(getwc),
    /* sched.h */
    HOOK_DIRECT_NO_DEBUG(clone),
    /* mntent.h */
    HOOK_DIRECT(setmntent),
    HOOK_INDIRECT(getmntent),
    HOOK_INDIRECT(getmntent_r),
    HOOK_INDIRECT(endmntent),
    /* stdlib.h */
    HOOK_DIRECT_NO_DEBUG(system),
    /* pwd.h */
    HOOK_DIRECT_NO_DEBUG(getpwuid),
    HOOK_DIRECT_NO_DEBUG(getpwnam),
    /* signal.h */
    /* Hooks commented out for the moment as we need proper translations between
     * bionic and glibc types for them to work (for instance, sigset_t has
     * different definitions in each library).
     */
#if 0
    HOOK_INDIRECT(sigaction),
    HOOK_INDIRECT(sigaddset),
    HOOK_INDIRECT(sigaltstack),
    HOOK_INDIRECT(sigblock),
    HOOK_INDIRECT(sigdelset),
    HOOK_INDIRECT(sigemptyset),
    HOOK_INDIRECT(sigfillset),
    HOOK_INDIRECT(siginterrupt),
    HOOK_INDIRECT(sigismember),
    HOOK_INDIRECT(siglongjmp),
    HOOK_INDIRECT(signal),
    HOOK_INDIRECT(signalfd),
    HOOK_INDIRECT(sigpending),
    HOOK_INDIRECT(sigprocmask),
    HOOK_INDIRECT(sigqueue),
    // setjmp.h defines segsetjmp via a #define and the real symbol
    // we have to forward to is __sigsetjmp
    {"sigsetjmp", __sigsetjmp},
    HOOK_INDIRECT(sigsetmask),
    HOOK_INDIRECT(sigsuspend),
    HOOK_INDIRECT(sigtimedwait),
    HOOK_INDIRECT(sigwait),
    HOOK_INDIRECT(sigwaitinfo),
#endif
    /* dirent.h */
    HOOK_TO(readdir64, _hybris_hook_readdir),
    HOOK_TO(readdir64_r, _hybris_hook_readdir_r),
    HOOK_INDIRECT(scandir),
    HOOK_INDIRECT(scandirat),
    HOOK_TO(scandir64, _hybris_hook_scandir),
};


static int hook_cmp(const void *a, const void *b)
{
    return strcmp(((struct _hook*)a)->name, ((struct _hook*)b)->name);
}

void hybris_set_hook_callback(hybris_hook_cb callback)
{
    hook_callback = callback;
}

#define LINKER_NAME_JB "jb"
#define LINKER_NAME_MM "mm"
#define LINKER_NAME_N "n"
#define LINKER_NAME_O "o"

#if defined(WANT_LINKER_O)
#define LINKER_VERSION_DEFAULT 27
#define LINKER_NAME_DEFAULT LINKER_NAME_O
#elif defined(WANT_LINKER_N)
#define LINKER_VERSION_DEFAULT 25
#define LINKER_NAME_DEFAULT LINKER_NAME_N
#elif defined(WANT_LINKER_MM)
#define LINKER_VERSION_DEFAULT 23
#define LINKER_NAME_DEFAULT LINKER_NAME_MM
#elif defined(WANT_LINKER_JB)
#define LINKER_VERSION_DEFAULT 18
#define LINKER_NAME_DEFAULT LINKER_NAME_JB
#endif

// create string version of default linker for get_android_sdk_version
#define QUOTE(x) #x
#define STRINGIFY(x) QUOTE(x)
#define LINKER_VERSION_DEFAULT_STRING STRINGIFY(LINKER_VERSION_DEFAULT)

static int get_android_sdk_version()
{
    static int sdk_version = -1;

    if (sdk_version > 0)
        return sdk_version;

    // in case android-init is patched we can use my_property_get. in case it
    // is not use the default linker. this is such that we don't need the
    // properties patch in android >=8, because properties are read via bionic
    // libc.so starting from android 8, since it is much easier to use the
    // bionic implementation and avoid having to implement all the fancy bionic
    // property features which are mandatory now and cannot be stubbed as
    // previously.
    char value[PROP_VALUE_MAX];
    my_property_get("ro.build.version.sdk", value, LINKER_VERSION_DEFAULT_STRING);

    sdk_version = LINKER_VERSION_DEFAULT;
    if (strlen(value) > 0) {
        sdk_version = atoi(value);
    }

#ifdef UBUNTU_LINKER_OVERRIDES
    /* We override both frieza and turbo here until they are ready to be
     * upgraded to the newer linker. */
    char device_name[PROP_VALUE_MAX];
    memset(device_name, 0, sizeof(device_name));
    my_property_get("ro.build.product", device_name, "");
    if (strlen(device_name) > 0) {
        /* Force SDK version for both frieza/cooler and turbo for the time being */
        if (strcmp(device_name, "frieza") == 0 ||
            strcmp(device_name, "cooler") == 0 ||
            strcmp(device_name, "turbo") == 0)
            sdk_version = 19;
    }
#endif

    char *version_override = getenv("HYBRIS_ANDROID_SDK_VERSION");
    if (version_override)
        sdk_version = atoi(version_override);

    LOGD("Using SDK API version %i\n", sdk_version);

    return sdk_version;
}

#define HOOKS_SIZE(hooks) \
    (sizeof(hooks) / sizeof(hooks[0]))


static void* __hybris_get_hooked_symbol(const char *sym, const char *requester)
{
    static int sorted = 0;
    static intptr_t counter = -1;
    void *found = NULL;
    struct _hook key;
    int sdk_version = -1;

    /* First check if we have a callback registered which could
     * give us a context specific hook implementation */
    if (hook_callback)
    {
        found = hook_callback(sym, requester);
        if (found)
            return (void*) found;
    }

    if (!sorted)
    {
        qsort(hooks_properties, HOOKS_SIZE(hooks_properties), sizeof(hooks_properties[0]), hook_cmp);
        qsort(hooks_common, HOOKS_SIZE(hooks_common), sizeof(hooks_common[0]), hook_cmp);
        qsort(hooks_mm, HOOKS_SIZE(hooks_mm), sizeof(hooks_mm[0]), hook_cmp);
        sorted = 1;
    }

    /* Allow newer hooks to override those which are available for all versions */
    key.name = sym;
    sdk_version = get_android_sdk_version();

#if defined(WANT_LINKER_MM) || defined(WANT_LINKER_N) || defined(WANT_LINKER_O)
    if (sdk_version > 21)
        found = bsearch(&key, hooks_mm, HOOKS_SIZE(hooks_mm), sizeof(hooks_mm[0]), hook_cmp);
#endif
    // make sure to skip the property hooks only when o.so is actually loaded
    // since for testing and we sometimes set things like 99 as sdk version.
    // The o linker is loaded when sdk_version >= 27 and exists.
    if (!found && sdk_version < 27)
        found = bsearch(&key, hooks_properties, HOOKS_SIZE(hooks_properties), sizeof(hooks_properties[0]), hook_cmp);

    if (!found)
        found = bsearch(&key, hooks_common, HOOKS_SIZE(hooks_common), sizeof(hooks_common[0]), hook_cmp);

    if (found)
    {
        if(hybris_should_trace(NULL, NULL))
            return ((struct _hook*) found)->debug_func;
        else
            return ((struct _hook*) found)->func;
    }

    if (strncmp(sym, "pthread", 7) == 0 ||
        strncmp(sym, "__pthread", 9) == 0)
    {
        /* safe */
        if (strcmp(sym, "pthread_sigmask") == 0)
           return NULL;
        /* not safe */
        counter--;
        // If you're experiencing a crash later on check the address of the
        // function pointer being call. If it matches the printed counter
        // value here then you can easily find out which symbol is missing.
        LOGD("Missing hook for pthread symbol %s (counter %" PRIiPTR ")\n", sym, counter);
        return (void *) counter;
    }

    if (!getenv("HYBRIS_DONT_PRINT_SYMBOLS_WITHOUT_HOOK"))
        LOGD("Could not find a hook for symbol %s", sym);

    return NULL;
}

static void *linker_handle = NULL;

static void* __hybris_load_linker(const char *path)
{
    void *handle = dlopen(path, RTLD_NOW | RTLD_LOCAL);
    if (!handle) {
        fprintf(stderr, "ERROR: Failed to load hybris linker for Android SDK version %d: %s\n",
                get_android_sdk_version(), dlerror());
        return NULL;
    }
    return handle;
}

static int linker_initialized = 0;

static void __hybris_linker_init()
{
    LOGD("Linker initialization");
    
    int enable_linker_gdb_support = 0;
    const char *env = getenv("HYBRIS_ENABLE_LINKER_DEBUG_MAP");
    if (env != NULL) {
        if (strcmp(env, "1") == 0) {
            enable_linker_gdb_support = 1;
        }
    }

    int sdk_version = get_android_sdk_version();

    char path[PATH_MAX];
    const char *name = LINKER_NAME_DEFAULT;

    /* See https://source.android.com/source/build-numbers.html for
     * an overview over available SDK version numbers and which
     * Android version they relate to. */
#if defined(WANT_LINKER_O)
    if (sdk_version <= 27)
        name = LINKER_NAME_O;
#endif
#if defined(WANT_LINKER_N)
    if (sdk_version <= 25)
        name = LINKER_NAME_N;
#endif
#if defined(WANT_LINKER_MM)
    if (sdk_version <= 23)
        name = LINKER_NAME_MM;
#endif
#if defined(WANT_LINKER_JB)
    if (sdk_version < 21)
        name = LINKER_NAME_JB;
#endif

    const char *linker_dir = LINKER_PLUGIN_DIR;
    /* getauxval to make sure users cannot load custom libraries into
     * setuid processes */
    const char *user_linker_dir = getauxval(AT_SECURE) ?
	    NULL :
	    getenv("HYBRIS_LINKER_DIR");
    if (user_linker_dir)
        linker_dir = user_linker_dir;

    snprintf(path, PATH_MAX, "%s/%s.so", linker_dir, name);

    LOGD("Loading linker from %s..", path);

    linker_handle = __hybris_load_linker(path);
    if (!linker_handle)
        exit(1);

    /* Load all necessary symbols we need from the linker */
    _android_linker_init = dlsym(linker_handle, "android_linker_init");
    _android_dlopen = dlsym(linker_handle, "android_dlopen");
    _android_dlerror = dlsym(linker_handle, "android_dlerror");
    _android_dlsym = dlsym(linker_handle, "android_dlsym");
    _android_dlvsym = dlsym(linker_handle, "android_dlvsym");
    _android_dladdr = dlsym(linker_handle, "android_dladdr");
    _android_dlclose = dlsym(linker_handle, "android_dlclose");
    _android_dl_unwind_find_exidx = dlsym(linker_handle, "android_dl_unwind_find_exidx");
    _android_dl_iterate_phdr = dlsym(linker_handle, "android_dl_iterate_phdr");
    _android_get_LD_LIBRARY_PATH = dlsym(linker_handle, "android_get_LD_LIBRARY_PATH");
    _android_update_LD_LIBRARY_PATH = dlsym(linker_handle, "android_update_LD_LIBRARY_PATH");
    _android_dlopen_ext = dlsym(linker_handle, "android_dlopen_ext");
    _android_set_application_target_sdk_version = dlsym(linker_handle, "android_set_application_target_sdk_version");
    _android_get_application_target_sdk_version = dlsym(linker_handle, "android_get_application_target_sdk_version");
    _android_create_namespace = dlsym(linker_handle, "android_create_namespace");
    _android_init_anonymous_namespace = dlsym(linker_handle, "android_init_anonymous_namespace");
    _android_dlwarning = dlsym(linker_handle, "android_dlwarning");
    _android_get_exported_namespace = dlsym(linker_handle, "android_get_exported_namespace");

    /* Now its time to setup the linker itself */
#ifdef WANT_ARM_TRACING
    _android_linker_init(sdk_version, __hybris_get_hooked_symbol, enable_linker_gdb_support, create_wrapper);
#else
    _android_linker_init(sdk_version, __hybris_get_hooked_symbol, enable_linker_gdb_support);
#endif

    if (_android_set_application_target_sdk_version) {
        _android_set_application_target_sdk_version(sdk_version);
    }

    linker_initialized = 1;
}

#define ENSURE_LINKER_IS_LOADED() \
    if (!linker_initialized) \
        __hybris_linker_init();

/* NOTE: As we're not linking directly with the linker anymore
 * but several users are using android_* functions directly we
 * have to export them here. */

void* android_dlopen(const char* filename, int flag)
{
    ENSURE_LINKER_IS_LOADED();

    if (!_android_dlopen) {
        return NULL;
    }

    return _android_dlopen(filename, flag);
}

char* android_dlerror()
{
    ENSURE_LINKER_IS_LOADED();

    if (!_android_dlerror) {
        return NULL;
    }

    return _android_dlerror();
}

void* android_dlsym(void* handle, const char* symbol)
{
    ENSURE_LINKER_IS_LOADED();

    // do not use hybris properties for older linkers
    if (get_android_sdk_version() < 27) {
        if (!strcmp(symbol, "property_list")) {
            return my_property_list;
        }
        if (!strcmp(symbol, "property_get")) {
            return my_property_get;
        }
        if (!strcmp(symbol, "property_set")) {
            return my_property_set;
        }
    }

    if (!_android_dlsym) {
        return NULL;
    }

    return _android_dlsym(handle, symbol);
}

void* android_dlvsym(void* handle, const char* symbol, const char* version)
{
    ENSURE_LINKER_IS_LOADED();

    if (!_android_dlvsym) {
        return NULL;
    }

    return _android_dlvsym(handle, symbol, version);
}

int android_dladdr(const void* addr, void* info)
{
    ENSURE_LINKER_IS_LOADED();

    if (!_android_dladdr) {
        return 0;
    }

    return _android_dladdr(addr, info);
}

int android_dlclose(void* handle)
{
    ENSURE_LINKER_IS_LOADED();

    if (!_android_dlclose) {
        return 0;
    }

    return _android_dlclose(handle);
}

void *android_dl_unwind_find_exidx(void *pc, int* pcount)
{
    ENSURE_LINKER_IS_LOADED();

    if (!_android_dl_unwind_find_exidx) {
        return NULL;
    }

    return _android_dl_unwind_find_exidx(pc, pcount);
}

int android_dl_iterate_phdr(int (*cb)(void* info, size_t size, void* data), void* data)
{
    ENSURE_LINKER_IS_LOADED();

    if (!_android_dl_iterate_phdr) {
        return 0;
    }

    return _android_dl_iterate_phdr(cb, data);
}

void android_get_LD_LIBRARY_PATH(char* buffer, size_t buffer_size)
{
    ENSURE_LINKER_IS_LOADED();

    if (!_android_get_LD_LIBRARY_PATH) {
        return;
    }

    _android_get_LD_LIBRARY_PATH(buffer, buffer_size);
}

void android_update_LD_LIBRARY_PATH(const char* ld_library_path)
{
    ENSURE_LINKER_IS_LOADED();

    if (!_android_update_LD_LIBRARY_PATH) {
        return;
    }

    _android_update_LD_LIBRARY_PATH(ld_library_path);
}

void* android_dlopen_ext(const char* filename, int flag, const void* extinfo)
{
    ENSURE_LINKER_IS_LOADED();

    if (!_android_dlopen_ext) {
        return NULL;
    }

    return _android_dlopen_ext(filename, flag, extinfo);
}

void android_set_application_target_sdk_version(uint32_t target)
{
    ENSURE_LINKER_IS_LOADED();

    if (!_android_set_application_target_sdk_version) {
        return;
    }

    _android_set_application_target_sdk_version(target);
}

uint32_t android_get_application_target_sdk_version()
{
    ENSURE_LINKER_IS_LOADED();

    if (!_android_get_application_target_sdk_version) {
        return 0;
    }

    return _android_get_application_target_sdk_version();
}

struct android_namespace_t* android_create_namespace(const char* name,
                                                     const char* ld_library_path,
                                                     const char* default_library_path,
                                                     uint64_t type,
                                                     const char* permitted_when_isolated_path,
                                                     struct android_namespace_t* parent)
{
    ENSURE_LINKER_IS_LOADED();

    if (!_android_create_namespace) {
        return NULL;
    }

    return _android_create_namespace(name, ld_library_path, default_library_path, type, permitted_when_isolated_path, parent);
}

bool android_init_anonymous_namespace(const char* shared_libs_sonames,
                                      const char* library_search_path)
{
    ENSURE_LINKER_IS_LOADED();

    if (!_android_init_anonymous_namespace) {
        return 0;
    }

    return _android_init_anonymous_namespace(shared_libs_sonames, library_search_path);
}

void android_dlwarning(void* obj, void (*f)(void*, const char*))
{
    ENSURE_LINKER_IS_LOADED();

    if (!_android_dlwarning) {
        return;
    }

    _android_dlwarning(obj, f);
}

struct android_namespace_t* android_get_exported_namespace(const char* name)
{
    ENSURE_LINKER_IS_LOADED();

    if (!_android_get_exported_namespace) {
        return NULL;
    }

    return _android_get_exported_namespace(name);
}

void* hybris_dlopen(const char* filename, int flag)
{
    return android_dlopen(filename, flag);
}

char* hybris_dlerror()
{
    return android_dlerror();
}

void* hybris_dlsym(void* handle, const char* symbol)
{
    return android_dlsym(handle, symbol);
}

void* hybris_dlvsym(void* handle, const char* symbol, const char* version)
{
    return android_dlvsym(handle, symbol, version);
}

int hybris_dladdr(const void* addr, void* info)
{
    return android_dladdr(addr, info);
}

int hybris_dlclose(void* handle)
{
    return android_dlclose(handle);
}

void *hybris_dl_unwind_find_exidx(void *pc, int* pcount)
{
    return android_dl_unwind_find_exidx(pc, pcount);
}

int hybris_dl_iterate_phdr(int (*cb)(void* info, size_t size, void* data), void* data)
{
    return android_dl_iterate_phdr(cb, data);
}

void hybris_get_LD_LIBRARY_PATH(char* buffer, size_t buffer_size)
{
    android_get_LD_LIBRARY_PATH(buffer, buffer_size);
}

void hybris_update_LD_LIBRARY_PATH(const char* ld_library_path)
{
    android_update_LD_LIBRARY_PATH(ld_library_path);
}

void* hybris_dlopen_ext(const char* filename, int flag, const void* extinfo)
{
    return android_dlopen_ext(filename, flag, extinfo);
}

void hybris_set_application_target_sdk_version(uint32_t target)
{
    android_set_application_target_sdk_version(target);
}

uint32_t hybris_get_application_target_sdk_version()
{
    return android_get_application_target_sdk_version();
}

void* hybris_create_namespace(const char* name,
                                                     const char* ld_library_path,
                                                     const char* default_library_path,
                                                     uint64_t type,
                                                     const char* permitted_when_isolated_path,
                                                     void* parent)
{
    return android_create_namespace(name, ld_library_path, default_library_path, type, permitted_when_isolated_path, parent);
}

bool hybris_init_anonymous_namespace(const char* shared_libs_sonames,
                                      const char* library_search_path)
{
    return android_init_anonymous_namespace(shared_libs_sonames, library_search_path);
}

void hybris_dlwarning(void* obj, void (*f)(void*, const char*))
{
    android_dlwarning(obj, f);
}

void* hybris_get_exported_namespace(const char* name)
{
    return android_get_exported_namespace(name);
}

