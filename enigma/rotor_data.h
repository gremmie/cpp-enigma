#ifndef CPP_ENIGMA_ROTOR_DATA_H
#define CPP_ENIGMA_ROTOR_DATA_H
// Copyright (C) 2012 by Brian Neal.
// This file is part of Cpp-Enigma, the Enigma Machine simulation.
// Cpp-Enigma is released under the MIT License (see License.txt).
//
// rotor_data.h - This file contains standard rotor & reflector data.

#include <map>
#include <string>

namespace enigma
{
   struct rotor_data
   {
      const char* wiring;
      const char* stepping;
   };

   typedef std::map<std::string, rotor_data> rotor_data_map;

   const rotor_data_map simulated_rotors
   {
      { "I",      { "EKMFLGDQVZNTOWYHXUSPAIBRCJ", "Q" } },
      { "II",     { "AJDKSIRUXBLHWTMCQGZNPYFVOE", "E" } },
      { "III",    { "BDFHJLCPRTXVZNYEIWGAKMUSQO", "V" } },
      { "IV",     { "ESOVPZJAYQUIRHXLNFTGKDCMWB", "J" } },
      { "V",      { "VZBRGITYUPSDNHLXAWMJQOFECK", "Z" } },
      { "VI",     { "JPGVOUMFYQBENHZRDKASXLICTW", "ZM"} },
      { "VII",    { "NZJHGRCXMYSWBOUFAIVLPEKQDT", "ZM"} },
      { "VIII",   { "FKQHTLXOCBJSPDZRAMEWNIUYGV", "ZM" } },
      { "Beta",   { "LEYJVCNIXWPBQMDRTAKZGFUHOS", nullptr } },
      { "Gamma",  { "FSOKANUERHMBTIYCWLQPZXVGJD", nullptr } },
    };

   typedef std::map<std::string, const char*> reflector_data_map;

   const reflector_data_map simulated_reflectors
   {
      { "B",      "YRUHQSLDPXNGOKMIEBFZCWVJAT" },
      { "C",      "FVPJIAOYEDRZXWGCTKUQSBNMHL" },
      { "B-Thin", "ENKQAUYWJICOPBLMDXZVFTHRGS" },
      { "C-Thin", "RDOBJNTKVEHMLFCWZAXGYIPSUQ" },
   };
}

#endif
