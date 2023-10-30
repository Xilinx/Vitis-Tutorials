/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

#pragma once
#include "xtlm_ipc.h"
#include <thread>
#include "pl_kernels.h"

using b_targ_socket = xtlm_ipc::axis_target_socket_util<xtlm_ipc::BLOCKING>;
using b_init_socket = xtlm_ipc::axis_initiator_socket_util<xtlm_ipc::BLOCKING>;

class polar_clip
{
	std::thread m_thread;
	int receive_data_count;
	int send_data_count;
	std::thread m_thread_1;
	std::unique_ptr<b_targ_socket> m_in_sock_ptr;
	std::unique_ptr<b_init_socket> m_out_sock_ptr;
	std::vector<char> vec_data;
	std::vector<char> processed_vec;
	std::queue<std::vector<char>> data_queue;

	void in_data_handler() {
		const int data_len = sizeof(int);
		int frame_len;
		while (receive_data_count < 1024) {
			m_in_sock_ptr->sample_transaction(vec_data);
			frame_len = vec_data.size()/data_len;

			if(receive_data_count%128==0)
			std::cout << "POLAR_CLIP:: Frame received (number of cint16) " << frame_len << std::endl;

			std::vector<char>::const_iterator begin = vec_data.begin();
			for(int i=0;i<frame_len;i++)
			{
				std::vector<char> data(begin+4*i,begin+4*(i+1));
				processed_vec = process_data(data);
				data_queue.push(processed_vec);
				receive_data_count++;
				if(receive_data_count%128==0)
					std::cout << "POLAR_CLIP:: in_data_handler overall number of data sent to processing " << receive_data_count << std::endl;
			}

		}
	}

	void out_data_handler() {
		int index = 0;
		while (1) {
			if (!data_queue.empty()) {
				send_data_count++;
				m_out_sock_ptr->transport(data_queue.front(),send_data_count%256==0?true:false); // 256 sample frame
				data_queue.pop();
				if(send_data_count%128==0)
					std::cout << "POLAR_CLIP:: out_data_handler number of data processed "<< send_data_count << std::endl;
			}
			if (send_data_count == 1024) {
				std::cout << " POLAR_CLIP:: All data sent " << std::endl;
				break;
			}
		}
	}
protected:
	const std::string m_in_sock_name;
	const std::string m_out_sock_name;
public:
	polar_clip(const std::string in_sock_name, const std::string out_sock_name)
	:receive_data_count(0), send_data_count(0), m_in_sock_name(in_sock_name), m_out_sock_name(out_sock_name) {

		m_in_sock_ptr = std::make_unique<b_targ_socket>(m_in_sock_name);
		sleep(4); // Work around before CR - https://jira.xilinx.com/browse/CR-1110519 gets fixed.
		m_out_sock_ptr = std::make_unique<b_init_socket>(m_out_sock_name);
	};

	virtual std::vector<char> process_data(
		const std::vector<char>& sampled_data) = 0;

		void run()
		{
			m_thread = std::thread(&polar_clip::in_data_handler, this);
			m_thread_1 = std::thread(&polar_clip::out_data_handler, this);
		}

		int dataTransferred()
		{
			return(send_data_count);
		}

		virtual ~polar_clip()
		{
			std::cout << this->m_in_sock_name << " polar clip before join " << std::endl;
			if(m_thread.joinable())
			m_thread.join();
			if(m_thread_1.joinable())
			m_thread_1.join();
			std::cout << this->m_in_sock_name << " polar clip after join " << std::endl;
		}
	};
