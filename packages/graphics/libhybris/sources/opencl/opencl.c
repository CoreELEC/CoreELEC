/*
 * Copyright (c) 2018 Matti Lehtimäki <matti.lehtimaki@gmail.com>
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

/* For RTLD_DEFAULT */
#define _GNU_SOURCE
#include <CL/opencl.h>
#include <dlfcn.h>
#include <stdlib.h>

#include <hybris/common/binding.h>

#define CL_USE_DEPRECATED_OPENCL_1_1_APIS 1
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS 1
#define CL_USE_DEPRECATED_OPENCL_2_0_APIS 1
#define CL_USE_DEPRECATED_OPENCL_2_1_APIS 1

static void *opencl_handle = NULL;

static cl_context (*_clCreateContext)(const cl_context_properties *, cl_uint, const cl_device_id *, void (CL_CALLBACK *)(const char *, const void *, size_t, void *), void *, cl_int *) = NULL;

static cl_context (*_clCreateContextFromType)(const cl_context_properties *, cl_device_type, void (CL_CALLBACK *)(const char *, const void *, size_t, void *), void *, cl_int *) = NULL;

static cl_int (*_clSetMemObjectDestructorCallback)( cl_mem, void (CL_CALLBACK *)(cl_mem, void *), void *) = NULL;

static cl_int (*_clBuildProgram)(cl_program, cl_uint, const cl_device_id *, const char *, void (CL_CALLBACK *)(cl_program, void *), void *) = NULL;

static cl_int (*_clCompileProgram)(cl_program, cl_uint, const cl_device_id *, const char *, cl_uint, const cl_program *, const char **, void (CL_CALLBACK *)(cl_program, void *), void *) = NULL;

static cl_program (*_clLinkProgram)(cl_context, cl_uint, const cl_device_id *, const char *, cl_uint, const cl_program *, void (CL_CALLBACK *)(cl_program, void *), void *, cl_int *) = NULL;

static cl_int (*_clSetProgramReleaseCallback)(cl_program, void (CL_CALLBACK *)(cl_program, void *), void *) = NULL;

static cl_int (*_clSetEventCallback)( cl_event, cl_int, void (CL_CALLBACK *)(cl_event, cl_int, void *), void *) = NULL;

static cl_int (*_clEnqueueNativeKernel)(cl_command_queue, void (*user_func)(void *), void *, size_t, cl_uint, const cl_mem *, const void **, cl_uint, const cl_event *, cl_event *) = NULL;

static cl_int (*_clEnqueueSVMFree)(cl_command_queue, cl_uint, void *[], void (CL_CALLBACK * /*pfn_free_func*/)(cl_command_queue, cl_uint, void *[], void *), void *, cl_uint, const cl_event *, cl_event *) = NULL;

static cl_int (*_clEnqueueSVMFreeARM)(cl_command_queue, cl_uint, void *[], void (CL_CALLBACK * /*pfn_free_func*/)(cl_command_queue, cl_uint, void *[], void *), void *, cl_uint, const cl_event *, cl_event *) = NULL;


static void _init_androidopencl()
{
	opencl_handle = (void *) android_dlopen(getenv("LIBOPENCL") ? getenv("LIBOPENCL") : "libOpenCL.so", RTLD_LAZY);
}

static inline void hybris_opencl_initialize()
{
	_init_androidopencl();
}

/* Platform API */
HYBRIS_IMPLEMENT_FUNCTION3(opencl, cl_int, clGetPlatformIDs, cl_uint, cl_platform_id *, cl_uint *);

HYBRIS_IMPLEMENT_FUNCTION5(opencl, cl_int, clGetPlatformInfo, cl_platform_id, cl_platform_info, size_t, void *, size_t *);

/* Device APIs */
HYBRIS_IMPLEMENT_FUNCTION5(opencl, cl_int, clGetDeviceIDs, cl_platform_id, cl_device_type, cl_uint, cl_device_id *, cl_uint *);

HYBRIS_IMPLEMENT_FUNCTION5(opencl, cl_int, clGetDeviceInfo, cl_device_id, cl_device_info, size_t, void *, size_t *);

HYBRIS_IMPLEMENT_FUNCTION5(opencl, cl_int, clCreateSubDevices, cl_device_id, const cl_device_partition_property *, cl_uint, cl_device_id *, cl_uint *);

HYBRIS_IMPLEMENT_FUNCTION1(opencl, cl_int, clRetainDevice, cl_device_id);

HYBRIS_IMPLEMENT_FUNCTION1(opencl, cl_int, clReleaseDevice, cl_device_id);

