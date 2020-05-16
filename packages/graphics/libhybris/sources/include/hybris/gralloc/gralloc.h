#ifndef hybris_gralloc_header_include_guard__
#define hybris_gralloc_header_include_guard__

#ifdef __cplusplus
extern "C" {
#endif

// for usage definitions and so on
#if HAS_GRALLOC1_HEADER
#include <hardware/gralloc1.h>
#endif
#include <hardware/gralloc.h>

#include <cutils/native_handle.h>

void hybris_gralloc_deinitialize(void);
void hybris_gralloc_initialize(int framebuffer);
void hybris_gralloc_deinitialize(void);
int hybris_gralloc_release(buffer_handle_t handle, int was_allocated);
int hybris_gralloc_retain(buffer_handle_t handle);
int hybris_gralloc_allocate(int width, int height, int format, int usage, buffer_handle_t *handle, uint32_t *stride);
int hybris_gralloc_lock(buffer_handle_t handle, int usage, int l, int t, int w, int h, void **vaddr);
int hybris_gralloc_unlock(buffer_handle_t handle);
int hybris_gralloc_fbdev_format(void);
int hybris_gralloc_fbdev_framebuffer_count(void);
int hybris_gralloc_fbdev_setSwapInterval(int interval);
int hybris_gralloc_fbdev_post(buffer_handle_t handle);
int hybris_gralloc_fbdev_width(void);
int hybris_gralloc_fbdev_height(void);

#ifdef __cplusplus
};
#endif

#endif

