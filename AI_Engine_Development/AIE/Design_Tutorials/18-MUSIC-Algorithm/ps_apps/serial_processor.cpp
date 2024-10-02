//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Vlad Druz
//

#include <cstdlib>
#include <iostream>
#include <boost/bind/bind.hpp>
#include <boost/asio.hpp>

#include <cstdint>
#include <cstring>
#include <stdio.h>

#include "serial_processor.hpp"

#define SEND_RESPONSE_SLEEP_S 240

serial_processor::serial_processor(boost::asio::io_context& io_context,
    data_callbacks_t *callbacks)
    : m_socket(io_context)
{
    m_data_callbacks = *callbacks;

    memset(rx_frm,  0, sizeof(rx_frm ));
    memset(rd_buf,  0, sizeof(rd_buf ));
}

boost::asio::ip::tcp::socket& serial_processor::get_socket()
{
    return m_socket;
}

void serial_processor::start()
{
    // Reset the input data buffers
    m_data_callbacks.reset_f(m_data_callbacks.ctx);
    m_num_frame_headers = 0;
    m_thread_keep_running = false;
    
    // Collect a series of snapshots
    std::cout << "Waiting for snapshots.. " << std::endl;
            
    m_socket.async_read_some(boost::asio::buffer(rd_buf, RD_BUFFER_LEN),
        boost::bind(&serial_processor::handle_read, this,
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
}

void serial_processor::handle_read(const boost::system::error_code& error, size_t bytes_transferred)
{
    if (!error)
    {
        process_frame(rd_buf, bytes_transferred);

        m_socket.async_read_some(boost::asio::buffer(rd_buf, RD_BUFFER_LEN),
          boost::bind(&serial_processor::handle_read, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
    }
    else
    {
      std::cout << "handle_read: Delete connection" << std::endl;
      delete this;
    }
}

void serial_processor::process_snapshots()
{
    uint8_t *out_data;
    uint32_t i, out_data_bytes, out_data_per_frame;
    float out_iter_avg_ns;    

    std::cout << "Processing collected snapshots" << std::endl;

    // process all collected snapshots at once, collect output data
    m_data_callbacks.process_f(&out_data, &out_data_bytes, &out_iter_avg_ns,
        m_data_callbacks.ctx);

    out_data_per_frame = out_data_bytes / m_num_frame_headers;

    while (m_thread_keep_running)
    {
        // send all collected output data to the client
        for (i = 0; i < m_num_frame_headers; i++)
        {
            send_response(&m_frame_headers[i], 
                &out_data[out_data_per_frame * i], out_data_per_frame,
                out_iter_avg_ns);
        }

        // sleep for a fixed amount of time while client is processing data
        for (i = 0; i < SEND_RESPONSE_SLEEP_S; i++)
        {
            if (!m_thread_keep_running)
                break;
            
            sleep(1);
        }
    }

    m_num_frame_headers = 0;
    m_data_callbacks.reset_f(m_data_callbacks.ctx);
}

void serial_processor::process_frame(uint8_t *buf, size_t len)
{
    bool is_complete = false;

    cb.put(buf, len);

    while (frame_recvd())
    { // Received a complete input frame
        serial_frame *frm;
        size_t payload_len;

        // Terminate the data processing thread if it's running
        if (m_thread_keep_running)
        {
            m_thread_keep_running = false;
            m_thread->join();
            delete m_thread;
        }

        // Place the serial frame header in rx_frame
        cb.peek(rx_frm, sizeof(serial_frame));
        frm = (serial_frame *) rx_frm;
        payload_len = frm->payload_len();

        // Now that we have the header, use payload_len to place
        // the entire frame in rx_frm
        cb.get(rx_frm, sizeof(serial_frame) + payload_len);

        std::cout << "Got snapshot " << frm->sequence_num << ", type " <<
            frm->snapshot_type << " (len " << payload_len << ")" << std::endl;

        // Store its header because we'll need it when sending back the response
        m_frame_headers[m_num_frame_headers] = *frm;
        m_num_frame_headers++;

        // Push the frame payload into DMA buffers: half of data into each
        if (m_data_callbacks.append_f(frm->payload, &frm->payload[payload_len/2],
            payload_len / 2, m_data_callbacks.ctx))
        {
            std::cout << "DMA buffers full" << std::endl;
            is_complete = true;  
        }            
        else if (m_num_frame_headers == SERIAL_PROCESSOR_MAX_FRAMES)
        {
            std::cout << "Max number of frames reached" << std::endl;
            is_complete = true;              
        }
        else if (frm->snapshot_type == 5.0)
        {
            std::cout << "Unique snapshot" << std::endl;
            is_complete = true;
        }
        else if (frm->snapshot_type == 4.0)
        {
            std::cout << "Last snapshot" << std::endl;
            is_complete = true;
        }

        // Create the data processing thread
        if (is_complete)
        {
            m_thread_keep_running = true;
            m_thread = new std::thread(&serial_processor::process_snapshots, this);
        }
    }
}

bool serial_processor::frame_recvd()
{
    bool rval = true;

    if (cb.size() > sizeof(serial_frame))
    {
        serial_frame *frm;

        /*
         * Received data is enought for a full header.  Check for
         * a valid start-of-frame indicator. If we did not receive
         * a valid start-of-frame indicator,discard the data, and
         * wait for valid start-of-frame.
         */

        // Place the frame header in rx_frm 
        cb.peek(rx_frm, sizeof(serial_frame));

        // Check for valid start-of-frame
        frm = (serial_frame *) rx_frm;
        if (frm->is_valid())
        {
            rval = frm->is_complete(cb.size());
        }
        else
        {
	    std::cout << "Rx " << cb.size() << " bytes invalid frame data." << std::endl;
            cb.reset();
            rval = false;
        }
    }
    else if (cb.size() >= sizeof(uint32_t))
    {
        serial_frame *frm;

        /*
         * Received data is less than a full header,  but larger
         * than the start-of-frame indicator.  Check for a valid
         * start-of-frame indicator.     If we did not receive a 
         * valid start-of-frame indicator,  discard the data,and
         * wait for valid start-of-frame.
         */

        // Place the start of frame indicator in rx_frm 
        cb.peek(rx_frm, sizeof(uint32_t));

        // Check for valid start-of-frame 
        frm = (serial_frame *) rx_frm;
        if (frm->is_valid() == false)
        {
            cb.reset();
        }
	else
	{
	    std::cout << "Rx " << cb.size() << " bytes valid frame data." << std::endl;
	}

        rval = false;
    }
    else
    {
        /* 
         * Not enough bytes have been received for a 
         * complete frame
         */
        rval = false;
    }
    
    return rval;
}

void serial_processor::send_response(serial_frame *request_frame, uint8_t *payload, size_t payload_len,
    float calc_avg_ns)
{
    uint8_t hdr_bytes[sizeof(serial_frame)];
    serial_frame *hdr = (serial_frame *) hdr_bytes;
    
    memcpy(hdr, request_frame, sizeof(serial_frame));

    hdr->set_payload_len(payload_len);
    hdr->calc_avg_ns = calc_avg_ns;

    std::cout << "Sending response " << hdr->sequence_num << " (" << payload_len << ")" << std::endl;

    boost::system::error_code error;

    boost::asio::write(m_socket, boost::asio::buffer(hdr_bytes, sizeof(hdr_bytes)), error);

    if (!error)
        boost::asio::write(m_socket, boost::asio::buffer(payload, payload_len), error);

    if (error)
    {
        std::cout << "send_response: Delete connection 0" << std::endl;
        delete this;
    }
}
