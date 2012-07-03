// Copyright (C) 2012 by Brian Neal.
// This file is part of Cpp-Enigma, the Enigma Machine simulation.
// Cpp-Enigma is released under the MIT License (see License.txt).
//
// machine.cpp - The implementation file for the main Enigma machine class.

#include <cstddef>
#include <sstream>
#include "machine.h"
#include "rotor.h"
#include "rotor_factory.h"

using namespace enigma;

////////////////////////////////////////////////////////////////////////////////

enigma_machine::enigma_machine(
      const rotor_vector& rv,
      const rotor& reflector,
      const plugboard& pb)
 : rotors(),
   pb(pb)
{
   rotors.push_back(reflector);
   for (const auto& r : rv)
   {
      rotors.push_back(*r);
   }
   rotor_count_check();
}

////////////////////////////////////////////////////////////////////////////////

enigma_machine::enigma_machine(
      const rotor_vector& rv,
      const rotor& reflector)
 : rotors(),
   pb()
{
   rotors.push_back(reflector);
   for (const auto& r : rv)
   {
      rotors.push_back(*r);
   }
   rotor_count_check();
}

////////////////////////////////////////////////////////////////////////////////

enigma_machine::enigma_machine(
      const std::vector<std::string>& rotor_types,
      const std::vector<int>& ring_settings,
      const std::string& reflector_name,
      const std::string& plugboard_settings)
 : rotors(),
   pb(plugboard_settings)
{
   const auto ukw(create_reflector(reflector_name.c_str()));
   rotors.push_back(*ukw);
   for (const auto& name : rotor_types)
   {
      const auto r(create_rotor(name.c_str()));
      rotors.push_back(*r);
   }
   rotor_count_check();

   // if ring settings are supplied, there has to be one for each rotor
   if (!ring_settings.empty())
   {
      if (rotors.size() - 1 != ring_settings.size())
      {
         throw enigma_machine_error("rotor/ring setting count mismatch");
      }

      for (std::size_t i = 1; i < rotors.size(); ++i)
      {
         rotors[i].set_ring_setting(ring_settings[i - 1]);
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

void enigma_machine::rotor_count_check()
{
   // the first rotor is actually the reflector; so we should have a total
   // of 4 or 5 rotors
   if (rotors.size() != 4 && rotors.size() != 5)
   {
      throw enigma_machine_error("rotor count");
   }

   if (rotors.size() == 4)
   {
      r_rotor = &rotors[3];
      m_rotor = &rotors[2];
      l_rotor = &rotors[1];
   }
   else
   {
      r_rotor = &rotors[4];
      m_rotor = &rotors[3];
      l_rotor = &rotors[2];
   }
}

////////////////////////////////////////////////////////////////////////////////

std::string enigma_machine::str(bool army) const
{
   std::ostringstream os;

   os << rotors[0].name() << ' ';

   for (std::size_t i = 1; i < rotors.size(); ++i)
   {
      os << rotors[i].name() << '/' << rotors[i].get_ring_setting() << ' ';
   }

   os << get_display() << ' ' << (army ? pb.army_str() : pb.navy_str());

   return os.str();
}

