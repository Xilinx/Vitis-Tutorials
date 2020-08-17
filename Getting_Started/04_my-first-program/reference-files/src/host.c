/*******************************************************************************
** HOST Code
*******************************************************************************/

#include "host.h"

#include <iostream>

int main(int argc, char* argv[])
{

    cl_int err;                        
    cl_uint check_status = 0;
    const cl_uint DATA_SIZE = 4096; 

    cl_platform_id platform_id;        
    cl_device_id device_id;            
    cl_context context;                
    cl_command_queue commands;         
    cl_program program;                
    cl_kernel kernel_vector_add;       
 
   cl_mem buffer_in1;                 
   cl_mem buffer_in2;                  
   cl_mem buffer_output;               
   cl_mem pt[2];
 //      cl_mem* buffer_in[2];

    cl_uint* source_in1;               
    cl_uint* source_in2;
    cl_uint* source_sw_results;
    char cl_platform_vendor[1001];
  
   if (argc != 3) {
        printf("Usage: %s xclbin\n", argv[0]);
        return EXIT_FAILURE;
    
	}
	
   const char* target_device_name = argv[2];
    
    cl_platform_id platforms[16];       
    cl_uint platform_count;
    cl_uint platform_found = 0;


    cl_uint num_devices;
    cl_uint device_found = 0;
    cl_device_id devices[16];  
    char cl_device_name[1001];
    

    source_in1 = (cl_uint*)aligned_alloc(MEM_ALIGNMENT,MAX_LENGTH * sizeof(cl_uint*));
    source_in2 = (cl_uint*)aligned_alloc(MEM_ALIGNMENT,MAX_LENGTH * sizeof(cl_uint*));
    source_sw_results = (cl_uint*)aligned_alloc(MEM_ALIGNMENT,MAX_LENGTH * sizeof(cl_uint*));
 

   cl_uint* source_hw_results = (cl_uint*)aligned_alloc(MEM_ALIGNMENT,MAX_LENGTH * sizeof(cl_uint*)); 
  
   for(cl_uint i = 0; i < MAX_LENGTH; i++) {
        source_in1[i]  = rand() % DATA_SIZE;
            std::cout <<"input "<< source_in1[i]<<std::endl;
	source_in2[i]  =  rand() % DATA_SIZE ;
	source_sw_results[i] = source_in1[i] + source_in2[i];	
        source_hw_results[i] = 0; 
      }

   	// ------------------------------------------------------------------------------------
	// Step 1: Get All PLATFORMS, then search for Target_Platform_Vendor (CL_PLATFORM_VENDOR)
	// ------------------------------------------------------------------------------------
	
	// Get the number of platforms
	// ..................................................
	  
   err = clGetPlatformIDs(16, platforms, &platform_count);
    if (err != CL_SUCCESS) {
        printf("Error: Failed to find an OpenCL platform!\n");
        printf("Test failed\n");
        return EXIT_FAILURE;
    }

    printf("INFO: Found %d platforms\n", platform_count);
 
      // ....................................................................................
      // step 1:  Search for Platform (ex: Xilinx) using: CL_PLATFORM_VENDOR = Target_Platform_Vendor
      // Check if the current platform matches Target_Platform_Vendor
      // ....................................................................................
 	
    for (cl_uint iplat=0; iplat<platform_count; iplat++) {
        err = clGetPlatformInfo(platforms[iplat], CL_PLATFORM_VENDOR, 1000, (void *)cl_platform_vendor,NULL);
        if (err != CL_SUCCESS) {
            printf("Error: clGetPlatformInfo(CL_PLATFORM_VENDOR) failed!\n");
            printf("Test failed\n");
            return EXIT_FAILURE;
        }
        if (strcmp(cl_platform_vendor, "Xilinx") == 0) {
            printf("INFO: Selected platform %d from %s\n", iplat, cl_platform_vendor);
            platform_id = platforms[iplat];
            platform_found = 1;
        }
    }
    if (!platform_found) {
        printf("ERROR: Platform Xilinx not found. Exit.\n");
        return EXIT_FAILURE;
    }
       // ------------------------------------------------------------------------------------  
       // Step 1:  Get All Devices for selected platform Target_Platform_ID
       //            then search for Xilinx platform (CL_DEVICE_TYPE_ACCELERATOR = Target_Device_Name)
	// ------------------------------------------------------------------------------------

	
   err = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_ACCELERATOR, 16, devices, &num_devices);
    printf("INFO: Found %d devices\n", num_devices);
    if (err != CL_SUCCESS) {
        printf("ERROR: Failed to create a device group!\n");
        printf("ERROR: Test failed\n");
        return -1;
    }
     // ------------------------------------------------------------------------------------  
     // Step 1:  Search for CL_DEVICE_NAME = Target_Device_Name
     // ............................................................................
	
   for (cl_uint i=0; i<num_devices; i++) {
        err = clGetDeviceInfo(devices[i], CL_DEVICE_NAME, 1024, cl_device_name, 0);
        if (err != CL_SUCCESS) {
            printf("Error: Failed to get device name for device %d!\n", i);
            printf("Test failed\n");
            return EXIT_FAILURE;
        }
        printf("CL_DEVICE_NAME %s\n", cl_device_name);

       // ............................................................................  
      // Step 1: Check if the current device matches Target_Device_Name
      // ............................................................................ 

       if(strcmp(cl_device_name, target_device_name) == 0) {
            device_id = devices[i];
            device_found = 1;
            printf("Selected %s as the target device\n", cl_device_name);
       }
    }
	// ------------------------------------------------------------------------------------
	// Step 1: Create Context
	// ------------------------------------------------------------------------------------
	
    context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
    if (!context) {
        printf("Error: Failed to create a compute context!\n");
        printf("Test failed\n");
        return EXIT_FAILURE;
    }
        // ------------------------------------------------------------------------------------
	// Step 1: Create Command Queue
	// ------------------------------------------------------------------------------------
    commands = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, &err);
    if (!commands) {
        printf("Error: Failed to create a command commands!\n");
        printf("Error: code %i\n",err);
        printf("Test failed\n");
        return EXIT_FAILURE;
    }

    cl_int status;

   unsigned char *kernelbinary;
    char *xclbin = argv[1];
	// ------------------------------------------------------------------
	// Step 1: Load Binary File from a disk to Memory
	// ------------------------------------------------------------------
	
   printf("INFO: loading xclbin %s\n", xclbin);
    cl_uint n_i0 = load_file_to_memory(xclbin, (char **) &kernelbinary);
    if (n_i0 < 0) {
        printf("failed to load kernel from xclbin: %s\n", xclbin);
        printf("Test failed\n");
        return EXIT_FAILURE;
    }

    size_t n0 = n_i0;
	
	// ------------------------------------------------------------
	// Step 1: Create a program using a Binary File
	// ------------------------------------------------------------
	
   program = clCreateProgramWithBinary(context, 1, &device_id, &n0,
                                        (const unsigned char **) &kernelbinary, &status, &err);
    free(kernelbinary);
	
	// ============================================================================
	// Step 2: Create Program and Kernels
	// ============================================================================
	//   o) Build a Program from a Binary File
	//   o) Create Kernels
	// ============================================================================
	


    if ((!program) || (err!=CL_SUCCESS)) {
        printf("Error: Failed to create compute program from binary %d!\n", err);
        printf("Test failed\n");
        return EXIT_FAILURE;
    }

	// -------------------------------------------------------------
	// Step 2: Build (compiles and links) a program executable from binary
	// -------------------------------------------------------------
	
    err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    if (err != CL_SUCCESS) {
        size_t len;
        char buffer[2048];

        printf("Error: Failed to build program executable!\n");
        clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
        printf("%s\n", buffer);
        printf("Test failed\n");
        return EXIT_FAILURE;
    }
	// -------------------------------------------------------------
	// Step 2: Create Kernels
	// -------------------------------------------------------------
	
   kernel_vector_add = clCreateKernel(program, "vadd", &err);
    if (!kernel_vector_add || err != CL_SUCCESS) {
        printf("Error: Failed to create compute kernel_vector_add!\n");
        printf("Test failed\n");
        return EXIT_FAILURE;
    }
	// ================================================================
	// Step 2: Setup Buffers and un Kernels
	// ================================================================
	//   o) Allocate Memory to store the results 
	//   o) Create Buffers in Global Memory to store data
	// ================================================================
	
	// ------------------------------------------------------------------
	// Step 2: Create Buffers in Global Memory to store data
	//             o) buffer_in1 - stores source_in1
	//             o) buffer_in2 - stores source_in2
	//             o) buffer_ouput - stores Results
	// ------------------------------------------------------------------
	
	// .......................................................
	// Allocate Global Memory for source_in1
	// .......................................................
	
   buffer_in1 = clCreateBuffer(context,  CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR,  sizeof(cl_uint) * DATA_SIZE, source_in1, &err);
    if (err != CL_SUCCESS) {
      std::cout << "Return code for clCreateBuffer - in1" << err << std::endl;
    }
	// .......................................................
	// Allocate Global Memory for source_in2
	// .......................................................
	
   buffer_in2 = clCreateBuffer(context,  CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR,  sizeof(cl_uint) * DATA_SIZE, source_in2, &err);
    if (err != CL_SUCCESS) {
      std::cout << "Return code for clCreateBuffer -in2" << err << std::endl;
    }
	// .......................................................
	// Allocate Global Memory for sourcce_hw_results
	// .......................................................
	
   buffer_output = clCreateBuffer(context,  CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR,  sizeof(cl_uint) * DATA_SIZE,source_hw_results , &err);
    if (err != CL_SUCCESS) {
      std::cout << "Return code for clCreateBuffer -ouput" << err << std::endl;
    }


    if (!(buffer_in1&&buffer_in2&&buffer_output)) {
        printf("Error: Failed to allocate device memory!\n");
        printf("Test failed\n");
        return EXIT_FAILURE;
    }
 	// ------------------------------------------------------
	// Step 2: Copy Input data from Host to Global Memory
	// ------------------------------------------------------
	

    

    pt[0] = buffer_in1;
    pt[1] = buffer_in2;
    pt[2] = buffer_output;

