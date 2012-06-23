// Copyright (C) 2012 by Brian Neal.
// This file is part of Cpp-Enigma, the Enigma Machine simulation.
// Cpp-Enigma is released under the MIT License (see License.txt).
//
// rotor_factory.cpp - Implementation file for the rotor & reflector factory functions.

#include <string>
#include "rotor_factory.h"
#include "rotor_data.h"
#include "rotor.h"

////////////////////////////////////////////////////////////////////////////////

std::unique_ptr<enigma::rotor> enigma::create_rotor(const char* name, int ring_setting)
{
   auto iter = simulated_rotors.find(name);
   if (iter == simulated_rotors.end())
   {
      throw rotor_error("unknown rotor type: " + std::string(name));
   }

   const rotor_data& rd(iter->second);
   return std::unique_ptr<rotor>{new rotor{name, rd.wiring, ring_setting, rd.stepping}};
}

////////////////////////////////////////////////////////////////////////////////

std::unique_ptr<enigma::rotor> enigma::create_reflector(const char* name)
{
   auto iter = simulated_reflectors.find(name);
   if (iter == simulated_reflectors.end())
   {
      throw rotor_error("unknown reflector type: " + std::string(name));
   }

   return std::unique_ptr<rotor>{new rotor{name, iter->second}};
}
