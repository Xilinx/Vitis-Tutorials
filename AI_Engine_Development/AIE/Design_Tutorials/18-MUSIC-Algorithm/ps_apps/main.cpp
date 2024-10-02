//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Vlad Druz
//

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <fstream>
#include <unistd.h>
#include <byteswap.h>
#include <assert.h>

#include <boost/bind/bind.hpp>
#include <boost/asio.hpp>
#include "server.hpp"
#include "serial_processor.hpp"

#include "experimental/xrt_kernel.h"
#include "experimental/xrt_graph.h"
#include "xrt/xrt_aie.h"

#define INPUT_DATA_SIZE_PER_ITER 4096
#define OUTPUT_DATA_SIZE_PER_ITER 2304
#define DMA_WORD_BYTES 16 // 128-bit wide

#define INPUT_BUF_SIZE (INPUT_DATA_SIZE_PER_ITER * SERIAL_PROCESSOR_MAX_FRAMES)
#define OUTPUT_BUF_SIZE (OUTPUT_DATA_SIZE_PER_ITER * SERIAL_PROCESSOR_MAX_FRAMES)

typedef struct {
    const char *name;
    xrt::kernel kernel;
    xrt::run run;
    xrt::bo bo;
    uint32_t buffer_n_bytes;
    uint32_t word_n_bytes;
    uint8_t *bo_u8;
    uint32_t *bo_u32;
} pl_dma_t;

typedef struct {
    char *xclbin_file;
    xrt::device device;
    xrt::uuid xclbin_uuid;

    unsigned short server_port;

    pl_dma_t mm2s_1;
    pl_dma_t mm2s_2;
    pl_dma_t s2mm_1;

    xrt::graph aie_graph;
    xrt::aie::profiling aie_profiling;

    uint32_t input_data_bytes;
    uint32_t input_max_data_bytes;
} app_ctx_t;

static int pl_dma_init(app_ctx_t *app, const char *kernel_name,
    uint32_t buffer_n_bytes, uint32_t word_n_bytes, pl_dma_t *dma)
{
    dma->name = kernel_name;
    dma->buffer_n_bytes = buffer_n_bytes;
    dma->word_n_bytes = word_n_bytes;
    dma->kernel = xrt::kernel(app->device, app->xclbin_uuid, kernel_name);
    dma->bo = xrt::bo(app->device, buffer_n_bytes, dma->kernel.group_id(0));
    dma->bo_u32 = dma->bo.map<uint32_t*>();
    dma->bo_u8 = dma->bo.map<uint8_t*>();
    memset(dma->bo_u32, 0, dma->buffer_n_bytes);
    dma->bo.sync(XCL_BO_SYNC_BO_TO_DEVICE);

    printf("%s: %s buffer addr 0x%px\n", __FUNCTION__, kernel_name, dma->bo_u8);

    return 0;
}

int pl_dma_buffer_fill(pl_dma_t *dma, void *data, uint32_t n_bytes)
{
    if (n_bytes > dma->buffer_n_bytes)
    {
        printf("WARNING: dma %s, %d input bytes won't fit in the buffer, sending only %d\r\n",
            dma->name, n_bytes, dma->buffer_n_bytes);
        n_bytes = dma->buffer_n_bytes;
    }
    else if (n_bytes < dma->buffer_n_bytes)
    {
        printf("WARNING: dma %s, %d input bytes is not enough, padding with 00s\r\n",
            dma->name, n_bytes);
        memset(dma->bo_u32, 0, dma->buffer_n_bytes);
    }

    memcpy(dma->bo_u32, data, n_bytes);
    dma->bo.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    printf("Filled dma %s buffer, %d bytes\r\n", dma->name, n_bytes);

    return 0;
}

static void pl_dma_run(pl_dma_t *dma, uint32_t n_bytes)
{
    printf("Starting dma %s for %d bytes\r\n", dma->name, n_bytes);
    dma->run = dma->kernel(dma->bo, nullptr, n_bytes / dma->word_n_bytes);
}

static void pl_dma_wait(pl_dma_t *dma)
{
    printf("Waiting for dma %s\r\n", dma->name);
    dma->run.wait();
    dma->bo.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
}

static void aie_init(app_ctx_t *app, const char *graph_name)
{
    app->aie_graph = xrt::graph(app->device, app->xclbin_uuid, graph_name);
    app->aie_graph.reset();

    app->aie_profiling = xrt::aie::profiling(app->device);
}

static void music_input_reset(void *ctx)
{
    app_ctx_t *app = (app_ctx_t *)ctx;

    app->input_data_bytes = 0;
}

