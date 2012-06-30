#ifndef CPP_ENIGMA_MACHINE_H
#define CPP_ENIGMA_MACHINE_H
// Copyright (C) 2012 by Brian Neal.
// This file is part of Cpp-Enigma, the Enigma Machine simulation.
// Cpp-Enigma is released under the MIT License (see License.txt).
//
// machine.h - This file contains the main Enigma machine class.

#include <memory>
#include <string>
#include <vector>
#include <cassert>
#include "enigma_types.h"
#include "rotor.h"
#include "plugboard.h"

namespace enigma
{
   typedef std::vector<std::unique_ptr<rotor>> rotor_vector;

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
      // construct an Enigma machine from component parts:
      enigma_machine(rotor_vector rv,
                     std::unique_ptr<rotor> reflector,
                     const plugboard& pb);

      // construct an Enigma machine with a default plugboard (no cables connected):
      enigma_machine(rotor_vector rv,
                     std::unique_ptr<rotor> reflector);

      // key-sheet style constructors:
      enigma_machine(const std::vector<std::string>& rotor_types,
                     const std::vector<int>& ring_settings,
                     const std::string& reflector_name = "B",
                     const std::string& plugboard_settings = "");

      // set the rotor display (starting position) - 3 rotor version
      void set_display(char left, char mid, char right)
      {
         assert(rotors.size() == 3);

         rotors[0]->set_display(left);
         rotors[1]->set_display(mid);
         rotors[2]->set_display(right);
      }

      // set the rotor display (starting position) - 4 rotor version
      void set_display(char c0, char c1, char c2, char c3)
      {
         assert(rotors.size() == 4);

         rotors[0]->set_display(c0);
         rotors[1]->set_display(c1);
         rotors[2]->set_display(c2);
         rotors[3]->set_display(c3);
      }

      // Set the rotor display (starting position) using a string; the
      // string length must match the number of rotors in use or a
      // enigma_machine_error exception will be thrown:
      void set_display(const std::string& val)
      {
         if (val.size() == 3 && rotors.size() == 3)
         {
            set_display(val[0], val[1], val[2]);
         }
         else if (val.size() == 4 && rotors.size() == 4)
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
         for (const auto& r : rotors)
         {
            result += r->get_display();
         }
         return result;
      }

      // simulate front panel key press; returns the lamp character that is lit
      char key_press(char c)
      {
         step_rotors();
         return electric_signal(c - 'A') + 'A';
      }

      // Process a buffer of text of length n, placing the result in an output buffer.
      void process_text(const char* input, char* output, std::size_t n)
      {
         for (std::size_t i = 0; i < n; ++i)
         {
            *output++ = key_press(*input++);
         }
      }

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

      // for access to the plugboard for hill-climbing, etc
      plugboard& get_plugboard() { return pb; }

      // Returns a string representation of the enigma machine's state. Useful
      // for logging, etc:
      //
      std::string army_str() const { return str(true); }
      std::string navy_str() const { return str(false); }

   private:
      rotor_vector rotors;
      std::unique_ptr<rotor> reflector;
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
         int pos = pb.signal(signal_num);

         for (auto r = rotors.rbegin(); r != rotors.rend(); ++r)
         {
            pos = (*r)->signal_in(pos);
         }

         pos = reflector->signal_in(pos);

         for (const auto& r : rotors)
         {
            pos = r->signal_out(pos);
         }

         return pb.signal(pos);
      }

      std::string str(bool army) const;
   };
}

#endif
