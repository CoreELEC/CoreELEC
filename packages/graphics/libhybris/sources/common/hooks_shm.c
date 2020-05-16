/*
 * Copyright (c) 2012 Carsten Munk <carsten.munk@gmail.com>
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

#include "hooks_shm.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>

/* Debug */
#include "logging.h"
#define LOGD(message, ...) HYBRIS_DEBUG_LOG(HOOKS, message, ##__VA_ARGS__)

#define HYBRIS_DATA_SIZE    4000
#if defined(__LP64__)
# define HYBRIS_SHM_MASK    0xFFFFFFFFFF000000ULL
#else
# define HYBRIS_SHM_MASK    0xFF000000UL
#endif
#define HYBRIS_SHM_PATH     "/hybris_shm_data"

/* Structure of a shared memory region */
typedef struct _hybris_shm_data_t {
    pthread_mutex_t access_mutex;
    int current_offset;
    int max_offset;
    unsigned char data;
} hybris_shm_data_t;

/* A helper to switch between the size of the data and the size of the shm object */
const int HYBRIS_SHM_DATA_HEADER_SIZE = (sizeof(hybris_shm_data_t) - sizeof(unsigned char));

/* pointer to the shared memory region */
static hybris_shm_data_t *_hybris_shm_data = NULL;

/* the SHM mem_id of the shared memory region */
static int _hybris_shm_fd = -1;

/* the size of the shared memory region that is currently mmap'ed to this process */
static int _current_mapped_size = 0;

/* forward-declare the internal static methods */
static void _release_shm(void);
static void _sync_mmap_with_shm(void);
static void _hybris_shm_init(void);
static void _hybris_shm_extend_region(void);

/*
 * Detach the allocated memory region, and mark it for deletion
 */
static void _release_shm(void)
{
    if (_hybris_shm_data) {
        munmap(_hybris_shm_data, _current_mapped_size); /* unmap from this process */
        _hybris_shm_data = NULL; /* pointer is no more valid */
    }
    if (_hybris_shm_fd >= 0) {
        close(_hybris_shm_fd);   /* close the shm file descriptor */
        _hybris_shm_fd = -1;
    }
    shm_unlink(HYBRIS_SHM_PATH);  /* request the deletion of the shm region */
}

/*
 * Synchronize the size of the mmap with the size of the shm region
 */
static void _sync_mmap_with_shm()
{
    if (_hybris_shm_fd >= 0 && _hybris_shm_data) {
        if (_current_mapped_size < _hybris_shm_data->max_offset + HYBRIS_SHM_DATA_HEADER_SIZE) {
            /* Note that mremap may change the address pointed by _hybris_shm_data.
             * But as we never point directly into _hybris_shm_data, it's fine.
             * */
            _hybris_shm_data = (hybris_shm_data_t *)mremap( _hybris_shm_data, _current_mapped_size,
                                  _hybris_shm_data->max_offset + HYBRIS_SHM_DATA_HEADER_SIZE,
                                  MREMAP_MAYMOVE );

            _current_mapped_size = _hybris_shm_data->max_offset + HYBRIS_SHM_DATA_HEADER_SIZE;
        }
    }
}

/*
 * Initialize the shared memory region for hybris, in order to store
 * pshared mutex, condition and rwlock
 */
