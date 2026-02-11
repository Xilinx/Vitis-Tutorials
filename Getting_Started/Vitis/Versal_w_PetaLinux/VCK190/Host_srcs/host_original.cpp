#include <iostream>
#include <vector>
// #include <xrt/xrt_device.h>
// #include <xrt/xrt_kernel.h>
#include <xrt/xrt_bo.h>
//#include <xrt/xrt_graph.h>
#include <fstream>
#include <cstring>
#include "experimental/xrt_kernel.h"
#include "experimental/xrt_graph.h"
#define DATA_SIZE 32 // Number of elements for each input

int main() {
    try {
        // Open the AMD device 
        auto device = xrt::device(0);
        std::cout << "Opened device: " << device.get_info<xrt::info::device::name>() << std::endl;

        // Load the xclbin containing the design
        std::string xclbin_path = "a.xclbin";
        auto xclbin = xrt::xclbin(xclbin_path);
        auto uuid = device.load_xclbin(xclbin);
        std::cout << "Loaded xclbin: " << xclbin_path << std::endl;

        // Allocate output buffer in DDR (S2MM output)
        xrt::bo output_bo = xrt::bo(device, DATA_SIZE * sizeof(int), 0);

        // Map the output buffer for host access
        auto output_ptr = output_bo.map<int*>();

        // Prepare input data for mm2s_1 and mm2s_2
        std::vector<int> input_data_1(DATA_SIZE);
        std::vector<int> input_data_2(DATA_SIZE);

        for (int i = 0; i < DATA_SIZE; i++) {
            input_data_1[i] = i;            //  input for mm2s_1
            input_data_2[i] = DATA_SIZE - i; //  input for mm2s_2
        }

        std::cout << "Prepared input data in A72." << std::endl;

        // Allocate input buffers on the device for mm2s_1 and mm2s_2
	auto input_bo_1 = xrt::bo(device, DATA_SIZE * sizeof(int), XCL_BO_FLAGS_DEV_ONLY);
	auto input_bo_2 = xrt::bo(device, DATA_SIZE * sizeof(int), XCL_BO_FLAGS_DEV_ONLY);

	// Copy the input data to the device buffers
        auto bo_mapped1 = input_bo_1.map<int*>();
	auto bo_mapped2 = input_bo_2.map<int*>();
	memcpy(bo_mapped1, input_data_1.data(), DATA_SIZE * sizeof(int));
        memcpy(bo_mapped2, input_data_2.data(), DATA_SIZE * sizeof(int));

        // Open the AI Engine graph
        auto graph = xrt::graph(device, uuid, "mygraph");
        std::cout << "AI Engine graph opened." << std::endl;

        // Open the MM2S kernels
        auto mm2s_1_kernel = xrt::kernel(device, uuid, "mm2s"); // First input stream
        auto mm2s_2_kernel = xrt::kernel(device, uuid, "mm2s"); // Second input stream

        // Open the S2MM kernel
        auto s2mm_kernel = xrt::kernel(device, uuid, "s2mm");
        std::cout << "MM2S and S2MM kernels opened." << std::endl;

        // Configure and start the S2MM kernel (collect output from AI Engine)
        auto s2mm_run = xrt::run(s2mm_kernel);
        s2mm_run.set_arg(0, output_bo);  // Output buffer in DDR
        s2mm_run.set_arg(2, DATA_SIZE); // Number of elements
        s2mm_run.start();
        std::cout << "S2MM kernel started." << std::endl;

        // Configure and start the AI Engine graph
        graph.run(1); // Process one batch
        std::cout << "AI Engine graph started." << std::endl;

        // Start MM2S_2 kernel (stream data to AI Engine)
        auto mm2s_2_run = xrt::run(mm2s_2_kernel);
        mm2s_2_run.set_arg(0, input_bo_2); // Input data for mm2s_2
        mm2s_2_run.set_arg(2, DATA_SIZE);  // Number of elements
        mm2s_2_run.start();
        std::cout << "MM2S_2 kernel started." << std::endl;

        // Start MM2S_1 kernel (stream data to AI Engine)
        auto mm2s_1_run = xrt::run(mm2s_1_kernel);
        mm2s_1_run.set_arg(0, input_bo_1); // Input data for mm2s_1
        mm2s_1_run.set_arg(2, DATA_SIZE);  // Number of elements
        mm2s_1_run.start();
        std::cout << "MM2S_1 kernel started." << std::endl;

        // Wait for all kernels and graph to complete
        s2mm_run.wait();
        std::cout << "S2MM Wait DONE" << std::endl;
        graph.wait();
        std::cout << "AI Kernel Wait DONE" << std::endl;
        mm2s_2_run.wait();
        std::cout << "MM2s_2 Wait DONE" << std::endl;
        mm2s_1_run.wait();
        std::cout << "MM2s Wait DONE" << std::endl;

        std::cout << "Data transfer and processing completed." << std::endl;

        // Synchronize output buffer from the device
        output_bo.sync(XCL_BO_SYNC_BO_FROM_DEVICE);

        // Validate output
        bool success = true;
        for (int i = 0; i < DATA_SIZE; i++) {
            // Assuming AI Engine combines inputs by summing them (example operation)
            if (output_ptr[i] != input_data_1[i] + input_data_2[i]) {
                std::cout << "Mismatch at index " << i << ": Expected "
                          << input_data_1[i] + input_data_2[i] << ", Got " << output_ptr[i] << std::endl;
                success = false;
                break;
            }
        }

        if (success) {
            std::cout << "Test PASSED: Output data matches expected results." << std::endl;
        } else {
            std::cout << "Test FAILED: Output data does not match expected results." << std::endl;
        }

        // Clean up
        graph.end();
        std::cout << "Graph stopped and resources released." << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