err = clEnqueueMigrateMemObjects(commands,(cl_uint)2,pt, 0 ,0,NULL, NULL);

// ============================================================================
	// Step 2: Set Kernel Arguments and Run the Application
	//         o) Set Kernel Arguments
	// 				----------------------------------------------------
	// 				 Kernel	Argument Nb    Description
	// 				----------------------------------------------------
	//				        in1   (input)     --> Input Vector1
        //					in2   (input)     --> Input Vector2
        //					out   (output)    --> Output Vector
        //					size  (input)     --> Size of Vector in Integer
	//         o) Copy Input Data from Host to Global Memory
	//         o) Submit Kernels for Execution
	//         o) Copy Results from Global Memory to Host
	// ============================================================================


    err = 0;
    cl_int size = DATA_SIZE;
    err |= clSetKernelArg(kernel_vector_add, 0, sizeof(cl_mem), &buffer_in1); 
    err |= clSetKernelArg(kernel_vector_add, 1, sizeof(cl_mem), &buffer_in2); 
    err |= clSetKernelArg(kernel_vector_add, 2, sizeof(cl_mem), &buffer_output); 
    err |= clSetKernelArg(kernel_vector_add, 3, sizeof(cl_int), &size); 
    
    if (err != CL_SUCCESS) {
        printf("Error: Failed to set kernel_vector_add arguments! %d\n", err);
        printf("Test failed\n");
 
 	}
 	
	// ----------------------------------------
	// Step 2: Submit Kernels for Execution
	// ----------------------------------------
	
   err = clEnqueueTask(commands, kernel_vector_add, 0, NULL, NULL);
    if (err) {
            printf("Error: Failed to execute kernel! %d\n", err);
            printf("Test failed\n");
            return EXIT_FAILURE;
        }

    err = 0;
	// --------------------------------------------------
	// Step 2: Copy Results from Global Memory to Host
	// --------------------------------------------------
	
 //   err |= clEnqueueReadBuffer( commands,buffer_output, CL_TRUE, 0, sizeof(cl_uint) * DATA_SIZE, source_hw_results, 0, NULL, NULL );
    err |= clEnqueueMigrateMemObjects(commands,(cl_uint)1,&pt[2], CL_MIGRATE_MEM_OBJECT_HOST,0,NULL, NULL);



   if (err != CL_SUCCESS) {
        printf("Error: Failed to write to source array: %d!\n", err);
        printf("Test failed\n");
        return EXIT_FAILURE;
    }

    clFinish(commands);

	// ============================================================================
	// Step 2: Processing Output Results
	//       o) Check correctness of the output results
	// ============================================================================
	
 
     for (cl_uint i = 0; i < DATA_SIZE; i++) {
        if ((source_hw_results[i] != source_sw_results[i])) {
            printf("\n Expected %d",source_hw_results[i] );
            printf("-----  Got %d\n",source_sw_results[i]);
            printf(" Error: Reuslt mismatch" );
            check_status = 1;
            break;
        }
   }

	// ============================================================================
	// Step 3: Release Allocated Resources
	// ============================================================================

    clReleaseMemObject(buffer_in1);
    clReleaseMemObject(buffer_in2);
    clReleaseMemObject(buffer_output);

    free(source_in1);
    free(source_in2);
    free(source_sw_results);
    free(source_hw_results);

    clReleaseProgram(program);
    clReleaseKernel(kernel_vector_add);
    clReleaseCommandQueue(commands);
    clReleaseContext(context);

    if (check_status) {
        printf("INFO: Test failed\n");
        return EXIT_FAILURE;
    } else {
        printf("INFO: Test completed successfully.\n");
        return EXIT_SUCCESS;
    }


} 
