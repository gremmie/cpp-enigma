// Copyright (C) 2012 by Brian Neal.
// This file is part of Cpp-Enigma, the Enigma Machine simulation.
// Cpp-Enigma is released under the MIT License (see License.txt).
//
// plugboard.cpp - This is the implementation file for the plugboard class.

#include <algorithm>
#include <set>
#include <sstream>
#include <utility>
#include "plugboard.h"

using namespace enigma;

////////////////////////////////////////////////////////////////////////////////

namespace
{
   // Returns a wiring map with "straight-through" mapping, where every input
   // pin 'i' is wired to the output pin 'i':

   alpha_int_array straight_through_mapping()
   {
      alpha_int_array result;
      for (alpha_int_array::size_type i = 0; i < result.size(); ++i)
      {
         result[i] = i;
      }
      return result;
   }
}

////////////////////////////////////////////////////////////////////////////////

plugboard::plugboard()
 : wiring_map(straight_through_mapping())
{
}

////////////////////////////////////////////////////////////////////////////////

plugboard::plugboard(const pair_vector& pairs)
 : wiring_map(straight_through_mapping())
{
   construct_wiring(pairs);
}

////////////////////////////////////////////////////////////////////////////////

plugboard::plugboard(const std::string& settings)
 : wiring_map(straight_through_mapping())
{
   if (settings.empty())
   {
      return;
   }

   pair_vector pairs;

   // detect which syntax is being used
   if (settings.find('/') == std::string::npos)
   {
      // Assume Heer (army) syntax

      std::istringstream iss(settings);
      std::string s;
      while (iss >> s)
      {
         if (s.size() != 2)
         {
            throw plugboard_error("invalid settings string");
         }
         const int m = std::toupper(s[0]) - 'A';
         const int n = std::toupper(s[1]) - 'A';

         pairs.push_back(std::make_pair(m, n));
      }
   }
   else
   {
      // Assume Kriegsmarine (navy) syntax

      std::istringstream iss(settings);
      std::string s;
      while (iss >> s)
      {
         const std::size_t x = s.find('/');
         if (x == std::string::npos || x == s.size() - 1)
         {
            throw plugboard_error("invalid settings string");
         }

         int m;
         int n;
         std::istringstream mss(s.substr(0, x));
         std::istringstream nss(s.substr(x + 1));

         if ((mss >> m) && (nss >> n))
         {
            pairs.push_back(std::make_pair(m - 1, n - 1));
         }
         else
         {
            throw plugboard_error("invalid settings string");
         }
      }
   }

   construct_wiring(pairs);
}

////////////////////////////////////////////////////////////////////////////////

plugboard::pair_vector plugboard::get_pairs() const
{
   std::set<std::pair<int, int>> pair_set;
   for (int i = 0; i < 26; ++i)
   {
      const int j = wiring_map[i];
      if (i < j)
      {
         pair_set.insert(std::make_pair(i, j));
      }
   }

   return pair_vector(pair_set.begin(), pair_set.end());
}

////////////////////////////////////////////////////////////////////////////////

std::string plugboard::army_str() const
{
   const auto pairs = get_pairs();

   std::string s;

   for (const auto& p : pairs)
   {
      s += static_cast<char>(p.first + 'A');
      s += static_cast<char>(p.second + 'A');
      s += ' ';
   }

   if (!s.empty())
   {
      s.erase(s.size() - 1);     // erase trailing space
   }
   return s;
}

////////////////////////////////////////////////////////////////////////////////

std::string plugboard::navy_str() const
{
   const auto pairs = get_pairs();

   std::ostringstream os;
   for (const auto& p : pairs)
   {
      os << (p.first + 1) << '/' << (p.second + 1) << ' ';
   }

   std::string s(os.str());
   if (!s.empty())
   {
      s.erase(s.size() - 1);     // erase trailing space
   }
   return s;
}

////////////////////////////////////////////////////////////////////////////////

void plugboard::construct_wiring(const pair_vector& pairs)
{
   if (pairs.size() > max_pairs)
   {
      throw plugboard_error("Too many pairs");
   }

   // range check the wiring & ensure a path appears at most once
   // (the double braces were added because gcc 4.6.3 emits a warning without
   // them with -std=c++0x -Wall -Wextra -pedantic)
   alpha_int_array counts = {{ 0 }};
   for (const auto& p : pairs)
   {
      if (p.first < 0 || p.second < 0 || p.first >= 26 || p.second >= 26)
      {
         throw plugboard_error("invalid wiring pair");
      }
      ++counts[p.first];
      ++counts[p.second];
   }

   if (std::find_if(counts.begin(),
                    counts.end(),
                    [](int n) { return n > 1; }) != counts.end())
   {
      throw plugboard_error("duplicate connection");
   }

   // all checks pass if we made it this far; make the connections

   for (auto& p : pairs)
   {
      wiring_map[p.first] = p.second;
      wiring_map[p.second] = p.first;
   }
}