static int music_input_append(const uint8_t *data_in0, const uint8_t *data_in1,
    uint32_t n_bytes, void *ctx)
{
    app_ctx_t *app = (app_ctx_t *)ctx;

    if ((app->input_data_bytes + n_bytes) > app->input_max_data_bytes)
        return -1;

    memcpy(app->mm2s_1.bo_u8 + app->input_data_bytes, data_in0, n_bytes);
    app->mm2s_1.bo.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    memcpy(app->mm2s_2.bo_u8 + app->input_data_bytes, data_in1, n_bytes);
    app->mm2s_2.bo.sync(XCL_BO_SYNC_BO_TO_DEVICE);

    app->input_data_bytes += n_bytes;

    return 0;
}

static void music_data_process(uint8_t **out_data, uint32_t *out_data_bytes,
    float *out_iter_avg_ns, void *ctx)
{
    app_ctx_t *app = (app_ctx_t *)ctx;
    long long cycle_count;
    float aie_run_ns_avg;
    uint32_t n_iters = app->input_data_bytes / INPUT_DATA_SIZE_PER_ITER;
    uint32_t n_output_bytes = n_iters * OUTPUT_DATA_SIZE_PER_ITER;

    printf("%s: enter - %d x 2 input bytes, %d output bytes, %d graph iterations\r\n",
        __FUNCTION__, app->input_data_bytes, n_output_bytes, n_iters);

    app->aie_profiling.start(
        xrt::aie::profiling::profiling_option::io_stream_start_to_bytes_transferred_cycles,
        "aie_dut.sig_o", "", n_output_bytes);

    app->aie_graph.run(n_iters);

    pl_dma_run(&app->s2mm_1, n_output_bytes);
    pl_dma_run(&app->mm2s_2, app->input_data_bytes);
    pl_dma_run(&app->mm2s_1, app->input_data_bytes);

    app->aie_graph.wait(0);

    pl_dma_wait(&app->mm2s_1);
    pl_dma_wait(&app->mm2s_2);
    pl_dma_wait(&app->s2mm_1);

    cycle_count = app->aie_profiling.read(); // 1.25 GHz clocks
    app->aie_profiling.stop();

    aie_run_ns_avg = (float)cycle_count * 0.8; // ns

    if (n_iters > 1)
        aie_run_ns_avg /= (n_iters - 1);

    printf("%s: done, cycle count %lld, aie iteration avg %f ns\r\n", __FUNCTION__,
        cycle_count, aie_run_ns_avg);

    *out_data = app->s2mm_1.bo_u8;
    *out_data_bytes = n_output_bytes;
    *out_iter_avg_ns = aie_run_ns_avg;
}

int main(int argc, char** argv)
{
    int opt;
    app_ctx_t *app = (app_ctx_t *)malloc(sizeof(app_ctx_t));
    boost::asio::io_context io_context;

    while ((opt = getopt(argc, argv, ":p:c:")) != -1)
    {
        switch(opt)
        {
            case 'c':
                app->xclbin_file = optarg;
                break;
            case 'p':
                app->server_port = atoi(optarg);
                break;
            case '?':
                printf("unknown option: %c\n", optopt);
            break;
        }
    }

    if (!app->server_port || !app->xclbin_file)
        throw std::runtime_error("usage: -c container.xclbin -p port_number");

    app->device = xrt::device(0);
    
    if (app->device == nullptr)
		throw std::runtime_error("No valid device handle found. Make sure using right xclOpen index.");
    
    app->xclbin_uuid = app->device.load_xclbin(app->xclbin_file);

    pl_dma_init(app, "mm2s:{mm2s_1}", INPUT_BUF_SIZE, 
        DMA_WORD_BYTES, &app->mm2s_1);
    pl_dma_init(app, "mm2s:{mm2s_2}", INPUT_BUF_SIZE, 
        DMA_WORD_BYTES, &app->mm2s_2);
    pl_dma_init(app, "s2mm", OUTPUT_BUF_SIZE, 
        DMA_WORD_BYTES, &app->s2mm_1);

    aie_init(app, "aie_dut");

    app->input_max_data_bytes = INPUT_BUF_SIZE;

    // TCP server
    data_callbacks_t callbacks = {
        .reset_f = music_input_reset,
        .append_f = music_input_append,
        .process_f = music_data_process,
        .ctx = app,
    };

    Server server(io_context, app->server_port, &callbacks);

    server.start();
    io_context.run();
 
    return 0;
}
