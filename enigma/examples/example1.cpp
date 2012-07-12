// Copyright (C) 2012 by Brian Neal.
// This file is part of Cpp-Enigma, the Enigma Machine simulation.
// Cpp-Enigma is released under the MIT License (see License.txt).
//
// example1.cpp - Quick example program showing usage.

#include <iostream>
#include <string>
#include "machine.h"

using namespace enigma;

int main()
{
   // setup machine according to specs from a key sheet:

   enigma_machine em({"II", "IV", "V"}, {1, 20, 11}, "B",
         "AV BS CG DL FU HZ IN KM OW RX");

   // set initial rotor starting position
   em.set_display("WXC");

   // decrypt the message key
   const std::string msg_key = em.process_text("KCH");

   // decrypt the ciphertext with the unencrypted message key
   em.set_display(msg_key);

   const std::string ciphertext("NIBLFMYMLLUFWCASCSSNVHAZ");
   const std::string plaintext(em.process_text(ciphertext));

   std::cout << plaintext << std::endl;
   return 0;
}
