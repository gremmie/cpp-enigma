// Copyright (C) 2012 by Brian Neal.
// This file is part of Cpp-Enigma, the Enigma Machine simulation.
// Cpp-Enigma is released under the MIT License (see License.txt).
//
// rotor.cpp - Implementation file for the rotor class.

#include <set>
#include <array>
#include <algorithm>
#include "rotor.h"

using namespace enigma;

namespace
{
   const char* const ucase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
   const std::set<char> ucase_set(ucase, ucase + 26);
}

rotor::rotor(const char* name, const char* wiring, int ring_setting, const char* stepping)
 : rotor_name(name),
   wiring_str(wiring),
   ring_setting(ring_setting),
   pos(0),
   rotations(0)
{
   // check wiring length
   if (wiring_str.size() != 26)
   {
      throw rotor_error("invalid wiring length");
   }

   // ensure wiring contains only uppercase letters & every char must appear
   // exactly once:

   std::array<int, 26> letter_counts {{ 0 }};
   for (int i = 0; i < 26; ++i)
   {
      const char c(wiring_str[i]);

      if (ucase_set.find(c) == ucase_set.end())
      {
         throw rotor_error("invalid wiring");
      }
      ++letter_counts[c - 'A'];
   }

   if (std::find_if(letter_counts.begin(),
                    letter_counts.end(),
                    [](int n) { return n != 1; }) != letter_counts.end())
   {
      throw rotor_error("invalid wiring; duplicate letter");
   }
}
