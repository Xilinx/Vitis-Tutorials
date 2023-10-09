/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

#pragma once
#include "xtlm_ipc.h"
#include <thread>

using b_targ_socket = xtlm_ipc::axis_target_socket_util<xtlm_ipc::BLOCKING>;

class s2mm
{
	std::thread m_thread;
	std::unique_ptr<b_targ_socket> m_sock_ptr;
	int count;

	void sock_data_handler()
	{
		m_sock_ptr = std::make_unique<b_targ_socket>(m_sock_name);
		try
		{
			while (count<1024)
			{
				std::vector<char> vec;
				m_sock_ptr->sample_transaction(vec);
				recv_data(vec);
				count+=vec.size()/sizeof(int);
				if(count%128==0)
					std::cout << m_sock_name << "  Receiving  " << count << std::endl;
			}
		}
		catch (std::exception &ex)
		{
			std::cout << m_sock_name << " caught exception. " << ex.what()
			<< std::endl;
		}
	}
protected:
	const std::string m_sock_name;
public:
	s2mm(const std::string sock_name) :
	m_sock_name(sock_name), m_sock_ptr(nullptr), count(0)
	{
	}

	virtual void recv_data(const std::vector<char>& data) = 0;

	void run()
	{
		m_thread = std::thread(&s2mm::sock_data_handler, this);
	}

	int dataTransferred()
	{
		return(count);
	}

	virtual ~s2mm()
	{
		std::cout << this->m_sock_name << " before join " << std::endl;
		if(m_thread.joinable())
		m_thread.join();
		std::cout << this->m_sock_name << " after join " << std::endl;
	}
};