HYBRIS_IMPLEMENT_FUNCTION3(opencl, cl_int, clSetDefaultDeviceCommandQueue, cl_context, cl_device_id, cl_command_queue);

HYBRIS_IMPLEMENT_FUNCTION3(opencl, cl_int, clGetDeviceAndHostTimer, cl_device_id, cl_ulong*, cl_ulong*);

HYBRIS_IMPLEMENT_FUNCTION2(opencl, cl_int, clGetHostTimer, cl_device_id, cl_ulong *) ;

/* Context APIs  */

cl_context clCreateContext(const cl_context_properties * properties,
                           cl_uint                       num_devices,
                           const cl_device_id *          devices,
                           void (CL_CALLBACK *           pfn_notify)(const char *, const void *, size_t, void *),
                           void *                        user_data,
                           cl_int *                      errcode_ret)
{
	HYBRIS_DLSYSM(opencl, &_clCreateContext, "clCreateContext");

	return (*_clCreateContext)(properties, num_devices, devices, pfn_notify, user_data, errcode_ret);
}

cl_context clCreateContextFromType(const cl_context_properties * properties,
                                   cl_device_type                device_type,
                                   void (CL_CALLBACK *           pfn_notify)(const char *, const void *, size_t, void *),
                                   void *                        user_data,
                                   cl_int *                      errcode_ret)
{
	HYBRIS_DLSYSM(opencl, &_clCreateContextFromType, "clCreateContextFromType");

	return (*_clCreateContextFromType)(properties, device_type, pfn_notify, user_data, errcode_ret);
}

HYBRIS_IMPLEMENT_FUNCTION1(opencl, cl_int, clRetainContext, cl_context);

HYBRIS_IMPLEMENT_FUNCTION1(opencl, cl_int, clReleaseContext, cl_context);

HYBRIS_IMPLEMENT_FUNCTION5(opencl, cl_int, clGetContextInfo, cl_context, cl_context_info, size_t, void *, size_t *);

/* Command Queue APIs */

HYBRIS_IMPLEMENT_FUNCTION4(opencl, cl_command_queue, clCreateCommandQueueWithProperties, cl_context, cl_device_id, const cl_queue_properties *, cl_int *);

HYBRIS_IMPLEMENT_FUNCTION1(opencl, cl_int, clRetainCommandQueue, cl_command_queue);

HYBRIS_IMPLEMENT_FUNCTION1(opencl, cl_int, clReleaseCommandQueue, cl_command_queue);

HYBRIS_IMPLEMENT_FUNCTION5(opencl, cl_int, clGetCommandQueueInfo, cl_command_queue, cl_command_queue_info, size_t, void *, size_t *);

#ifdef CL_USE_DEPRECATED_OPENCL_1_0_APIS
#warning CL_USE_DEPRECATED_OPENCL_1_0_APIS is defined. These APIs are unsupported and untested in OpenCL 1.1!
/*
 *  WARNING:
 *     This API introduces mutable state into the OpenCL implementation. It has been REMOVED
 *  to better facilitate thread safety.  The 1.0 API is not thread safe. It is not tested by the
 *  OpenCL 1.1 conformance test, and consequently may not work or may not work dependably.
 *  It is likely to be non-performant. Use of this API is not advised. Use at your own risk.
 *
 *  Software developers previously relying on this API are instructed to set the command queue
 *  properties when creating the queue, instead.
 */
HYBRIS_IMPLEMENT_FUNCTION4(opencl, cl_int, clSetCommandQueueProperty, cl_command_queue, cl_command_queue_properties, cl_bool, cl_command_queue_properties *);
#endif /* CL_USE_DEPRECATED_OPENCL_1_0_APIS */

/* Memory Object APIs */
HYBRIS_IMPLEMENT_FUNCTION5(opencl, cl_mem, clCreateBuffer, cl_context, cl_mem_flags, size_t, void *, cl_int *);

HYBRIS_IMPLEMENT_FUNCTION5(opencl, cl_mem, clCreateSubBuffer, cl_mem, cl_mem_flags, cl_buffer_create_type, const void *, cl_int *);

HYBRIS_IMPLEMENT_FUNCTION6(opencl, cl_mem, clCreateImage, cl_context, cl_mem_flags, const cl_image_format *, const cl_image_desc *, void *, cl_int *);

HYBRIS_IMPLEMENT_FUNCTION6(opencl, cl_mem, clCreatePipe, cl_context, cl_mem_flags, cl_uint, cl_uint, const cl_pipe_properties *, cl_int *);

HYBRIS_IMPLEMENT_FUNCTION1(opencl, cl_int, clRetainMemObject, cl_mem);

