#ifndef CPP_ENIGMA_MACHINE_H
#define CPP_ENIGMA_MACHINE_H
// Copyright (C) 2012 by Brian Neal.
// This file is part of Cpp-Enigma, the Enigma Machine simulation.
// Cpp-Enigma is released under the MIT License (see License.txt).
//
// machine.h - This file contains the main Enigma machine class.

#include <algorithm>
#include <memory>
#include <string>
#include <vector>
#include <cassert>
#include <cstddef>
#include "enigma_types.h"
#include "rotor.h"
#include "plugboard.h"

namespace enigma
{
   typedef std::vector<rotor*> rotor_vector;

   class enigma_machine_error : public enigma_error
   {
   public:
      explicit enigma_machine_error(const std::string& what_arg)
       : enigma_error(what_arg)
      {}
   };

   class enigma_machine
   {
   public:
      // Construct an Enigma machine from component parts.
      // Note that the enigma_machine makes copies of the rotors and will not
      // delete the rotor pointers:
      enigma_machine(const rotor_vector& rv,
                     const rotor& reflector,
                     const plugboard& pb);

      // construct an Enigma machine with a default plugboard (no cables connected):
      enigma_machine(const rotor_vector& rv,
                     const rotor& reflector);

      // key-sheet style constructors:
      enigma_machine(const std::vector<std::string>& rotor_types,
                     const std::vector<int>& ring_settings,
                     const std::string& reflector_name = "B",
                     const std::string& plugboard_settings = "");

      // set the rotor display (starting position) - 3 rotor version
      void set_display(char left, char mid, char right)
      {
         assert(rotors.size() == 4);

         rotors[1].set_display(left);
         rotors[2].set_display(mid);
         rotors[3].set_display(right);
      }

      // set the rotor display (starting position) - 4 rotor version
      void set_display(char c0, char c1, char c2, char c3)
      {
         assert(rotors.size() == 5);

         rotors[1].set_display(c0);
         rotors[2].set_display(c1);
         rotors[3].set_display(c2);
         rotors[4].set_display(c3);
      }

      // Set the rotor display (starting position) using a string; the
      // string length must match the number of rotors in use or a
      // enigma_machine_error exception will be thrown:
      void set_display(const std::string& val)
      {
         if (val.size() == 3 && rotors.size() == 4)
         {
            set_display(val[0], val[1], val[2]);
         }
         else if (val.size() == 4 && rotors.size() == 5)
         {
            set_display(val[0], val[1], val[2], val[3]);
         }
         else
         {
            throw enigma_machine_error("set_display invalid size");
         }
      }

      // return the rotor display (starting position) as a string
      std::string get_display() const
      {
         std::string result;
         for (std::size_t i = 1; i < rotors.size(); ++i)
         {
            result += rotors[i].get_display();
         }
         return result;
      }

      // Returns the number of rotors in the machine (this count does not include
      // the reflector).
      std::size_t num_rotors() const
      {
         return rotors.size() - 1;
      }

      // For changing the ring setting on a rotor inside the machine.
      // Parameters:
      //    rotor - identifies the rotor to change the ring setting; must be
      //            in the range 0 - (num_rotors() - 1). 0 is the leftmost rotor.
      //    ring_setting - the ring setting value, 0-25
      //
      void set_ring_setting(int rotor, int ring_setting)
      {
         rotors[rotor + 1].set_ring_setting(ring_setting);
      }

      // For getting the ring setting on a rotor inside the machine.
      // Parameters:
      //    rotor - identifies the rotor to change the ring setting; must be
      //            in the range 0 - (num_rotors() - 1). 0 is the leftmost rotor.
      //
      int get_ring_setting(int rotor) const
      {
         return rotors[rotor + 1].get_ring_setting();
      }

      // For changing the ring settings on all rotors inside the machine.
      // Parameters:
      //    settings - a vector of ring settings, 0-25. The size of this
      //               vector must match num_rotors().
      //
      void set_ring_settings(const std::vector<int>& settings)
      {
         if (settings.size() == num_rotors())
         {
            rotor* r = &rotors[1];  // skip the reflector;
            for (auto s : settings)
            {
               r->set_ring_setting(s);
               ++r;
            }
         }
         else
         {
            throw enigma_machine_error("set_ring_settings rotor/settings size mismatch");
         }
      }

