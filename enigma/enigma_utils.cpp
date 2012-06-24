// Copyright (C) 2012 by Brian Neal.
// This file is part of Cpp-Enigma, the Enigma Machine simulation.
// Cpp-Enigma is released under the MIT License (see License.txt).
//
// enigma_utils.cpp - Implementation file for enigma_utils.h.

#include "enigma_utils.h"

std::string enigma::remove_spaces(const std::string& s)
{
   std::string result;
   result.reserve(s.size());

   for (const auto& c : s)
   {
      if (c != ' ')
      {
         result += c;
      }
   }
   return result;
}