HYBRIS_IMPLEMENT_FUNCTION1(opencl, cl_int, clReleaseMemObject, cl_mem);

HYBRIS_IMPLEMENT_FUNCTION6(opencl, cl_int, clGetSupportedImageFormats, cl_context, cl_mem_flags, cl_mem_object_type, cl_uint, cl_image_format *, cl_uint *);

HYBRIS_IMPLEMENT_FUNCTION5(opencl, cl_int, clGetMemObjectInfo, cl_mem, cl_mem_info, size_t, void *, size_t *);

HYBRIS_IMPLEMENT_FUNCTION5(opencl, cl_int, clGetImageInfo, cl_mem, cl_image_info, size_t, void *, size_t *);

HYBRIS_IMPLEMENT_FUNCTION5(opencl, cl_int, clGetPipeInfo, cl_mem, cl_pipe_info, size_t, void *, size_t *);

cl_int clSetMemObjectDestructorCallback(cl_mem memobj,
                                        void (CL_CALLBACK * pfn_notify)( cl_mem, void*),
                                        void * user_data)
{
	HYBRIS_DLSYSM(opencl, &_clSetMemObjectDestructorCallback, "clSetMemObjectDestructorCallback");

	return (*_clSetMemObjectDestructorCallback)(memobj, pfn_notify, user_data);
}

/* SVM Allocation APIs */
HYBRIS_IMPLEMENT_FUNCTION4(opencl, void *, clSVMAlloc, cl_context, cl_svm_mem_flags, size_t, cl_uint);

HYBRIS_IMPLEMENT_FUNCTION2(opencl, void, clSVMFree, cl_context, void *);

/* Sampler APIs  */
HYBRIS_IMPLEMENT_FUNCTION3(opencl, cl_sampler, clCreateSamplerWithProperties, cl_context, const cl_sampler_properties *, cl_int *);

HYBRIS_IMPLEMENT_FUNCTION1(opencl, cl_int, clRetainSampler, cl_sampler);

HYBRIS_IMPLEMENT_FUNCTION1(opencl, cl_int, clReleaseSampler, cl_sampler);

HYBRIS_IMPLEMENT_FUNCTION5(opencl, cl_int, clGetSamplerInfo, cl_sampler, cl_sampler_info, size_t, void *, size_t *);

/* Program Object APIs  */
HYBRIS_IMPLEMENT_FUNCTION5(opencl, cl_program, clCreateProgramWithSource, cl_context, cl_uint, const char **, const size_t *, cl_int *);

HYBRIS_IMPLEMENT_FUNCTION7(opencl, cl_program, clCreateProgramWithBinary, cl_context, cl_uint, const cl_device_id *, const size_t *, const unsigned char **, cl_int *, cl_int *);

HYBRIS_IMPLEMENT_FUNCTION5(opencl, cl_program, clCreateProgramWithBuiltInKernels, cl_context, cl_uint, const cl_device_id *, const char *, cl_int *);

HYBRIS_IMPLEMENT_FUNCTION4(opencl, cl_program, clCreateProgramWithIL, cl_context, const void*, size_t, cl_int*);

HYBRIS_IMPLEMENT_FUNCTION1(opencl, cl_int, clRetainProgram, cl_program);

HYBRIS_IMPLEMENT_FUNCTION1(opencl, cl_int, clReleaseProgram, cl_program);

cl_int clBuildProgram(cl_program           program,
                      cl_uint              num_devices,
                      const cl_device_id * device_list,
                      const char *         options,
                      void (CL_CALLBACK *  pfn_notify)(cl_program /* program */, void * /* user_data */),
                      void *               user_data)
{
	HYBRIS_DLSYSM(opencl, &_clBuildProgram, "clBuildProgram");

	return (*_clBuildProgram)(program, num_devices, device_list, options, pfn_notify, user_data);
}

cl_int clCompileProgram(cl_program           program,
                 cl_uint              num_devices,
                 const cl_device_id * device_list,
                 const char *         options,
                 cl_uint              num_input_headers,
                 const cl_program *   input_headers,
                 const char **        header_include_names,
                 void (CL_CALLBACK *  pfn_notify)(cl_program /* program */, void * /* user_data */),
                 void *               user_data)
{
	HYBRIS_DLSYSM(opencl, &_clCompileProgram, "clCompileProgram");
	return (*_clCompileProgram)(program, num_devices, device_list, options, num_input_headers, input_headers, header_include_names, pfn_notify, user_data);
}

