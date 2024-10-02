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

#include "serial_processor.hpp"

class Server 
{
public:
    Server(boost::asio::io_context& io_context, unsigned short port,
        data_callbacks_t *callbacks)
     : io_context_(io_context),
       endpoint_(boost::asio::ip::address_v4::any(), port),
       acceptor_(io_context_)
    {
        port_ = port;
        data_callbacks_ = *callbacks;
    }
    void start();
private:
    void start_accept();
    void handle_accept(serial_processor* sp, const boost::system::error_code& error);

    unsigned short port_;
    data_callbacks_t data_callbacks_;
    boost::asio::io_context& io_context_;
    boost::asio::ip::tcp::endpoint endpoint_;
    boost::asio::ip::tcp::acceptor acceptor_;
};
