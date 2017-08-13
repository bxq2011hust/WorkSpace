#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "stdlib.h"
#include "stdio.h"

bool initCuda();
cudaError_t addWithCuda(int *c, const int *a, const int *b, unsigned int size);

__global__ void addKernel(int *c, const int *a, const int *b,clock_t* time)
{
    clock_t start = clock();
    int i = threadIdx.x;
    c[i] = a[i] + b[i];
    *time = clock() - start;
}

int main()
{
    initCuda();
    const int arraySize = 5;
    const int a[arraySize] = { 1, 2, 3, 4, 5 };
    const int b[arraySize] = { 10, 20, 30, 40, 50 };
    int c[arraySize] = { 0 };

    // Add vectors in parallel.
    cudaError_t cudaStatus = addWithCuda(c, a, b, arraySize);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "addWithCuda failed!");
        return 1;
    }

    printf("{1,2,3,4,5} + {10,20,30,40,50} = {%d,%d,%d,%d,%d}\n",
        c[0], c[1], c[2], c[3], c[4]);

    // cudaDeviceReset must be called before exiting in order for profiling and
    // tracing tools such as Nsight and Visual Profiler to show complete traces.
    cudaStatus = cudaDeviceReset();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaDeviceReset failed!");
        return 1;
    }
    return 0;
}

void printDeviceProp(const cudaDeviceProp &prop)
{
    printf("---------------------------------------------------- \n");
    printf("Device Name : %s \n", prop.name);
    printf("totalGlobalMem : %zu MB\n",prop.totalGlobalMem/(1<<20));
    printf("sharedMemPerBlock : %zu B \n", prop.sharedMemPerBlock);
    printf("regsPerBlock : %d \n", prop.regsPerBlock);
    printf("warpSize : %d \n", prop.warpSize);
    printf("memPitch : %zu B \n", prop.memPitch);
    printf("maxThreadsPerBlock : %d \n", prop.maxThreadsPerBlock);
    printf("maxThreadsDim[0 - 2] : %d %d %d \n", prop.maxThreadsDim[0], prop.maxThreadsDim[1], prop.maxThreadsDim[2]);
    printf("maxGridSize[0 - 2] : %d %d %d \n", prop.maxGridSize[0], prop.maxGridSize[1], prop.maxGridSize[2]);
    printf("totalConstMem : %zu B \n", prop.totalConstMem);
    printf("major.minor : %d.%d \n", prop.major, prop.minor);
    printf("clockRate : %d khz \n", prop.clockRate);
    printf("textureAlignment : %zu B \n", prop.textureAlignment);
    printf("deviceOverlap : %d \n", prop.deviceOverlap);
    printf("multiProcessorCount : %d \n", prop.multiProcessorCount);
    printf("---------------------------------------------------- \n");
}
bool initCuda()
{
    cudaError_t cudaStatus;

    int count;
    cudaGetDeviceCount(&count);
    if(count == 0)
    {
        printf("There is no device \n");
        return false;
    }
    int i;
    for(i=0;i<count;++i)
    {
        cudaDeviceProp prop;
        if(cudaGetDeviceProperties(&prop,i) == cudaSuccess)
        {
            if(prop.major>=1)
            {
                //打印设备信息
                printf("cuda device id: %d\n",i);
                printDeviceProp(prop);
                break;
            } 
        }
    }

    if(i==count) 
    {
        printf("There is no device supporting cuda 1.x\n");
        return false;
    }
    
    cudaStatus = cudaSetDevice(0);
    return cudaStatus;
}

// Helper function for using CUDA to add vectors in parallel.
cudaError_t addWithCuda(int *c, const int *a, const int *b, unsigned int size)
{
    int *dev_a = 0;
    int *dev_b = 0;
    int *dev_c = 0;
    cudaError_t cudaStatus;
    clock_t* time;
clock_t time_used;
    // Allocate GPU buffers for three vectors (two input, one output)    .
    cudaStatus = cudaMalloc((void**)&dev_c, size * sizeof(int));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }

    cudaStatus = cudaMalloc((void**)&dev_a, size * sizeof(int));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }

    cudaStatus = cudaMalloc((void**)&dev_b, size * sizeof(int));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }

    cudaStatus = cudaMalloc((void**)&time, sizeof(clock_t));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }


    // Copy input vectors from host memory to GPU buffers.
    cudaStatus = cudaMemcpy(dev_a, a, size * sizeof(int), cudaMemcpyHostToDevice);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }

    cudaStatus = cudaMemcpy(dev_b, b, size * sizeof(int), cudaMemcpyHostToDevice);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }

    // Launch a kernel on the GPU with one thread for each element.
    // 在CUDA 中执行函数 语法：函数名称<<<block 数目, thread 数目, shared memory 大小>>>(参数...);
    addKernel<<<1, size>>>(dev_c, dev_a, dev_b, time);

    // Check for any errors launching the kernel
    cudaStatus = cudaGetLastError();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "addKernel launch failed: %s\n", cudaGetErrorString(cudaStatus));
        goto Error;
    }
    
    // cudaDeviceSynchronize waits for the kernel to finish, and returns
    // any errors encountered during the launch.
    cudaStatus = cudaDeviceSynchronize();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaDeviceSynchronize returned error code %d after launching addKernel!\n", cudaStatus);
        goto Error;
    }

    // Copy output vector from GPU buffer to host memory.
    cudaStatus = cudaMemcpy(c, dev_c, size * sizeof(int), cudaMemcpyDeviceToHost);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }

    cudaStatus = cudaMemcpy(&time_used, time, sizeof(clock_t), cudaMemcpyDeviceToHost);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }
    printf("GPU time: %ld\n", time_used);

Error:
    cudaFree(dev_c);
    cudaFree(dev_a);
    cudaFree(dev_b);
    
    return cudaStatus;
}