cl_program clLinkProgram(cl_context           context,
              cl_uint              num_devices,
              const cl_device_id * device_list,
              const char *         options,
              cl_uint              num_input_programs,
              const cl_program *   input_programs,
              void (CL_CALLBACK *  pfn_notify)(cl_program /* program */, void * /* user_data */),
              void *               user_data,
              cl_int *             errcode_ret)
{
	HYBRIS_DLSYSM(opencl, &_clLinkProgram, "clLinkProgram");
	return (*_clLinkProgram)(context, num_devices, device_list, options, num_input_programs, input_programs, pfn_notify, user_data, errcode_ret);
}

cl_int clSetProgramReleaseCallback(cl_program   program,
                            void (CL_CALLBACK * pfn_notify)(cl_program /* program */, void * /* user_data */),
                            void *              user_data)
{
	HYBRIS_DLSYSM(opencl, &_clSetProgramReleaseCallback, "clSetProgramReleaseCallback");
	return (*_clSetProgramReleaseCallback)(program, pfn_notify, user_data);
}

HYBRIS_IMPLEMENT_FUNCTION4(opencl, cl_int, clSetProgramSpecializationConstant, cl_program, cl_uint, size_t, const void*);

HYBRIS_IMPLEMENT_FUNCTION1(opencl, cl_int, clUnloadPlatformCompiler, cl_platform_id);

HYBRIS_IMPLEMENT_FUNCTION5(opencl, cl_int, clGetProgramInfo, cl_program, cl_program_info, size_t, void *, size_t *);

HYBRIS_IMPLEMENT_FUNCTION6(opencl, cl_int, clGetProgramBuildInfo, cl_program, cl_device_id, cl_program_build_info, size_t, void *, size_t *);

/* Kernel Object APIs */
HYBRIS_IMPLEMENT_FUNCTION3(opencl, cl_kernel, clCreateKernel, cl_program, const char *, cl_int *);

HYBRIS_IMPLEMENT_FUNCTION4(opencl, cl_int, clCreateKernelsInProgram, cl_program, cl_uint, cl_kernel *, cl_uint *);

HYBRIS_IMPLEMENT_FUNCTION2(opencl, cl_kernel, clCloneKernel, cl_kernel, cl_int*);

HYBRIS_IMPLEMENT_FUNCTION1(opencl, cl_int, clRetainKernel, cl_kernel);

HYBRIS_IMPLEMENT_FUNCTION1(opencl, cl_int, clReleaseKernel, cl_kernel);

HYBRIS_IMPLEMENT_FUNCTION4(opencl, cl_int, clSetKernelArg, cl_kernel, cl_uint, size_t, const void *);

HYBRIS_IMPLEMENT_FUNCTION3(opencl, cl_int, clSetKernelArgSVMPointer, cl_kernel, cl_uint, const void *);

HYBRIS_IMPLEMENT_FUNCTION4(opencl, cl_int, clSetKernelExecInfo, cl_kernel, cl_kernel_exec_info, size_t, const void *);

HYBRIS_IMPLEMENT_FUNCTION5(opencl, cl_int, clGetKernelInfo, cl_kernel, cl_kernel_info, size_t, void *, size_t *);

HYBRIS_IMPLEMENT_FUNCTION6(opencl, cl_int, clGetKernelArgInfo, cl_kernel, cl_uint, cl_kernel_arg_info, size_t, void *, size_t *);


HYBRIS_IMPLEMENT_FUNCTION6(opencl, cl_int, clGetKernelWorkGroupInfo, cl_kernel, cl_device_id, cl_kernel_work_group_info, size_t, void *, size_t *);

HYBRIS_IMPLEMENT_FUNCTION8(opencl, cl_int, clGetKernelSubGroupInfo, cl_kernel, cl_device_id, cl_kernel_sub_group_info, size_t, const void*, size_t, void*, size_t*);

/* Event Object APIs  */
HYBRIS_IMPLEMENT_FUNCTION2(opencl, cl_int, clWaitForEvents, cl_uint, const cl_event *);

HYBRIS_IMPLEMENT_FUNCTION5(opencl, cl_int, clGetEventInfo, cl_event, cl_event_info, size_t, void *, size_t *);

HYBRIS_IMPLEMENT_FUNCTION2(opencl, cl_event, clCreateUserEvent, cl_context, cl_int *);

HYBRIS_IMPLEMENT_FUNCTION1(opencl, cl_int, clRetainEvent, cl_event);

HYBRIS_IMPLEMENT_FUNCTION1(opencl, cl_int, clReleaseEvent, cl_event);

HYBRIS_IMPLEMENT_FUNCTION2(opencl, cl_int, clSetUserEventStatus, cl_event, cl_int);

