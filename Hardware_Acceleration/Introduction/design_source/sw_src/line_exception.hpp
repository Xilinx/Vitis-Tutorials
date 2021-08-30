/*
 * Copyright (C) 2018 Xilinx, Inc.  All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * Use of the Software is limited solely to applications: (a) running on a
 * Xilinx device, or (b) that interact with a Xilinx device through a bus or
 * interconnect.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * XILINX BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of the Xilinx shall not be used
 * in advertising or otherwise to promote the sale, use or other dealings in
 * this Software without prior written authorization from Xilinx.
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
