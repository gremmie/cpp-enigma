#ifndef CPP_ENIGMA_ROTOR_H
#define CPP_ENIGMA_ROTOR_H
// Copyright (C) 2012 by Brian Neal.
// This file is part of Cpp-Enigma, the Enigma Machine simulation.
// Cpp-Enigma is released under the MIT License (see License.txt).
//
// rotor.h - This file contains the rotor class.

#include <string>
#include "enigma_types.h"


namespace enigma
{
   class rotor_error : public enigma_error
   {
   public:
      explicit rotor_error(const std::string& what_arg)
       : enigma_error(what_arg)
      {}
   };

   // The rotor class represents the Enigma Machine rotors (Walzen).
   //
   // A rotor has 26 circularly arranged pins on the right (entry) side and 26
   // contacts on the left side. Each pin is connected to a single contact by
   // internal wiring, thus establishing a substitution cipher. We represent this
   // wiring by establishing a mapping from a pin to a contact (and vice versa for
   // the return path). Internally we number the pins and contacts from 0-25 in a
   // clockwise manner with 0 being the "top".
   //
   // An alphabetic or numeric ring is fastened to the rotor by the operator. The
   // labels of this ring are displayed to the operator through a small window on
   // the top panel. The ring can be fixed to the rotor in one of 26 different
   // positions; this is called the ring setting (Ringstellung). We will number
   // the ring settings from 0-25 where 0 means no offset (e.g. the letter "A" is
   // mapped to pin 0 on an alphabetic ring). A ring setting of 1 means the letter
   // "B" is mapped to pin 0.
   //
   // Each rotor can be in one of 26 positions on the spindle, with position 0
   // where pin/contact 0 is being indicated in the operator window. The rotor
   // rotates towards the operator by mechanical means during normal operation as
   // keys are being pressed during data entry. Position 1 is thus defined to be
   // one step from position 0. Likewise, position 25 is the last position before
   // another step returns it to position 0, completing 1 trip around the spindle.
   //
   // Finally, a rotor has a "stepping" or "turnover" parameter. Physically this
   // is implemented by putting a notch on the alphabet ring and it controls when
   // the rotor will "kick" the rotor to its left, causing the neighbor rotor to
   // rotate. Most rotors had one notch, but some Kriegsmarine rotors had 2
   // notches and thus rotated twice as fast.
   //
   // Note that due to the system of ratchets and pawls, the middle rotor (in a 3
   // rotor Enigma) can "double-step". The middle rotor will advance on the next
   // step of the first rotor a second time in a row, if the middle rotor is in
   // its own turnover position.
   //
   // Note that we allow the stepping parameter to be None. This indicates the
   // rotor does not rotate. This allows us to model the entry wheel and
   // reflectors as stationary rotors.

   class rotor
   {
   public:
      // rotor constructor:
      //
      // model_name - e.g. "I", "II", "III", "Beta", "Gamma"
      //
      // wiring - this should be a string of 26 alphabetic characters that
      // represents the internal wiring transformation of the signal as it enters
      // from the right side. This is the format used in various online
      // resources. For example, for the Wehrmacht Enigma type I rotor the
      // mapping is "EKMFLGDQVZNTOWYHXUSPAIBRCJ".
      //
      // ring_setting - this should be an integer from 0-25, inclusive, which
      // indicates the Ringstellung. A value of 0 means there is no offset; e.g.
      // the letter "A" is fixed to pin 0. A value of 1 means "B" is mapped to
      // pin 0.
      //
      // stepping - this is the stepping or turnover parameter. It should be
      // a string such as "Q". This will indicate that when the rotor transitions
      // from "Q" to "R" (by observing the operator window), the rotor will "kick"
      // the rotor to its left, causing it to rotate. If the rotor has more than one
      // notch, a string of length 2 could be used, e.g. "ZM". Another way to think
      // of this parameter is that when a character in the stepping string is visible
      // in the operator window, a notch is lined up with the pawl on the left side
      // of the rotor. This will allow the pawl to push up on the rotor *and* the
      // rotor to the left when the next key is depressed.
      //
      // Note that for purposes of simulation, our rotors will always use
      // alphabetic labels A-Z. In reality, the Heer & Luftwaffe devices used
      // numbers 01-26, and Kriegsmarine devices used A-Z. Our usage of A-Z is
      // simply for simulation convenience.
      // display.

      rotor(const char* name, const char* wiring, int ring_setting = 0, const char* stepping = 0);

      // Returns the rotor name:
      const std::string& name() const { return rotor_name; }

      // Spin the rotor such that the string val appears in the operator window:
      void set_display(const char* val);

      // Returns what is currently being displayed in the operator window:
      std::string get_display() const;

      // Simulate a signal entering the rotor from the right at a given pin:
      // n must be an integer between 0 and 25.
      // Returns the contact number of the output signal (0-25).
      int signal_in(int n) const;

      // Simulate a signal entering the rotor from the left at a given contact position n.
      // n must be an integer between 0 and 25.
      // Returns the pin number of the output signal (0-25).
      int signal_out(int n) const;

      // Return true if this rotor has a notch in the stepping position and false otherwise:
      bool notch_over_pawl() const;

      // Rotate the rotor forward one step:
      void rotate();

   private:
      std::string rotor_name;
      std::string wiring_str;
      int ring_setting;
      int pos;
      int rotations;
   };
}

#endif