cl_int clSetEventCallback(cl_event    event,
                          cl_int      command_exec_callback_type,
                          void (CL_CALLBACK * pfn_notify)(cl_event, cl_int, void *),
                          void *      user_data)
{
	HYBRIS_DLSYSM(opencl, &_clSetEventCallback, "clSetEventCallback");

	return (*_clSetEventCallback)(event, command_exec_callback_type, pfn_notify, user_data);
}

/* Profiling APIs  */
HYBRIS_IMPLEMENT_FUNCTION5(opencl, cl_int, clGetEventProfilingInfo, cl_event, cl_profiling_info, size_t, void *, size_t *);

/* Flush and Finish APIs */
HYBRIS_IMPLEMENT_FUNCTION1(opencl, cl_int, clFlush, cl_command_queue);

HYBRIS_IMPLEMENT_FUNCTION1(opencl, cl_int, clFinish, cl_command_queue);

/* Enqueued Commands APIs */
HYBRIS_IMPLEMENT_FUNCTION9(opencl, cl_int, clEnqueueReadBuffer, cl_command_queue, cl_mem, cl_bool, size_t, size_t, void *, cl_uint, const cl_event *, cl_event *);

HYBRIS_IMPLEMENT_FUNCTION14(opencl, cl_int, clEnqueueReadBufferRect, cl_command_queue, cl_mem, cl_bool, const size_t *, const size_t *, const size_t *, size_t, size_t, size_t, size_t, void *, cl_uint, const cl_event *, cl_event *);

HYBRIS_IMPLEMENT_FUNCTION9(opencl, cl_int, clEnqueueWriteBuffer, cl_command_queue, cl_mem, cl_bool, size_t, size_t, const void *, cl_uint, const cl_event *, cl_event *);

HYBRIS_IMPLEMENT_FUNCTION14(opencl, cl_int, clEnqueueWriteBufferRect, cl_command_queue, cl_mem, cl_bool, const size_t *, const size_t *, const size_t *, size_t, size_t, size_t, size_t, const void *, cl_uint, const cl_event *, cl_event *);

HYBRIS_IMPLEMENT_FUNCTION9(opencl, cl_int, clEnqueueFillBuffer, cl_command_queue, cl_mem, const void *, size_t, size_t, size_t, cl_uint, const cl_event *, cl_event *);

HYBRIS_IMPLEMENT_FUNCTION9(opencl, cl_int, clEnqueueCopyBuffer, cl_command_queue, cl_mem, cl_mem, size_t, size_t, size_t, cl_uint, const cl_event *, cl_event *);

HYBRIS_IMPLEMENT_FUNCTION13(opencl, cl_int, clEnqueueCopyBufferRect, cl_command_queue, cl_mem, cl_mem, const size_t *, const size_t *, const size_t *, size_t, size_t, size_t, size_t, cl_uint, const cl_event *, cl_event *);

HYBRIS_IMPLEMENT_FUNCTION11(opencl, cl_int, clEnqueueReadImage, cl_command_queue, cl_mem, cl_bool, const size_t *, const size_t *, size_t, size_t, void *, cl_uint, const cl_event *, cl_event *);

HYBRIS_IMPLEMENT_FUNCTION11(opencl, cl_int, clEnqueueWriteImage, cl_command_queue, cl_mem, cl_bool, const size_t *, const size_t *, size_t, size_t, const void *, cl_uint, const cl_event *, cl_event *);

HYBRIS_IMPLEMENT_FUNCTION8(opencl, cl_int, clEnqueueFillImage, cl_command_queue, cl_mem, const void *, const size_t *, const size_t *, cl_uint, const cl_event *, cl_event *);

HYBRIS_IMPLEMENT_FUNCTION9(opencl, cl_int, clEnqueueCopyImage, cl_command_queue, cl_mem, cl_mem, const size_t *, const size_t *, const size_t *, cl_uint, const cl_event *, cl_event *);

HYBRIS_IMPLEMENT_FUNCTION9(opencl, cl_int, clEnqueueCopyImageToBuffer, cl_command_queue, cl_mem, cl_mem, const size_t *, const size_t *, size_t, cl_uint, const cl_event *, cl_event *);

HYBRIS_IMPLEMENT_FUNCTION9(opencl, cl_int, clEnqueueCopyBufferToImage, cl_command_queue, cl_mem, cl_mem, size_t, const size_t *, const size_t *, cl_uint, const cl_event *, cl_event *);

