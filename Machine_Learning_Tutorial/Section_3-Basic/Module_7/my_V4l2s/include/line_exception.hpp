/*
 * Copyright 2019-2020 Xilinx Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef LINE_EXCEPTION__
#define LINE_EXCEPTION__

#include <cerrno>
#include <cstring>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

class LineException : public std::runtime_error
{
    std::string message;
    int exception_errno;

public:
    LineException(const std::string &arg, int errno_in, const char *file, int line) : std::runtime_error(arg)
    {
        std::ostringstream output;
        const char *filename = NULL;
        filename             = strrchr(file, '/');

        output << "(";
        if (filename) {
            output << (filename + 1);
        }
        else {
            output << file;
        }
        output << ":" << line << ") " << arg;
        if (errno != 0) {
            output << std::endl
                   << "   (" << std::strerror(errno) << ")" << std::endl;
        }
        message         = output.str();
        exception_errno = errno_in;
    }
    ~LineException() throw() {}
    const char *what() const throw()
    {
        return message.c_str();
    }
    const int le_errno()
    {
        return exception_errno;
    }
};

#define throw_lineexception(arg) throw LineException(arg, 0, __FILE__, __LINE__);
#define throw_lineexception_errno(arg, errno_in) throw LineException(arg, errno_in, __FILE__, __LINE__);

#endif
