#ifndef CPP_ENIGMA_ENIGMA_UTILS_H
#define CPP_ENIGMA_ENIGMA_UTILS_H
// Copyright (C) 2012 by Brian Neal.
// This file is part of Cpp-Enigma, the Enigma Machine simulation.
// Cpp-Enigma is released under the MIT License (see License.txt).
//
// enigma_utils.h - This file contains common functions used throughout Cpp-Enigma.

#include <string>

namespace enigma
{
   // This version of mod acts like Python's with respect to negative dividends.
   inline int alpha_mod(int dividend)
   {
      if (dividend < 0)
      {
         dividend += 26;
      }
      return dividend % 26;
   }

   // Removes spaces from a string and returns the resulting string:
   std::string remove_spaces(const std::string& s);

   // Removes spaces & converts to signal numbers:
   std::string preprocess_ciphertext(const std::string& s);
}

#endif