HYBRIS_IMPLEMENT_FUNCTION10(opencl, void *, clEnqueueMapBuffer, cl_command_queue, cl_mem, cl_bool, cl_map_flags, size_t, size_t, cl_uint, const cl_event *, cl_event *, cl_int *);

HYBRIS_IMPLEMENT_FUNCTION12(opencl, void *, clEnqueueMapImage, cl_command_queue, cl_mem, cl_bool, cl_map_flags, const size_t *, const size_t *, size_t *, size_t *, cl_uint, const cl_event *, cl_event *, cl_int *);

HYBRIS_IMPLEMENT_FUNCTION6(opencl, cl_int, clEnqueueUnmapMemObject, cl_command_queue, cl_mem, void *, cl_uint, const cl_event *, cl_event *);

HYBRIS_IMPLEMENT_FUNCTION7(opencl, cl_int, clEnqueueMigrateMemObjects, cl_command_queue, cl_uint, const cl_mem *, cl_mem_migration_flags, cl_uint, const cl_event *, cl_event *);

HYBRIS_IMPLEMENT_FUNCTION9(opencl, cl_int, clEnqueueNDRangeKernel, cl_command_queue, cl_kernel, cl_uint, const size_t *, const size_t *, const size_t *, cl_uint, const cl_event *, cl_event *);

cl_int clEnqueueNativeKernel(cl_command_queue  command_queue,
                             void (*user_func)(void *),
                             void *            args,
                             size_t            cb_args,
                             cl_uint           num_mem_objects,
                             const cl_mem *    mem_list,
                             const void **     args_mem_loc,
                             cl_uint           num_events_in_wait_list,
                             const cl_event *  event_wait_list,
                             cl_event *        event)
{
	HYBRIS_DLSYSM(opencl, &_clEnqueueNativeKernel, "clEnqueueNativeKernel");

	return (*_clEnqueueNativeKernel)(command_queue, user_func, args, cb_args, num_mem_objects, mem_list, args_mem_loc, num_events_in_wait_list, event_wait_list, event);
}

HYBRIS_IMPLEMENT_FUNCTION4(opencl, cl_int, clEnqueueMarkerWithWaitList, cl_command_queue, cl_uint, const cl_event *, cl_event *);

HYBRIS_IMPLEMENT_FUNCTION4(opencl, cl_int, clEnqueueBarrierWithWaitList, cl_command_queue, cl_uint, const cl_event *, cl_event *);

cl_int clEnqueueSVMFree(cl_command_queue  command_queue,
                        cl_uint           num_svm_pointers,
                        void **           svm_pointers,
                        void (CL_CALLBACK * pfn_free_func)(cl_command_queue, cl_uint, void *[], void *),
                        void *            user_data,
                        cl_uint           num_events_in_wait_list,
                        const cl_event *  event_wait_list,
                        cl_event *        event)
{
	HYBRIS_DLSYSM(opencl, &_clEnqueueSVMFree, "clEnqueueSVMFree");

	return (*_clEnqueueSVMFree)(command_queue, num_svm_pointers, svm_pointers, pfn_free_func, user_data, num_events_in_wait_list, event_wait_list, event);
}

HYBRIS_IMPLEMENT_FUNCTION8(opencl, cl_int, clEnqueueSVMMemcpy, cl_command_queue, cl_bool, void *, const void *, size_t, cl_uint, const cl_event *, cl_event *);

HYBRIS_IMPLEMENT_FUNCTION8(opencl, cl_int, clEnqueueSVMMemFill, cl_command_queue, void *, const void *, size_t, size_t, cl_uint, const cl_event *, cl_event *);

HYBRIS_IMPLEMENT_FUNCTION8(opencl, cl_int, clEnqueueSVMMap, cl_command_queue, cl_bool, cl_map_flags, void *, size_t, cl_uint, const cl_event *, cl_event *);

HYBRIS_IMPLEMENT_FUNCTION5(opencl, cl_int, clEnqueueSVMUnmap, cl_command_queue, void *, cl_uint, const cl_event *, cl_event *);

HYBRIS_IMPLEMENT_FUNCTION8(opencl, cl_int, clEnqueueSVMMigrateMem, cl_command_queue, cl_uint, const void **, const size_t *, cl_mem_migration_flags, cl_uint, const cl_event *, cl_event *);

/* Extension function access
 *
 * Returns the extension function address for the given function name,
 * or NULL if a valid function can not be found.  The client must
 * check to make sure the address is not NULL, before using or
 * calling the returned function address.
 */
HYBRIS_IMPLEMENT_FUNCTION2(opencl, void *, clGetExtensionFunctionAddressForPlatform, cl_platform_id, const char *);

