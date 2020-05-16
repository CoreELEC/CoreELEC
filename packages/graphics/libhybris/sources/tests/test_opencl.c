/*
 * test_opencl: Test OpenCL implementation
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
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include <CL/cl.h>
#include <CL/cl_ext.h>

#define MEM_SIZE (128)

char *source_str = "__kernel void test(__global char* string)\n\
{\n\
string[0] = 'O';\n\
string[1] = 'p';\n\
string[2] = 'e';\n\
string[3] = 'n';\n\
string[4] = 'C';\n\
string[5] = 'L';\n\
string[6] = ' ';\n\
string[7] = 'S';\n\
string[8] = 'u';\n\
string[9] = 'c';\n\
string[10] = 'c';\n\
string[11] = 'e';\n\
string[12] = 's';\n\
string[13] = 's';\n\
string[14] = '!';\n\
string[15] = '\\0';\n\
}\n\
";

void printPlatformInfo(cl_platform_id platform, cl_platform_info param_name, const char *name) {
    size_t value_size;
    char *param_value;
    clGetPlatformInfo(platform, param_name, 0, NULL, &value_size);
    param_value = (char *)malloc(value_size);
    cl_int ret = clGetPlatformInfo(platform, param_name, value_size, param_value, NULL);
    if (ret == CL_SUCCESS) {
        printf("%s: %s\n", name, param_value);
    }
    free(param_value);
}

void printDeviceInfo(cl_device_id device, cl_device_info param_name, const char *name) {
    size_t value_size;
    char *param_value;
    clGetDeviceInfo(device, param_name, 0, NULL, &value_size);
    param_value = (char *)malloc(value_size);
    cl_int ret = clGetDeviceInfo(device, param_name, value_size, param_value, NULL);
    if (ret == CL_SUCCESS) {
        printf("%s: %s\n", name, param_value);
    }
    free(param_value);
}

int main(int argc, char *argv[])
{
    cl_platform_id *platforms = NULL;
    cl_uint num_platforms;
    cl_int ret;

    /* Get Platforms */
    ret = clGetPlatformIDs(0, NULL, &num_platforms);
    platforms = (cl_platform_id *) malloc(sizeof(cl_platform_id) * num_platforms);
    ret = clGetPlatformIDs(num_platforms, platforms, NULL);
    assert(ret == CL_SUCCESS);
    printf("Found %i platforms\n", num_platforms);

    int i;
    for (i = 0; i < num_platforms; ++i) {
        /* Prinf platform information */
        printf("Platform %i\n", i);
        printPlatformInfo(platforms[i], CL_PLATFORM_NAME, "CL_PLATFORM_NAME");
        printPlatformInfo(platforms[i], CL_PLATFORM_VENDOR, "CL_PLATFORM_VENDOR");
        printPlatformInfo(platforms[i], CL_PLATFORM_VERSION, "CL_PLATFORM_VERSION");
        printPlatformInfo(platforms[i], CL_PLATFORM_PROFILE, "CL_PLATFORM_PROFILE");
        printPlatformInfo(platforms[i], CL_PLATFORM_EXTENSIONS, "CL_PLATFORM_EXTENSIONS");

        cl_device_id *devices = NULL;
        cl_uint num_devices;

        /* Get Devices */
        ret = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_DEFAULT, 0, NULL, &num_devices);
        devices = (cl_device_id *) malloc(sizeof(cl_device_id) * num_devices);
        ret = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_DEFAULT, num_devices, devices, NULL);
        assert(ret == CL_SUCCESS);
        printf("Found %i devices\n", num_devices);

        int dev;
        for (dev = 0; dev < num_devices; ++dev) {
            /* Prinf device information */
            printf("Device %i\n", i);
            printDeviceInfo(devices[dev], CL_DEVICE_NAME, "CL_DEVICE_NAME");
            printDeviceInfo(devices[dev], CL_DEVICE_VENDOR, "CL_DEVICE_VENDOR");
            printDeviceInfo(devices[dev], CL_DRIVER_VERSION, "CL_DRIVER_VERSION");
            printDeviceInfo(devices[dev], CL_DEVICE_VERSION, "CL_DEVICE_VERSION");
            printDeviceInfo(devices[dev], CL_DEVICE_OPENCL_C_VERSION, "CL_DEVICE_OPENCL_C_VERSION");
            printDeviceInfo(devices[dev], CL_DEVICE_PROFILE, "CL_DEVICE_PROFILE");
            printDeviceInfo(devices[dev], CL_DEVICE_EXTENSIONS, "CL_DEVICE_EXTENSIONS");
            printf("\n");

            cl_context context = NULL;
            cl_command_queue command_queue = NULL;
            cl_mem memobj = NULL;
            cl_program program = NULL;
            cl_kernel kernel = NULL;
            char string[MEM_SIZE];
            size_t source_size = strlen(source_str);

            printf("Running OpenCL test program...\n");

            /* Create OpenCL context */
            context = clCreateContext(NULL, 1, &devices[dev], NULL, NULL, &ret);
            assert(ret == CL_SUCCESS);

            /* Create Command Queue */
            command_queue = clCreateCommandQueue(context, devices[dev], 0, &ret);
            assert(ret == CL_SUCCESS);

            /* Create Memory Buffer */
            memobj = clCreateBuffer(context, CL_MEM_READ_WRITE, MEM_SIZE * sizeof(char), NULL, &ret);
            assert(ret == CL_SUCCESS);

            /* Create Kernel Program from the source */
            program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret);
            assert(ret == CL_SUCCESS);

            /* Build Kernel Program */
            ret = clBuildProgram(program, 1, &devices[dev], NULL, NULL, NULL);
            assert(ret == CL_SUCCESS);

            /* Create OpenCL Kernel */
            kernel = clCreateKernel(program, "test", &ret);
            assert(ret == CL_SUCCESS);

            /* Set OpenCL Kernel Parameters */
            ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&memobj);
            assert(ret == CL_SUCCESS);

            /* Execute OpenCL Kernel */
            ret = clEnqueueTask(command_queue, kernel, 0, NULL, NULL);
            assert(ret == CL_SUCCESS);

            /* Copy results from the memory buffer */
            ret = clEnqueueReadBuffer(command_queue, memobj, CL_TRUE, 0, MEM_SIZE * sizeof(char),string, 0, NULL, NULL);
            assert(ret == CL_SUCCESS);

            /* Display Result */
            printf("Result: %s\n", string);

            /* Finalization */
            ret = clFlush(command_queue);
            assert(ret == CL_SUCCESS);
            ret = clFinish(command_queue);
            assert(ret == CL_SUCCESS);
            ret = clReleaseKernel(kernel);
            assert(ret == CL_SUCCESS);
            ret = clReleaseProgram(program);
            assert(ret == CL_SUCCESS);
            ret = clReleaseMemObject(memobj);
            assert(ret == CL_SUCCESS);
            ret = clReleaseCommandQueue(command_queue);
            assert(ret == CL_SUCCESS);
            ret = clReleaseContext(context);
            assert(ret == CL_SUCCESS);
        }
        free(devices);
    }
    free(platforms);

    return 0;
}
