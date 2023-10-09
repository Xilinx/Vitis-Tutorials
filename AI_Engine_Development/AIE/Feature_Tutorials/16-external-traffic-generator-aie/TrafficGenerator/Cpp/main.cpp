/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

#include "mm2s.hpp"
#include "polar_clip.hpp"
#include "s2mm.hpp"
#include <chrono>
#include <fstream>
#include <stdint.h>
#include <string>
#include "input.h"

class mm2s_impl : public mm2s
{
	unsigned int count;
	std::ofstream m_file;
public:
	mm2s_impl(std::string sock_name) :
			mm2s(sock_name), count(0), m_file(sock_name+".txt")
	{
	}
	virtual std::vector<char> send_data() override
	{
		if(count >= 1024) return {};
		unsigned int data_length = 4;

    char data[data_length];
    memcpy(&data[0],&cint16Input[count],2*sizeof(int16_t));

		std::vector<char> v_data;
    v_data.insert(v_data.end(), data, data + data_length);
    count = count + 2 ;

		m_file << this->m_sock_name << " : Sending data : ";

		for(int i=0; i<2 ; i++)
			m_file << std::to_string(*((int16_t *)(&v_data[2*i]))) << " " ;

		m_file << std::endl;
		m_file.flush();
		return v_data;
	}
};

class polar_clip_impl : public polar_clip
{
public:
	polar_clip_impl(const std::string in_sock, const std::string out_sock):
		polar_clip(in_sock, out_sock){}

	virtual std::vector<char> process_data(
			const std::vector<char> &sampled_data) override
	{
    int out_sample;
    process_polar_clip(*((int *)(&sampled_data[0])), out_sample);

    unsigned int data_len = 4;
    char data[data_len];
    memcpy(&data[0],&out_sample,sizeof(int));

		std::vector<char> processed_data;
    processed_data.insert(processed_data.end(), data, data + data_len);
		return processed_data;
	}
};


class s2mm_impl : public s2mm
{
	std::ofstream m_file;
	std::ofstream m_outputfile;
public:
	s2mm_impl(const std::string sock_name) :
			s2mm(sock_name), m_file(sock_name + ".txt"), m_outputfile("output.txt")
	{
	}
	virtual void recv_data(const std::vector<char>& data) override
	{
		// m_file << this->m_sock_name << " : Received data :: " ;

		for(int i=0;i<data.size();i+=4)
		{
			int k = data[i];
	    m_file << this->m_sock_name << " : Received data :: " << std::to_string(k) << " " << std::endl;
		}
		m_file.flush();

    m_outputfile << std::to_string(*((int *)(&data[0]))) << " ";
		m_outputfile<< std::endl;
		m_outputfile.flush();
	}
};

// int main_aiesim()
int run_etg()
{
	mm2s_impl 		chain_1_mm2s("DataIn1");
	polar_clip_impl chain_1_pc	("clip_in", "clip_out");
	s2mm_impl		chain_1_s2mm("DataOut1");

	using namespace std::chrono_literals;
	chain_1_mm2s.run();
	std::cout << "Started mm2s " << std::endl;
	std::this_thread::sleep_for(500ms);
	chain_1_pc.run();
	std::cout << "Started polar_clip " << std::endl;
	std::this_thread::sleep_for(400ms);
	chain_1_s2mm.run();
	std::cout << "Started s2mm " << std::endl;

	while(chain_1_s2mm.dataTransferred()!=1024)
	{
		std::this_thread::sleep_for(2s);
	}
}





int main(int argc, char *argv[])
{


			std::cout << "\n\n-----------------------------------\n\n Run Traffic Generator \n\n---------------------------------------\n\n" << std::endl;
			run_etg();

			std::cout << "\n\n-----------------------------------\n\n  Traffic Generator end \n\n---------------------------------------\n\n" << std::endl;
	return(0);
}
