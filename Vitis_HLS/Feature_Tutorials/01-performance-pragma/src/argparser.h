/*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
*/

#pragma once

#include <algorithm>
#include <string>
#include <vector>
#include <cstdint>

class ArgParser {
   public:
    ArgParser(int& argc, const char** argv) {
        for (int i = 1; i < argc; ++i) mTokens.push_back(std::string(argv[i]));
    }

#define INPLACE_GET_OPTION(VAR_TYPE, CONV_OP)                                                \
    bool getOption(const std::string option, VAR_TYPE& value, const std::string info = "") { \
        std::vector<std::string>::const_iterator itr;                                        \
        mOptions.push_back(option);                                                          \
        mInfos.push_back(info);                                                              \
        itr = std::find(this->mTokens.begin(), this->mTokens.end(), option);                 \
        if (itr != this->mTokens.end() && ++itr != this->mTokens.end()) {                    \
            std::string tmp = *itr;                                                          \
            value = CONV_OP;                                                                 \
            return true;                                                                     \
        }                                                                                    \
        return false;                                                                        \
    }

    INPLACE_GET_OPTION(uint64_t,     atoi(tmp.c_str()));
    INPLACE_GET_OPTION(uint32_t,     atoi(tmp.c_str()));
    INPLACE_GET_OPTION(uint16_t,     atoi(tmp.c_str()));
    INPLACE_GET_OPTION(uint8_t,      atoi(tmp.c_str()));
    INPLACE_GET_OPTION(int64_t,      atoi(tmp.c_str()));
    INPLACE_GET_OPTION(int32_t,      atoi(tmp.c_str()));
    INPLACE_GET_OPTION(int16_t,      atoi(tmp.c_str()));
    INPLACE_GET_OPTION(int8_t,       atoi(tmp.c_str()));
    INPLACE_GET_OPTION(double,       atof(tmp.c_str()));
    INPLACE_GET_OPTION(float,        atof(tmp.c_str()));
    INPLACE_GET_OPTION(std::string,  tmp);

    bool getOption(const std::string option, bool& value, const std::string info  = "")  {
        std::vector<std::string>::const_iterator itr;
        mOptions.push_back(option);
        mInfos.push_back(info);       
        itr = std::find(this->mTokens.begin(), this->mTokens.end(), option);
        if (itr != this->mTokens.end()) {
            value = true;
            return true;
        }
        value = false;
        return false;
    }

    void printHelp() {
        printf("Usage :\n");
        for (auto i=0; i<mOptions.size(); i++) {
            printf("  %s  : %s\n", mOptions[i].c_str(), mInfos[i].c_str());
        }
    }


   private:
    std::vector<std::string> mTokens;
    std::vector<std::string> mOptions;
    std::vector<std::string> mInfos;

};