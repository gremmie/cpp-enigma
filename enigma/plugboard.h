#ifndef CPP_ENIGMA_PLUGBOARD_H
#define CPP_ENIGMA_PLUGBOARD_H
// Copyright (C) 2012 by Brian Neal.
// This file is part of Cpp-Enigma, the Enigma Machine simulation.
// Cpp-Enigma is released under the MIT License (see License.txt).
//
// plugboard.h - This file contains the plugboard class.

#include <vector>
#include <utility>
#include <string>
#include <cstddef>
#include "enigma_types.h"

namespace enigma
{
   class plugboard_error : public enigma_error
   {
   public:
      explicit plugboard_error(const std::string& what_arg)
       : enigma_error(what_arg)
      {}
   };

   // The plugboard allows the operator to swap letters before and after the
   // entry wheel. This is accomplished by connecting cables between pairs of
   // plugs that are marked with letters (Heer & Luftwaffe models) or numbers
   // (Kriegsmarine). Ten cables were issued with each machine; thus up to 10 of
   // these swappings could be used as part of a machine setup.

   // Each cable swaps both the input and output signals. Thus if A is connected
   // to B, A crosses to B in the keyboard to entry wheel direction and also in
   // the reverse entry wheel to lamp direction.

   class plugboard
   {
   public:
      const static std::size_t max_pairs = 10;

      typedef std::vector<std::pair<int, int>> pair_vector;

      // Construct a plugboard with no connections:
      plugboard();

      // Construct from a vector of integer pairs that describe the
      // connections. Each integer must be between [0-25], and the
      // vector can have no more than max_pairs pairs. Each plug should
      // be present at most once. A plugboard_error will be thrown if
      // the pair_vector is invalid.

      explicit plugboard(const pair_vector& pairs);

      // Configure the plugboard according to a settings string as you may
      // find on a key sheet.
      //
      // Two syntaxes are supported, the Heer/Luftwaffe and Kriegsmarine styles:
      //
      // In the Heer syntax, the settings are given as a string of
      // alphabetic pairs. For example: 'PO ML IU KJ NH YT GB VF RE DC'
      //
      // In the Kriegsmarine syntax, the settings are given as a string of number
      // pairs, separated by a '/'. Note that the numbering uses 1-26, inclusive.
      // For example: '18/26 17/4 21/6 3/16 19/14 22/7 8/1 12/25 5/9 10/15'
      //
      // To specify no plugboard connections, settings can be an empty string.
      //
      // A PlugboardError will be raised if the settings string is invalid, or if
      // it contains more than max_pairs pairs. Each plug should be present at
      // most once in the settings string.

      explicit plugboard(const std::string& settings);

      // Return the current settings as a vector of pairs:
      pair_vector get_pairs() const;

      // Return the current settings as a string in Heer (army) format:
      std::string army_str() const;

      // Return the current settings as a string in Kriegsmarine (navy) format:
      std::string navy_str() const;

      // Simulate a signal entering the plugboard on wire n, where n must be
      // an integer between 0 and 25.
      //
      // Returns the wire number of the output signal (0-25).
      //
      // Note that since the plugboard always crosses pairs of wires, it doesn't
      // matter what direction (keyboard -> entry wheel or vice versa) the signal
      // is coming from.

      int signal(int n) const
      {
         return wiring_map[n];
      }

      //
      // Functions to support hill-climbing:
      //

      // Return the internal state of the wiring:
      alpha_int_array get_wiring() const
      {
         return wiring_map;
      }

      // Sets the internal state of the wiring:
      void set_wiring(const alpha_int_array& wiring)
      {
         wiring_map = wiring;
      }

      // Returns true if connection n has a cable attached to it.
      // 0 <= n < 26
      bool is_wired(int n) const
      {
         return wiring_map[n] != n;
      }

      // Returns true if connection n has no cable attached to it.
      // 0 <= n < 26
      bool is_free(int n) const
      {
         return wiring_map[n] == n;
      }

      // Removes cable from plug number n [0-25].
      void disconnect(int n)
      {
         const int x = wiring_map[n];
         wiring_map[x] = x;
         wiring_map[n] = n;
      }

      // Connects plug x to plug y, removing any existing connection first.
      // x & y must be in [0-25].
      void connect(int x, int y)
      {
         // disconnect any existing connections
         const int m = wiring_map[x];
         const int n = wiring_map[y];
         wiring_map[m] = m;
         wiring_map[n] = n;

         wiring_map[x] = y;
         wiring_map[y] = x;
      }

      // Returns true if plug x is connected to plug y.
      // x & y must be in [0-25].
      bool is_connected(int x, int y)
      {
         return wiring_map[x] == y && wiring_map[y] == x;
      }

      // Unplugs all cables
      void unplug_all()
      {
         for (auto i = 0U; i < wiring_map.size(); ++i)
         {
            wiring_map[i] = i;
         }
      }

   private:
      alpha_int_array wiring_map;

      // common constructor code:
      void construct_wiring(const pair_vector& pairs);
   };


   // This class can be used to save & restore the state of a plugboard
   // in RAII style:

   class plugboard_state_saver
   {
   public:
      explicit plugboard_state_saver(plugboard& pb)
       : pb(pb)
      {
         state = pb.get_wiring();
      }

      ~plugboard_state_saver()
      {
         pb.set_wiring(state);
      }

      // disable copying & assignment
      plugboard_state_saver(const plugboard_state_saver&) = delete;
      plugboard_state_saver& operator=(const plugboard_state_saver&) = delete;

   private:
      plugboard& pb;
      alpha_int_array state;
   };

}

#endif
