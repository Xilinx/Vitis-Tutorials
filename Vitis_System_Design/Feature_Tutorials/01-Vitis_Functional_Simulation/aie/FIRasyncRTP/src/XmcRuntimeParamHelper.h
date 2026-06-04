//
// Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#ifndef __XMC_RUNTIME_PARAM_HELPER__
#define __XMC_RUNTIME_PARAM_HELPER__

#include "adf.h"

#include <type_traits>
#include <fstream>

template<class T, class Enable = void>
class XMCRuntimeParamHelper {
public:
   static inline bool getInput(const std::string &filename,
                               T *values,
                               unsigned size = 1)
   {
      std::ifstream file(filename);
      unsigned count = 0;
      while (file.is_open() && !file.eof() && count < size) {
         file >> values[count].real;
         if (!file.eof()) {
            file >> values[count++].imag;
         }
      }
      file.close();

      if (count < size) {
         return false;
      }
      return true;
   }
};

template<class T>
class XMCRuntimeParamHelper<T,
         typename std::enable_if<std::is_arithmetic<T>::value>::type> {
public:
   static inline bool getInput(const std::string &filename,
                               T *values,
                               unsigned size = 1)
   {
      std::ifstream file(filename);
      unsigned count = 0;
      while (file.is_open() && !file.eof() && count < size) {
	 if (std::is_unsigned<T>::value && sizeof(T) == 1) {
	    unsigned number;
	    file >> number;
	    values[count] = static_cast<T>(number);
	 } else if (std::is_signed<T>::value && sizeof(T) == 1) {
	    int number;
	    file >> number;
	    values[count] = static_cast<T>(number);
	 } else {
	    file >> values[count];
	 }

	 ++count;
      }
      file.close();

      if (count < size) {
         return false;
      }
      return true;
   }
};

#if __AIE_ARCH__==20
template<>
class XMCRuntimeParamHelper<bfloat16> {
public:
   static inline bool getInput(const std::string &filename,
                               bfloat16 *values,
                               unsigned size = 1)
   {
      std::ifstream file(filename);
      unsigned count = 0;
      while (file.is_open() && !file.eof() && count < size) {
         float data;
         file >> data;
         uint32_t hexValue = *reinterpret_cast<uint32_t *>(&data);
         values[count++].data = static_cast<uint16_t>(hexValue >> 16);
      }
      file.close();

      if (count < size) {
         return false;
      }
      return true;
   }
};
#endif

#endif // __XMC_RUNTIME_PARAM_HELPER__
