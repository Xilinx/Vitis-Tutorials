/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

#pragma once
#include "xtlm_ipc.h"
#include <thread>

using b_targ_socket = xtlm_ipc::axis_target_socket_util<xtlm_ipc::BLOCKING>;
using b_init_socket = xtlm_ipc::axis_initiator_socket_util<xtlm_ipc::BLOCKING>;

class mm2s
{
	std::thread m_thread;
	std::unique_ptr<b_init_socket> m_socket_ptr;
	int count;
	void sock_data_handler()
	{
		m_socket_ptr = std::make_unique<b_init_socket>(m_sock_name);
		std::vector<char> data_to_send;
		data_to_send = send_data();
		while (data_to_send.size() > 0)
		{
			m_socket_ptr->transport(data_to_send,count%128==127?true:false);  // transport(data, tlast), 128 sample frame
			count++;
			data_to_send = send_data();
		}
	}
protected :
	const std::string m_sock_name;
public:
	mm2s(const std::string sock_name) :
		m_sock_name(sock_name), m_socket_ptr(nullptr),count(0)
	{
	}

	virtual std::vector<char> send_data() = 0;

	void run()
	{
		m_thread = std::thread(&mm2s::sock_data_handler, this);
	}

	int dataTransferred()
		{
			return(count);
		}


	virtual ~mm2s()
	{
		std::cout << this->m_sock_name << " before join " << std::endl;
		if(m_thread.joinable())
			m_thread.join();
		std::cout << this->m_sock_name << " after join " << std::endl;
	}

};
