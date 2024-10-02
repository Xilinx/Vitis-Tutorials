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

#include "server.hpp"
#include "serial_processor.hpp"

void Server::start()
{
    std::cout << "Starting TCP server on port " << port_ << std::endl;

    // Open the acceptor socket
    acceptor_.open(endpoint_.protocol());

    // Bind the acceptor socket to the endpoint
    acceptor_.bind(endpoint_);

    // Listen for incoming connection requests
    acceptor_.listen(boost::asio::socket_base::max_connections);

    start_accept();
}

void Server::start_accept()
{
    std::cout << "Accepting connections" << std::endl;

    // The serial_processor owns the socket
    serial_processor* sp = new serial_processor(io_context_, &data_callbacks_);
    acceptor_.async_accept(sp->get_socket(),
        boost::bind(&Server::handle_accept, this, sp,
          boost::asio::placeholders::error));
}

void Server::handle_accept(serial_processor* sp, const boost::system::error_code& error)
{
    if (!error)
    {
      std::cout << "Accepted a connection" << std::endl;
      typedef boost::asio::detail::socket_option::boolean<SOL_SOCKET, SO_REUSEPORT> reuse_port;
      typedef boost::asio::detail::socket_option::boolean<SOL_SOCKET, SO_REUSEADDR> reuse_addr;
      sp->get_socket().set_option(reuse_port(true));
      sp->get_socket().set_option(reuse_addr(true));
      sp->start();
    }
    else
    {
      delete sp;
      std::cout << "Deleted connection" << std::endl;
    }
}
