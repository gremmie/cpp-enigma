#ifndef CPP_ENIGMA_ROTOR_FACTORY_H
#define CPP_ENIGMA_ROTOR_FACTORY_H
// Copyright (C) 2012 by Brian Neal.
// This file is part of Cpp-Enigma, the Enigma Machine simulation.
// Cpp-Enigma is released under the MIT License (see License.txt).
//
// rotor_factory.h - simulated rotor & reflector factory functions

#include <memory>

namespace enigma
{
   class rotor;

   // Create a historical rotor with the given ring setting and return it:
   std::unique_ptr<rotor> create_rotor(const char* name, int ring_setting = 0);

   // Create a historical reflector and return it:
   std::unique_ptr<rotor> create_reflector(const char* name);
}

#endif
