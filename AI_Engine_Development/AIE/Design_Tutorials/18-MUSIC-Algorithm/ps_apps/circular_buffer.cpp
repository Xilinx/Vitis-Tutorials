//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Vlad Druz
//

#include <iostream>

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "circular_buffer.hpp"

bool circular_buffer::is_full(void)
{
    return full;
}

size_t circular_buffer::capacity()
{
    return (size_max - size());
}

size_t circular_buffer::size()
{
    size_t size;

    if (full)
    {
        size = size_max;
    }
    else
    {
        size = (head >= tail) ? (head - tail)
                              : (size_max - tail + head);
    }

    return size;
}

void circular_buffer::advance(size_t len)
{
    head = (head + len) % size_max;
    full = (head == tail);
}

void circular_buffer::retreat(size_t len)
{
    full = false;
    tail = (tail + len) % size_max;
}

void circular_buffer::reset()
{
    head = 0;
    tail = 0;
    full = false;
    memset(buf, 0, size_max);
}

int32_t circular_buffer::put(uint8_t *in, size_t len)
{
    int32_t r_val;

    if (len <= capacity())
    {
        if (head + len > size_max)
        {
            /*
             * The len parameter takes us past the end of the
             * array, so wrap-around.
             */
            size_t ar1_len = (size_max - head);
            size_t ar2_len = (len - ar1_len);
            memcpy(&buf[head], &in[0      ], ar1_len);
            memcpy(&buf[0   ], &in[ar1_len], ar2_len);
        }
        else
        {
            memcpy(&buf[head], &in[0      ], len);
        }

        advance(len);

        r_val =  0;
    }
    else
    {
        r_val = -1;
    }

    return r_val;
}

int32_t circular_buffer::get(uint8_t *out, size_t len)
{
    int32_t r_val;

    r_val = peek(out, len);
    if (r_val == 0)
    {
        retreat(len);
    }

    return r_val;
}

int32_t circular_buffer::peek(uint8_t *out, size_t len)
{
    int32_t r_val;

    if (len <= size())
    {
        if (tail + len > size_max)
        {
            size_t ar1_len = (size_max - tail);
            size_t ar2_len = (len - ar1_len);
            memcpy(&out[0      ], &buf[tail], ar1_len);
            memcpy(&out[ar1_len], &buf[0   ], ar2_len);
        }
        else
        {
            memcpy(&out[0      ], &buf[tail], len);
        }

        r_val = 0;
    }
    else
    {
        r_val = -1;
    }

    return r_val;
}
