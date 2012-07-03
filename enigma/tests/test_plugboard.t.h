// Copyright (C) 2012 by Brian Neal.
// This file is part of Cpp-Enigma, the Enigma Machine simulation.
// Cpp-Enigma is released under the MIT License (see License.txt).
//
// test_plugboard.t.h - Unit tests for the plugboard class.

#include <algorithm>
#include <cxxtest/TestSuite.h>
#include "plugboard.h"

using namespace enigma;


class plugboard_test_suite : public CxxTest::TestSuite
{
public:

   void test_bad_settings()
   {
      // too many
      TS_ASSERT_THROWS(plugboard("AB CD EF GH IJ KL MN OP QR ST UV"), plugboard_error);
      TS_ASSERT_THROWS(plugboard("18/26 17/4 21/6 3/16 19/14 22/7 8/1 12/25 5/9 10/15 2/20"),
                       plugboard_error);

      // duplicate
      TS_ASSERT_THROWS(plugboard("AB CD EF GH IJ KL MN OF QR ST"), plugboard_error);
      TS_ASSERT_THROWS(plugboard("AB CD EF GH IJ KL MN FP QR ST"), plugboard_error);
      TS_ASSERT_THROWS(plugboard("18/26 17/4 21/6 3/16 19/14 22/3 8/1 12/25"), plugboard_error);

      // invalid
      TS_ASSERT_THROWS(plugboard("A2 CD EF GH IJ KL MN FP QR ST"), plugboard_error);
      TS_ASSERT_THROWS(plugboard("AB CD EF *H IJ KL MN FP QR ST"), plugboard_error);
      TS_ASSERT_THROWS(plugboard("ABCD EF GH IJKLMN OP"), plugboard_error);
      TS_ASSERT_THROWS(plugboard("A-D EF GH OP"), plugboard_error);
      TS_ASSERT_THROWS(plugboard("A"), plugboard_error);
      TS_ASSERT_THROWS(plugboard("9"), plugboard_error);
      TS_ASSERT_THROWS(plugboard("1*/26 17/4 21/6 3/16 19/14 22/3 8/1 12/25"), plugboard_error);
      TS_ASSERT_THROWS(plugboard("18/26 17/4 2A/6 3/16 19/14 22/3 8/1 12/25"), plugboard_error);
      TS_ASSERT_THROWS(plugboard("100/2"), plugboard_error);
      TS_ASSERT_THROWS(plugboard("T/C"), plugboard_error);
   }

   void test_valid_settings()
   {
      TS_ASSERT_THROWS_NOTHING(plugboard{});
      TS_ASSERT_THROWS_NOTHING(plugboard{""});
      TS_ASSERT_THROWS_NOTHING(plugboard(plugboard::pair_vector{}));
      TS_ASSERT_THROWS_NOTHING(plugboard{"AB CD EF GH IJ KL MN OP QR ST"});
      TS_ASSERT_THROWS_NOTHING(plugboard{"CD EF GH IJ KL MN OP QR ST"});
      TS_ASSERT_THROWS_NOTHING(plugboard{"EF GH IJ KL MN OP QR ST"});
      TS_ASSERT_THROWS_NOTHING(plugboard{" GH "});
      TS_ASSERT_THROWS_NOTHING(plugboard{"18/26 17/4 21/6 3/16 19/14 22/7 8/1 12/25 5/9 10/15"});
      TS_ASSERT_THROWS_NOTHING(plugboard{"18/26 17/4"});
      TS_ASSERT_THROWS_NOTHING(plugboard{" 18/26 "});
   }

   void test_default_wiring()
   {
      plugboard p;
      for (int i = 0; i < 26; ++i)
      {
         TS_ASSERT_EQUALS(i, p.signal(i));
      }
   }

   void test_wiring()
   {
      std::vector<std::string> settings{"AB CD EF GH IJ KL MN OP QR ST",
                  "1/2 3/4 5/6 7/8 9/10 11/12 13/14 15/16 17/18 19/20"};

      for (const auto& setting : settings)
      {
         plugboard p{setting};
         for (int n = 0; n < 26; ++n)
         {
            if (n < 20)
            {
               if (n % 2 == 0)
               {
                  TS_ASSERT_EQUALS(p.signal(n), n + 1);
               }
               else
               {
                  TS_ASSERT_EQUALS(p.signal(n), n - 1);
               }
            }
            else
            {
               TS_ASSERT_EQUALS(p.signal(n), n);
            }
         }
      }
   }

   void test_wiring2()
   {
      std::string stecker{"AV BS CG DL FU HZ IN KM OW RX"};
      plugboard p{stecker};
      plugboard::pair_vector pairs{p.get_pairs()};

      plugboard::pair_vector expected_pairs{
         {0, 21}, {1, 18}, {2, 6}, {3, 11}, {5, 20},
         {7, 25}, {8, 13}, {10, 12}, {14, 22}, {17, 23}
      };

      TS_ASSERT_EQUALS(expected_pairs, pairs);

      std::map<int, int> wiring;
      for (const auto& p : pairs)
      {
         wiring.insert(p);
         wiring.insert(std::make_pair(p.second, p.first));
      }

      for (int n = 0; n < 26; ++n)
      {
         auto iter = wiring.find(n);
         if (iter != wiring.end())
         {
            TS_ASSERT_EQUALS(p.signal(n), iter->second);
         }
         else
         {
            TS_ASSERT_EQUALS(p.signal(n), n);
         }
      }
   }

