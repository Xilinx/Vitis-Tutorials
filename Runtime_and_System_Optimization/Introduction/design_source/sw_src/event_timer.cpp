/**********
Copyright (c) 2019, Xilinx, Inc.
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors
may be used to endorse or promote products derived from this software
without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**********/


#include "event_timer.hpp"

#include <iomanip>
#include <iostream>

EventTimer::EventTimer()
{
    unfinished        = false;
    event_count       = 0;
    max_string_length = 0;
}

float EventTimer::ms_difference(EventTimer::timepoint start,
                                EventTimer::timepoint end)
{
    std::chrono::duration<float, std::milli> duration = end - start;
    return duration.count();
}

int EventTimer::add(std::string description)
{
    // If previously pending event was unfinished, adding a new event
    // will terminate it if this function is called
    if (unfinished)
        finish();

    unfinished = true;

    event_names.push_back(description);
    int length = description.length();
    if (length > max_string_length)
        max_string_length = length;
    start_times.push_back(std::chrono::high_resolution_clock::now());
    return event_count++;
}

void EventTimer::finish(void)
{
    end_times.push_back(std::chrono::high_resolution_clock::now());
    if (!unfinished) {
        end_times.pop_back();
        return;
    }
    unfinished = false;
}

void EventTimer::clear(void)
{
    start_times.clear();
    end_times.clear();
    event_names.clear();
    event_count = 0;
    unfinished  = false;
}

void EventTimer::print(int id)
{
    std::ios_base::fmtflags flags(std::cout.flags());
    if (id >= 0) {
        if ((unsigned)id > event_names.size())
            return;
        std::cout << event_names[id] << " : " << std::fixed << std::setprecision(3)
                  << ms_difference(start_times[id], end_times[id]) << std::endl;
    }
    else {
        int printable_events = unfinished ? event_count - 1 : event_count;
        for (int i = 0; i < printable_events; i++) {
            std::cout << std::left << std::setw(max_string_length) << event_names[i] << " : ";
            std::cout << std::right << std::setw(8) << std::fixed << std::setprecision(3)
                      << ms_difference(start_times[i], end_times[i]) << " ms"
                      << std::endl;
        }
    }
    std::cout.flags(flags);
}
