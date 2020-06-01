#include "V4l2Access.h"

V4l2Access::V4l2Access(V4l2Device* device) : m_device(device) {
}

V4l2Access::~V4l2Access() { 
	delete m_device; 
}