   void test_army_str()
   {
      std::string stecker{"AB CD EF GH IJ KL MN OP QR ST"};
      plugboard p{stecker};
      TS_ASSERT_EQUALS(stecker, p.army_str());
   }

   void test_navy_str()
   {
      std::string stecker{"1/2 3/4 5/6 7/8 9/10 11/12 13/14 15/16 17/18 19/20"};
      plugboard p{stecker};
      TS_ASSERT_EQUALS(stecker, p.navy_str());
   }

   void test_get_wiring()
   {
      plugboard pb;
      auto w(pb.get_wiring());

      for (int i = 0; i < 26; ++i)
      {
         TS_ASSERT_EQUALS(w[i], i);
      }

      pb.connect(0, 1);
      pb.connect(8, 20);
      pb.connect(24, 25);
      for (int i = 0; i < 26; ++i)
      {
         w[i] = i;
      }
      std::swap(w[0], w[1]);
      std::swap(w[8], w[20]);
      std::swap(w[24], w[25]);

      auto w1(pb.get_wiring());
      for (int i = 0; i < 26; ++i)
      {
         TS_ASSERT_EQUALS(w[i], w1[i]);
      }
   }

   void test_set_wiring()
   {
      alpha_int_array w;
      for (int i = 0; i < 26; ++i)
      {
         w[i] = i;
      }
      std::swap(w[0], w[1]);
      std::swap(w[8], w[20]);
      std::swap(w[24], w[25]);

      plugboard pb;
      pb.set_wiring(w);

      auto w2 = pb.get_wiring();
      TS_ASSERT_EQUALS(w, w2);
   }

   void test_is_wired()
   {
      plugboard pb;
      pb.connect(0, 25);
      pb.connect(4, 18);
      for (int i = 0; i < 26; ++i)
      {
         if (i == 0 || i == 25 || i == 4 || i == 18)
         {
            TS_ASSERT(pb.is_wired(i));
         }
         else
         {
            TS_ASSERT(!pb.is_wired(i));
         }
      }
   }

   void test_is_free()
   {
      plugboard pb;
      pb.connect(0, 25);
      pb.connect(4, 18);
      for (int i = 0; i < 26; ++i)
      {
         if (i == 0 || i == 25 || i == 4 || i == 18)
         {
            TS_ASSERT(!pb.is_free(i));
         }
         else
         {
            TS_ASSERT(pb.is_free(i));
         }
      }
   }

   void test_disconnect()
   {
      plugboard pb;
      pb.connect(0, 25);
      pb.connect(4, 18);
      pb.disconnect(0);
      pb.disconnect(18);
      for (int i = 0; i < 26; ++i)
      {
         TS_ASSERT(pb.is_free(i));
      }
   }

   void test_disconnect2()
   {
      plugboard pb;
      for (int i = 0; i < 26; ++i)
      {
         pb.disconnect(i);
      }
      for (int i = 0; i < 26; ++i)
      {
         TS_ASSERT(pb.is_free(i));
      }
   }

   void test_connect()
   {
      plugboard pb;
      pb.connect(2, 4);
      pb.connect(17, 20);
      for (int i = 0; i < 26; ++i)
      {
         if (i == 2 || i == 4 || i == 17 || i == 20)
         {
            TS_ASSERT(pb.is_wired(i));
         }
         else
         {
            TS_ASSERT(pb.is_free(i));
         }
      }
      pb.connect(20, 25);
      for (int i = 0; i < 26; ++i)
      {
         if (i == 2 || i == 4 || i == 25 || i == 20)
         {
            TS_ASSERT(pb.is_wired(i));
         }
         else
         {
            TS_ASSERT(pb.is_free(i));
         }
      }
   }

   void test_is_connected()
   {
      plugboard pb;
      pb.connect(5, 7);
      pb.connect(1, 20);
      TS_ASSERT(pb.is_connected(5, 7));
      TS_ASSERT(pb.is_connected(1, 20));

      for (int i = 0; i < 26; ++i)
      {
         if (i == 7)
         {
            TS_ASSERT(pb.is_connected(5, i));
         }
         else
         {
            TS_ASSERT(!pb.is_connected(5, i));
         }
         if (i == 1)
         {
            TS_ASSERT(pb.is_connected(20, i));
         }
         else
         {
            TS_ASSERT(!pb.is_connected(20, i));
         }
      }
   }

   void test_unplug_all()
   {
      plugboard pb;
      pb.connect(5, 7);
      pb.connect(1, 20);
      pb.connect(18, 24);
      pb.unplug_all();
      for (int i = 0; i < 26; ++i)
      {
         TS_ASSERT(pb.is_free(i));
      }
      pb.unplug_all();
      for (int i = 0; i < 26; ++i)
      {
         TS_ASSERT(pb.is_free(i));
      }
   }

};
