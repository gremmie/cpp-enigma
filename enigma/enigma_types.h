#ifndef CPP_ENIGMA_ENIGMA_TYPES_H
#define CPP_ENIGMA_ENIGMA_TYPES_H
// Copyright (C) 2012 by Brian Neal.
// This file is part of Cpp-Enigma, the Enigma Machine simulation.
// Cpp-Enigma is released under the MIT License (see License.txt).
//
// enigma_types.h - This file contains common types used throughout Cpp-Enigma.

#include <exception>
#include <string>
#include <array>

namespace enigma
{
   class enigma_error : public std::exception
   {
   public:
      explicit enigma_error(const std::string& what_arg)
       : what_arg(what_arg)
      {}

      virtual ~enigma_error() throw() {}

      virtual const char* what() const throw()
      {
         return what_arg.c_str();
      }

   private:
      std::string what_arg;
   };

   // Arrays of 26 items are very commonly used:
   typedef std::array<int, 26> alpha_int_array;
   typedef std::array<bool, 26> alpha_bool_array;
}

#endif