      // For getting the ring settings as a vector of integers. Element 0 corresponds
      // to the leftmost rotor.
      std::vector<int> get_ring_settings() const
      {
         std::vector<int> result(num_rotors());
         std::transform(rotors.begin() + 1, rotors.end(), result.begin(),
               [](const rotor& r) { return r.get_ring_setting(); });
         return result;
      }

      // simulate front panel key press; returns the lamp character that is lit
      char key_press(char c)
      {
         step_rotors();
         return electric_signal(c - 'A') + 'A';
      }

      // this is like key_press(), but it works in signal numbers (0-25) instead of chars:
      int step(int n)
      {
         step_rotors();
         return electric_signal(n);
      }

      // Process a buffer of text of length n, placing the result in an output buffer.
      void process_text(const char* input, char* output, std::size_t n)
      {
         for (std::size_t i = 0; i < n; ++i)
         {
            *output++ = key_press(*input++);
         }
      }

      // Process a buffer of text from a string, returning the result as a string.
      std::string process_text(const std::string& input)
      {
         std::string result;
         result.reserve(input.size());

         for (const auto& c : input)
         {
            result += key_press(c);
         }
         return result;
      }

      // Process a buffer of pre-processed text of length n, placing the result in an output buffer.
      void process_data(const char* input, char* output, std::size_t n)
      {
         for (std::size_t i = 0; i < n; ++i)
         {
            *output++ = step(*input++) + 'A';
         }
      }

      // for access to the plugboard for hill-climbing, etc
      plugboard& get_plugboard() { return pb; }

      // Returns a string representation of the enigma machine's state. Useful
      // for logging, etc:
      //
      std::string army_str() const { return str(true); }
      std::string navy_str() const { return str(false); }

   private:
      // Note that to improve cache performance, the rotors and reflectors are stored
      // in a contiguous vector.
      std::vector<rotor> rotors;    // rotor & reflector array
      plugboard pb;
      rotor* r_rotor;      // rightmost rotor
      rotor* m_rotor;      // 2nd to right rotor
      rotor* l_rotor;      // 3rd to right rotor

      void rotor_count_check();

      void step_rotors()
      {
         // The right-most rotor's right-side ratchet is always over a pawl, and
         // it has no neighbor to the right, so it always rotates.
         //
         // The middle rotor will rotate if either:
         //   1) The right-most rotor's left side notch is over the 2nd pawl
         //       or
         //   2) It has a left-side notch over the 3rd pawl
         //
         // The third rotor (from the right) will rotate only if the middle rotor
         // has a left-side notch over the 3rd pawl.
         //
         // Kriegsmarine model M4 has 4 rotors, but the 4th rotor (the leftmost)
         // does not rotate (they did not add a 4th pawl to the mechanism).

         const bool l_rotate = m_rotor->notch_over_pawl();
         const bool m_rotate = l_rotate || r_rotor->notch_over_pawl();

         r_rotor->rotate();
         if (m_rotate)
         {
            m_rotor->rotate();
         }
         if (l_rotate)
         {
            l_rotor->rotate();
         }
      }

      // Simulate running an electric signal through the machine in order to
      // perform an encrypt or decrypt operation
      // signal_num - the wire (0-25) that the simulated current occurs on
      // Returns a lamp number to light (an integer 0-25).
      int electric_signal(int signal_num)
      {
         int n = pb.signal(signal_num);

         if (rotors.size() == 4)    // 3 rotors + reflector
         {
            n = rotors[3].signal_in(n);
            n = rotors[2].signal_in(n);
            n = rotors[1].signal_in(n);
            n = rotors[0].signal_in(n);   // reflector
            n = rotors[1].signal_out(n);
            n = rotors[2].signal_out(n);
            n = rotors[3].signal_out(n);
         }
         else  // Kriegsmarine 4 rotor + reflector
         {
            n = rotors[4].signal_in(n);
            n = rotors[3].signal_in(n);
            n = rotors[2].signal_in(n);
            n = rotors[1].signal_in(n);
            n = rotors[0].signal_in(n);   // reflector
            n = rotors[1].signal_out(n);
            n = rotors[2].signal_out(n);
            n = rotors[3].signal_out(n);
            n = rotors[4].signal_out(n);
         }
         return pb.signal(n);
      }

      std::string str(bool army) const;
   };
}

#endif