HYBRIS_IMPLEMENT_FUNCTION8(opencl, cl_mem, clCreateImage2D, cl_context, cl_mem_flags, const cl_image_format *, size_t, size_t, size_t, void *, cl_int *);

HYBRIS_IMPLEMENT_FUNCTION10(opencl, cl_mem, clCreateImage3D, cl_context, cl_mem_flags, const cl_image_format *, size_t, size_t, size_t, size_t, size_t, void *, cl_int *);

HYBRIS_IMPLEMENT_FUNCTION2(opencl, cl_int, clEnqueueMarker, cl_command_queue, cl_event *);

HYBRIS_IMPLEMENT_FUNCTION3(opencl, cl_int, clEnqueueWaitForEvents, cl_command_queue, cl_uint, const cl_event *);

HYBRIS_IMPLEMENT_FUNCTION1(opencl, cl_int, clEnqueueBarrier, cl_command_queue);

HYBRIS_IMPLEMENT_FUNCTION0(opencl, cl_int, clUnloadCompiler);

HYBRIS_IMPLEMENT_FUNCTION1(opencl, void *, clGetExtensionFunctionAddress, const char *);

HYBRIS_IMPLEMENT_FUNCTION4(opencl, cl_command_queue, clCreateCommandQueue, cl_context, cl_device_id, cl_command_queue_properties, cl_int *);

HYBRIS_IMPLEMENT_FUNCTION5(opencl, cl_sampler, clCreateSampler, cl_context, cl_bool, cl_addressing_mode, cl_filter_mode, cl_int *);

HYBRIS_IMPLEMENT_FUNCTION5(opencl, cl_int, clEnqueueTask, cl_command_queue, cl_kernel, cl_uint, const cl_event *, cl_event *);


/*
 * cl_khr_icd extension
 */

HYBRIS_IMPLEMENT_FUNCTION3(opencl, cl_int, clIcdGetPlatformIDsKHR, cl_uint, cl_platform_id *, cl_uint *);

/*
 * cl_khr_terminate_context extension
 */

HYBRIS_IMPLEMENT_FUNCTION1(opencl, cl_int, clTerminateContextKHR, cl_context);

/*
 * cl_ext_device_fission extension
 */

HYBRIS_IMPLEMENT_FUNCTION1(opencl, cl_int, clReleaseDeviceEXT, cl_device_id);

HYBRIS_IMPLEMENT_FUNCTION1(opencl, cl_int, clRetainDeviceEXT, cl_device_id);

HYBRIS_IMPLEMENT_FUNCTION5(opencl, cl_int, clCreateSubDevicesEXT, cl_device_id, const cl_device_partition_property_ext *, cl_uint, cl_device_id *, cl_uint *);

/*
 * cl_qcom_ext_host_ptr extension
 */

HYBRIS_IMPLEMENT_FUNCTION8(opencl, cl_int, clGetDeviceImageInfoQCOM, cl_device_id, size_t, size_t, const cl_image_format *, cl_image_pitch_info_qcom, size_t, void *, size_t *);

/*
 * cl_img_use_gralloc_ptr extension
 */

HYBRIS_IMPLEMENT_FUNCTION6(opencl, cl_int, clEnqueueAcquireGrallocObjectsIMG, cl_command_queue, cl_uint, const cl_mem *, cl_uint, const cl_event *, cl_event *);

HYBRIS_IMPLEMENT_FUNCTION6(opencl, cl_int, clEnqueueReleaseGrallocObjectsIMG, cl_command_queue, cl_uint, const cl_mem *, cl_uint, const cl_event *, cl_event *);

/*
 * cl_khr_subgroups extension
 */

HYBRIS_IMPLEMENT_FUNCTION8(opencl, cl_int, clGetKernelSubGroupInfoKHR, cl_kernel, cl_device_id, cl_kernel_sub_group_info, size_t, const void *, size_t, void*, size_t*);

/*
 * cl_arm_import_memory extension
 */

HYBRIS_IMPLEMENT_FUNCTION6(opencl, cl_mem, clImportMemoryARM, cl_context, cl_mem_flags, const cl_import_properties_arm *, void *, size_t, cl_int *);

/*
 * cl_arm_shared_virtual_memory extension
 */

HYBRIS_IMPLEMENT_FUNCTION4(opencl, void *, clSVMAllocARM, cl_context, cl_svm_mem_flags_arm, size_t, cl_uint);

HYBRIS_IMPLEMENT_FUNCTION2(opencl, void, clSVMFreeARM, cl_context, void *);