static void _hybris_shm_init()
{
    if (_hybris_shm_fd < 0) {
        const size_t size_to_map = HYBRIS_SHM_DATA_HEADER_SIZE + HYBRIS_DATA_SIZE; /* 4000 bytes for the data, plus the header size */

        /* initialize or get shared memory segment */
        _hybris_shm_fd = shm_open(HYBRIS_SHM_PATH, O_RDWR, 0660);
        if (_hybris_shm_fd >= 0) {
            /* Map the memory object */
            _hybris_shm_data = (hybris_shm_data_t *)mmap( NULL, size_to_map,
                                             PROT_READ | PROT_WRITE, MAP_SHARED,
                                             _hybris_shm_fd, 0 );
            _current_mapped_size = size_to_map;

            _sync_mmap_with_shm();
        }
        else {
            LOGD("Creating a new shared memory segment.");

            mode_t pumask = umask(0);
            _hybris_shm_fd = shm_open(HYBRIS_SHM_PATH, O_RDWR | O_CREAT, 0666);
            umask(pumask);
            if (_hybris_shm_fd >= 0) {
                TEMP_FAILURE_RETRY(ftruncate( _hybris_shm_fd, size_to_map ));
                /* Map the memory object */
                _hybris_shm_data = (hybris_shm_data_t *)mmap( NULL, size_to_map,
                                             PROT_READ | PROT_WRITE, MAP_SHARED,
                                             _hybris_shm_fd, 0 );
                if (_hybris_shm_data == MAP_FAILED) {
                    HYBRIS_ERROR_LOG(HOOKS, "ERROR: mmap failed: %s\n", strerror(errno));
                    _release_shm();
                }
                else {
                    _current_mapped_size = size_to_map;
                    /* Initialize the memory object */
                    memset((void*)_hybris_shm_data, 0, size_to_map);
                    _hybris_shm_data->max_offset = HYBRIS_DATA_SIZE;

                    pthread_mutexattr_t attr;
                    pthread_mutexattr_init(&attr);
                    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
                    pthread_mutex_init(&_hybris_shm_data->access_mutex, &attr);
                    pthread_mutexattr_destroy(&attr);

                    atexit(_release_shm);
                }
            }
            else {
                HYBRIS_ERROR_LOG(HOOKS, "ERROR: Couldn't create shared memory segment !");
            }
        }
    }
}

/*
 * Extend the SHM region's size
 */
static void _hybris_shm_extend_region()
{
    TEMP_FAILURE_RETRY(ftruncate( _hybris_shm_fd, _current_mapped_size + HYBRIS_DATA_SIZE ));
    _hybris_shm_data->max_offset += HYBRIS_DATA_SIZE;

    _sync_mmap_with_shm();
}

/************ public functions *******************/

 /*
  * Determine if the pointer that has been extracted by hybris is
  * pointing to an address in the shared memory.
  */
int hybris_is_pointer_in_shm(void *ptr)
{
    if (((uintptr_t) ptr >= HYBRIS_SHM_MASK) &&
                    ((uintptr_t) ptr <= HYBRIS_SHM_MASK_TOP))
        return 1;

    return 0;
}

/*
 * Convert this offset pointer to the shared memory to an
 * absolute pointer that can be used in user space
 */
void *hybris_get_shmpointer(hybris_shm_pointer_t handle)
{
    void *realpointer = NULL;
    if (hybris_is_pointer_in_shm((void*)handle)) {
        if (_hybris_shm_fd < 0) {
            /* if we are not yet attached to any shm region, then do it now */
            _hybris_shm_init();
        }

        pthread_mutex_lock(&_hybris_shm_data->access_mutex);

        _sync_mmap_with_shm();  /* make sure our mmap is sync'ed */

        if (_hybris_shm_data != NULL) {
            uintptr_t offset = handle & (~HYBRIS_SHM_MASK);
            realpointer = &(_hybris_shm_data->data) + offset;

            /* Be careful when activating this trace: this method is called *a lot* !
            LOGD("handle = %x, offset  = %d, realpointer = %x)", handle, offset, realpointer);
             */
        }

        pthread_mutex_unlock(&_hybris_shm_data->access_mutex);
    }

    return realpointer;
}

/*
 * Allocate a space in the shared memory region of hybris
 */
hybris_shm_pointer_t hybris_shm_alloc(size_t size)
{
    hybris_shm_pointer_t location = 0;

    if (_hybris_shm_fd < 0) {
        /* if we are not yet attached to any shm region, then do it now */
        _hybris_shm_init();
    }

    pthread_mutex_lock(&_hybris_shm_data->access_mutex);

    /* Make sure our mmap is sync'ed */
    _sync_mmap_with_shm();

    if (_hybris_shm_data == NULL || _hybris_shm_fd < 0)
        return 0;

    if (_hybris_shm_data->current_offset + size >= _hybris_shm_data->max_offset) {
        /* the current buffer if full: extend it a little bit more */
        _hybris_shm_extend_region();
        _sync_mmap_with_shm();
    }

    /* there is now enough place in this pool */
    location = _hybris_shm_data->current_offset | HYBRIS_SHM_MASK;
    LOGD("Allocated a shared object (size = %zu, at offset %d)", size, _hybris_shm_data->current_offset);

    _hybris_shm_data->current_offset += size;

    pthread_mutex_unlock(&_hybris_shm_data->access_mutex);

    return location;
}
