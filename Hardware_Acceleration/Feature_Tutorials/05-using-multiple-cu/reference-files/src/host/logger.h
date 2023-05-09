/*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
*/

#ifndef LOGGER_H_
#define LOGGER_H_

#include <iomanip>
#include <iostream>
#include <string>
#include <vector>


#define ENABLE_LOG_TOFILE 1
#define ENABLE_LOG_TIME 1

//global logging
#define LogInfo(desc, ...) sda::LogWrapper(0, __FILE__, __LINE__, desc, ##__VA_ARGS__)
#define LogWarn(desc, ...) sda::LogWrapper(1, __FILE__, __LINE__, desc, ##__VA_ARGS__)
#define LogError(desc, ...) sda::LogWrapper(2, __FILE__, __LINE__, desc, ##__VA_ARGS__)

using namespace std;

namespace sda {

	enum LOGTYPE {etInfo, etWarning, etError};

	//string
	string& ltrim(string& s);
	string& rtrim(string& s);
	string& trim(string& s);
	string GetFileExt(const string& s);
	string GetFileTitleOnly(const string& s);

	string ToLower(const string& s);
	string ToUpper(const string& s);

	//time
	string GetTimeStamp();

	//paths
	string GetApplicationPath();


	//debug
	template<typename T>
	void PrintPOD(const vector<T>& pod, size_t display_count = 0, const int precision = 4) {

		size_t count = pod.size();
		if(display_count > 0)
			count = std::min<size_t>(pod.size(), display_count);

		for(size_t i = 0; i < count; i++) {
			cout << std::setprecision(precision) << pod[i] << ", ";
		}
		cout << endl;
	}

	//logging
	void LogWrapper(int etype, const char* file, int line, const char* desc, ...);

}



#endif /* LOGGER_H_ */