cl_int clEnqueueSVMFreeARM(cl_command_queue  command_queue,
                           cl_uint           num_svm_pointers,
                           void **           svm_pointers,
                           void (CL_CALLBACK * pfn_free_func)(cl_command_queue, cl_uint, void *[], void *),
                           void *            user_data,
                           cl_uint           num_events_in_wait_list,
                           const cl_event *  event_wait_list,
                           cl_event *        event)
{
	HYBRIS_DLSYSM(opencl, &_clEnqueueSVMFreeARM, "clEnqueueSVMFreeARM");

	return (*_clEnqueueSVMFreeARM)(command_queue, num_svm_pointers, svm_pointers, pfn_free_func, user_data, num_events_in_wait_list, event_wait_list, event);
}

HYBRIS_IMPLEMENT_FUNCTION8(opencl, cl_int, clEnqueueSVMMemcpyARM, cl_command_queue, cl_bool, void *, const void *, size_t, cl_uint, const cl_event *, cl_event *);

HYBRIS_IMPLEMENT_FUNCTION8(opencl, cl_int, clEnqueueSVMMemFillARM, cl_command_queue, void *, const void *, size_t, size_t, cl_uint, const cl_event *, cl_event *);

HYBRIS_IMPLEMENT_FUNCTION8(opencl, cl_int, clEnqueueSVMMapARM, cl_command_queue, cl_bool, cl_map_flags, void *, size_t, cl_uint, const cl_event *, cl_event *);

HYBRIS_IMPLEMENT_FUNCTION5(opencl, cl_int, clEnqueueSVMUnmapARM, cl_command_queue, void *, cl_uint, const cl_event *, cl_event *);

HYBRIS_IMPLEMENT_FUNCTION3(opencl, cl_int, clSetKernelArgSVMPointerARM, cl_kernel, cl_uint, const void *);

HYBRIS_IMPLEMENT_FUNCTION4(opencl, cl_int, clSetKernelExecInfoARM, cl_kernel, cl_kernel_exec_info_arm, size_t, const void *);


/*
 * CL GL
 */

HYBRIS_IMPLEMENT_FUNCTION4(opencl, cl_mem, clCreateFromGLBuffer, cl_context, cl_mem_flags, cl_GLuint, int *);

HYBRIS_IMPLEMENT_FUNCTION6(opencl, cl_mem, clCreateFromGLTexture, cl_context, cl_mem_flags, cl_GLenum, cl_GLint, cl_GLuint, cl_int *);

HYBRIS_IMPLEMENT_FUNCTION4(opencl, cl_mem, clCreateFromGLRenderbuffer, cl_context, cl_mem_flags, cl_GLuint, cl_int *);

HYBRIS_IMPLEMENT_FUNCTION3(opencl, cl_int, clGetGLObjectInfo, cl_mem, cl_gl_object_type *, cl_GLuint *);

HYBRIS_IMPLEMENT_FUNCTION5(opencl, cl_int, clGetGLTextureInfo, cl_mem, cl_gl_texture_info, size_t, void *, size_t *);

HYBRIS_IMPLEMENT_FUNCTION6(opencl, cl_int, clEnqueueAcquireGLObjects, cl_command_queue, cl_uint, const cl_mem *, cl_uint, const cl_event *, cl_event *);

HYBRIS_IMPLEMENT_FUNCTION6(opencl, cl_int, clEnqueueReleaseGLObjects, cl_command_queue, cl_uint, const cl_mem *, cl_uint, const cl_event *, cl_event *);


/* Deprecated OpenCL 1.1 APIs */
HYBRIS_IMPLEMENT_FUNCTION6(opencl, cl_mem, clCreateFromGLTexture2D, cl_context, cl_mem_flags, cl_GLenum, cl_GLint, cl_GLuint, cl_int *);

HYBRIS_IMPLEMENT_FUNCTION6(opencl, cl_mem, clCreateFromGLTexture3D, cl_context, cl_mem_flags, cl_GLenum, cl_GLint, cl_GLuint, cl_int *);

/* cl_khr_gl_sharing extension  */
HYBRIS_IMPLEMENT_FUNCTION5(opencl, cl_int, clGetGLContextInfoKHR, const cl_context_properties *, cl_gl_context_info, size_t, void *, size_t *);

/*
 *  cl_khr_gl_event  extension
 *  See section 9.9 in the OpenCL 1.1 spec for more information
 */
HYBRIS_IMPLEMENT_FUNCTION3(opencl, cl_event, clCreateEventFromGLsyncKHR, cl_context, cl_GLsync, cl_int *);


// vim:ts=4:sw=4:noexpandtab
