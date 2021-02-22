/*

  Some source codes below are copied
https://github.com/xxradon/libv4l2_opencv_mat
https://github.com/mpromonet/libv4l2cpp

  They are modified.

This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

*/
#ifndef V4L2_OUTPUT
#define V4L2_OUTPUT

#include "V4l2Access.hpp"

// ---------------------------------
// V4L2 Output
// ---------------------------------
class V4l2Output : public V4l2Access
{		
	protected:
		V4l2Output(V4l2Device* device);

	public:
		static V4l2Output* create(const V4L2DeviceParameters & param, IoType iotype = V4l2Access::IOTYPE_MMAP);
		~V4l2Output();
	
		size_t write(char* buffer, size_t bufferSize);
		int    isWritable(timeval* tv);
		bool   startPartialWrite(void);
		size_t writePartial(char* buffer, size_t bufferSize);
		bool   endPartialWrite(void);
};

#endif

