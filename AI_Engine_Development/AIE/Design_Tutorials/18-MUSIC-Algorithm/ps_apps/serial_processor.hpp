//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Vlad Druz
//

#ifndef SERIAL_PROCESSOR_HPP__
#define SERIAL_PROCESSOR_HPP__

#include <cstdint>
#include <cstring>
#include <thread>
#include <semaphore>
#include <boost/asio.hpp>
#include "circular_buffer.hpp"
#include "serial_frame.hpp"

#define SERIAL_PROCESSOR_BUF_SIZE (100000)
#define RD_BUFFER_LEN             (2000)
#define SERIAL_PROCESSOR_MAX_FRAMES (512)

typedef void (*input_reset_f)(void *ctx);
typedef int (*input_append_f)(const uint8_t *data_in0, const uint8_t *data_in1,
    uint32_t n_bytes, void *ctx);
typedef void (*data_process_f)(uint8_t **out_data, uint32_t *out_data_bytes,
    float *out_iter_avg_ns, void *ctx);

typedef struct {
    input_reset_f reset_f;
    input_append_f append_f;
    data_process_f process_f;
    void *ctx;
} data_callbacks_t;

class serial_processor
{
public:
    void process_frame(uint8_t *buf, size_t len);
    serial_processor(boost::asio::io_context& io_context, data_callbacks_t *callbacks);
    void start();
    boost::asio::ip::tcp::socket& get_socket();
private:
    void handle_read (const boost::system::error_code& error, size_t bytes_transferred);
    void send_response(serial_frame *request_frame, uint8_t *payload, size_t payload_len,
        float calc_avg_ns);
    bool frame_recvd();
    void process_snapshots();
    
    // Rx buffer storage
    circular_buffer cb;

    // Storage for a complete frame
    uint8_t  rx_frm [SERIAL_PROCESSOR_BUF_SIZE];

    // read buffer
    uint8_t  rd_buf[RD_BUFFER_LEN];

    // Server socket
    boost::asio::ip::tcp::socket m_socket;

    // Payload processing functions
    data_callbacks_t m_data_callbacks;

    serial_frame m_frame_headers[SERIAL_PROCESSOR_MAX_FRAMES];
    uint32_t m_num_frame_headers;

    std::thread *m_thread;
    bool m_thread_keep_running;
};

#endif /* SERIAL_PROCESSOR_HPP__ */
