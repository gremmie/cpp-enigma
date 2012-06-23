// Copyright (C) 2012 by Brian Neal.
// This file is part of Cpp-Enigma, the Enigma Machine simulation.
// Cpp-Enigma is released under the MIT License (see License.txt).
//
// test_rotor.t.h - rotor unit tests

#include <deque>
#include <string>
#include <memory>
#include <cxxtest/TestSuite.h>
#include "rotor.h"
#include "enigma_utils.h"
#include "rotor_data.h"
#include "rotor_factory.h"

using namespace enigma;

const char* const wiring = "EKMFLGDQVZNTOWYHXUSPAIBRCJ";


class MyTestSuite1 : public CxxTest::TestSuite
{
public:

    void test_bad_wiring()
    {
       TS_ASSERT_THROWS(rotor("I", ""), rotor_error);
       TS_ASSERT_THROWS(rotor("I", "ABC"), rotor_error);
       TS_ASSERT_THROWS(rotor("I", "123"), rotor_error);
       TS_ASSERT_THROWS(rotor("I", "!\"#$%&'()*+,-./:;<=>?@[\\]^"), rotor_error);
       TS_ASSERT_THROWS(rotor("I", "ABCDABCDABCDABCDABCDABCDAB"), rotor_error);
    }

    void test_bad_ring_setting()
    {
       TS_ASSERT_THROWS(rotor("I", wiring, -1), rotor_error);
       TS_ASSERT_THROWS(rotor("I", wiring, 26), rotor_error);
    }

    void test_bad_stepping()
    {
       TS_ASSERT_THROWS(rotor("I", wiring, 1, "0"), rotor_error);
       TS_ASSERT_THROWS(rotor("I", wiring, 1, "-"), rotor_error);
       TS_ASSERT_THROWS(rotor("I", wiring, 1, "A%"), rotor_error);
       TS_ASSERT_THROWS(rotor("I", wiring, 1, "A%14"), rotor_error);
    }

    void test_display()
    {
       for (int i = 0; i < 26; ++i)
       {
          rotor r{"I", wiring, i};
          for (int j = 0; j < 26; ++j)
          {
             r.set_display(j + 'A');
             TS_ASSERT_EQUALS(j + 'A', r.get_display());
          }
       }
    }

    // Loop through all ring settings & rotor positions and test the wiring.
    void test_wiring()
    {
       for (int r = 0; r < 26; ++r)
       {
          rotor test_rotor("I", wiring, r);

          for (int n = 0; n < 26; ++n)
          {
             const char d = n + 'A';
             test_rotor.set_display(d);

             std::deque<char> wiring_deque(wiring, wiring + 26);
             // rotate contents to the right if positive, left if negative:
             int rotate_count = r - n;
             const bool rotate_right = rotate_count >= 0;
             if (rotate_count < 0)
             {
                rotate_count = -rotate_count;
             }
             for (int x = 0; x < rotate_count; ++x)
             {
                if (rotate_right)
                {
                   wiring_deque.push_front(wiring_deque.back());
                   wiring_deque.pop_back();
                }
                else
                {
                   wiring_deque.push_back(wiring_deque.front());
                   wiring_deque.pop_front();
                }
             }

             for (int i = 0; i < 26; ++i)
             {
                int output = test_rotor.signal_in(i);
                int expected = alpha_mod(wiring_deque[i] - 'A' + r - n);
                TS_ASSERT_EQUALS(output, expected);

                output = test_rotor.signal_out(expected);
                TS_ASSERT_EQUALS(output, i);
             }
          }
       }
    }

    // For every rotor we simulate, ensure that the notch setting is correct
    // regardless of the ring setting.
    void test_notches()
    {
       for (const auto& p : simulated_rotors)
       {
          const std::string& rotor_name(p.first);
          const rotor_data& rd(p.second);
          if (rd.stepping == nullptr)
          {
             continue;
          }
          const std::string notches(rd.stepping);

          for (int r = 0; r < 26; ++r)
          {
             std::unique_ptr<rotor> rp = create_rotor(rotor_name.c_str(), r);
             rp->set_display('A');

             for (int n = 0; n < 26; ++n)
             {
                const bool over_notch = notches.find(rp->get_display()) != std::string::npos;
                TS_ASSERT_EQUALS(over_notch, rp->notch_over_pawl());
             }
          }
       }
    }

    void test_rotate()
    {
       for (int r = 0; r < 26; ++r)
       {
          rotor r1("X", wiring, r);
          rotor r2("Y", wiring, r);

          r2.set_display('A');
          for (int i = 0; i < 26; ++i)
          {
             r1.set_display(i + 'A');
             TS_ASSERT_EQUALS(r1.get_display(), r2.get_display());
             r2.rotate();
          }
       }
    }

    void test_ring_setting()
    {
       rotor r("X", wiring, 0);
       for (int n = 0; n < 26; ++n)
       {
          r.set_ring_setting(n);
          TS_ASSERT_EQUALS(n, r.get_ring_setting());

          r.set_display('A');
          for (int a = 0; a < 26; ++a)
          {
             TS_ASSERT_EQUALS(a + 'A', r.get_display());
             r.rotate();
          }
       }
    }
};
