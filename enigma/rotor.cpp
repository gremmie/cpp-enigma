// Copyright (C) 2012 by Brian Neal.
// This file is part of Cpp-Enigma, the Enigma Machine simulation.
// Cpp-Enigma is released under the MIT License (see License.txt).
//
// rotor.cpp - Implementation file for the rotor class.

#include <set>
#include <array>
#include <algorithm>
#include "rotor.h"
#include "enigma_utils.h"

using namespace enigma;

////////////////////////////////////////////////////////////////////////////////

namespace
{
   const char* const ucase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

   // Turned into a function to avoid translation unit initialization order problems.
   const std::set<char>& get_ucase_set()
   {
      static const std::set<char> the_set(ucase, ucase + 26);
      return the_set;
   }
}

////////////////////////////////////////////////////////////////////////////////

rotor::rotor(const char* name, const char* wiring, int ring_setting, const char* stepping)
 : rotor_name{name},
   wiring_str{wiring},
   ring_setting{ring_setting},
   pos{0},
   display_val{'A'},
   entry_map(),
   exit_map(),
   display_map(),
   pos_map(),
   step_map()
{
   // check wiring length
   if (wiring_str.size() != 26)
   {
      throw rotor_error("invalid wiring length");
   }

   // ensure wiring contains only uppercase letters & every char must appear
   // exactly once:
   const std::set<char>& ucase_set(get_ucase_set());

   // g++ 4.6.3 warns about missing braces unless we double them up, below:
   std::array<int, 26> letter_counts = {{ 0 }};
   for (int i = 0; i < 26; ++i)
   {
      const char c{wiring_str[i]};
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

   if (ring_setting < 0 || ring_setting >= 26)
   {
      throw rotor_error("invalid ring setting");
   }
   set_ring_setting(ring_setting);

   // Initialize our two arrays that describe the internal wiring. Arrays are used
   // to do fast lookup from both entry (from the right) and exit (from the
   // left).

   for (int i = 0; i < 26; ++i)
   {
      const int v = wiring_str[i] - 'A';
      entry_map[i] = v;
      exit_map[v] = i;
   }

   // Build a lookup table that tells us when the pawls are allowed to step.
   // The index to this array is the current display letter [A-Z] - 'A'.

   if (stepping != nullptr)
   {
      for (char c = *stepping; *stepping != '\0'; ++stepping)
      {
         if (ucase_set.find(c) != ucase_set.end())
         {
            step_map[c - 'A'] = true;
         }
         else
         {
            throw rotor_error("invalid stepping");
         }
      }
   }

   // set initial position
   set_display('A');
}

////////////////////////////////////////////////////////////////////////////////

void rotor::set_ring_setting(int n)
{
   ring_setting = n;

   // Build a mapping from window display values to positions
   // and a reverse mapping of position to display value:
   for (int i = 0; i < 26; ++i)
   {
      const int n = alpha_mod(i - ring_setting);
      display_map[i] = n;
      pos_map[n] = i + 'A';
   }
}
