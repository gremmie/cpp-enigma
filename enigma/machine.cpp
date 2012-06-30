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
      std::shared_ptr<rotor> reflector,
      const plugboard& pb)
 : rotors(rv),
   reflector(reflector),
   pb(pb)
{
   rotor_count_check();
}

////////////////////////////////////////////////////////////////////////////////

enigma_machine::enigma_machine(
      const rotor_vector& rv,
      std::shared_ptr<rotor> reflector)
 : rotors(rv),
   reflector(reflector),
   pb()
{
   rotor_count_check();
}

////////////////////////////////////////////////////////////////////////////////

enigma_machine::enigma_machine(
      const std::vector<std::string>& rotor_types,
      const std::vector<int>& ring_settings,
      const std::string& reflector_name,
      const std::string& plugboard_settings)
 : rotors(),
   reflector(create_reflector(reflector_name.c_str())),
   pb(plugboard_settings)
{
   for (const auto& name : rotor_types)
   {
      rotors.push_back(create_rotor(name.c_str()));
   }
   rotor_count_check();

   // if ring settings are supplied, there has to be one for each rotor
   if (!ring_settings.empty())
   {
      if (rotors.size() != ring_settings.size())
      {
         throw enigma_machine_error("rotor/ring setting count mismatch");
      }

      for (std::size_t i = 0; i < rotors.size(); ++i)
      {
         rotors[i]->set_ring_setting(ring_settings[i]);
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

void enigma_machine::rotor_count_check()
{
   if (rotors.size() != 3 && rotors.size() != 4)
   {
      throw enigma_machine_error("rotor count");
   }

   if (rotors.size() == 3)
   {
      r_rotor = rotors[2].get();
      m_rotor = rotors[1].get();
      l_rotor = rotors[0].get();
   }
   else
   {
      r_rotor = rotors[3].get();
      m_rotor = rotors[2].get();
      l_rotor = rotors[1].get();
   }
}

////////////////////////////////////////////////////////////////////////////////

std::string enigma_machine::str(bool army) const
{
   std::ostringstream os;

   os << reflector->name() << ' ';

   for (const auto& r : rotors)
   {
      os << r->name() << '/' << r->get_ring_setting() << ' ';
   }

   os << get_display() << ' ' << (army ? pb.army_str() : pb.navy_str());

   return os.str();
}

