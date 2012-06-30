// Copyright (C) 2012 by Brian Neal.
// This file is part of Cpp-Enigma, the Enigma Machine simulation.
// Cpp-Enigma is released under the MIT License (see License.txt).
//
// test_machine.t.h - Unit tests for the enigma_machine class.

#include <memory>
#include <string>
#include <vector>
#include <cxxtest/TestSuite.h>
#include "machine.h"
#include "enigma_utils.h"

using namespace enigma;


class display_suite : public CxxTest::TestSuite
{
public:

   void test_set_display3()
   {
      enigma_machine m({"II", "IV", "V"}, {}, "B");
      TS_ASSERT_THROWS(m.set_display("ABCD"), enigma_machine_error);
      TS_ASSERT_THROWS_NOTHING(m.set_display("ABC"));
   }

   void test_set_display4()
   {
      enigma_machine m({"Gamma", "II", "IV", "V"}, {}, "B-Thin");
      TS_ASSERT_THROWS(m.set_display("BCD"), enigma_machine_error);
      TS_ASSERT_THROWS_NOTHING(m.set_display("ABCD"));
   }

   void test_navy_str()
   {
      const std::string stecker = "1/20 2/12 4/6 7/10 8/13 14/23 15/16 17/25 18/26 22/24";
      enigma_machine machine({"Beta", "II", "IV", "I"}, {0, 0, 0, 21}, "B-Thin", stecker);

      TS_ASSERT_EQUALS(machine.navy_str(), "B-Thin Beta/0 II/0 IV/0 I/21 AAAA "
               "1/20 2/12 4/6 7/10 8/13 14/23 15/16 17/25 18/26 22/24");
   }

   void test_army_str()
   {
      enigma_machine machine({"II", "IV", "V"}, {1, 20, 11}, "B",
               "AV BS CG DL FU HZ IN KM OW RX");

      TS_ASSERT_EQUALS(machine.army_str(), "B II/1 IV/20 V/11 AAA "
               "AV BS CG DL FU HZ IN KM OW RX");
   }
};

class stepping_test_suite : public CxxTest::TestSuite
{
public:

   void test_double_stepping()
   {
      // Ensure the rotors step realistically by testing for a "double-step"
      // This example taken from
      // http://users.telenet.be/d.rijmenants/en/enigmatech.htm
      // in the section on "The Stepping Mechanism."

      enigma_machine m({"III", "II", "I"}, {});
      m.set_display('K', 'D', 'O');

      const std::vector<std::string> truth_data = {
               "KDP", "KDQ", "KER", "LFS", "LFT", "LFU",
      };

      for (const auto& expected : truth_data)
      {
         m.key_press('A');
         TS_ASSERT_EQUALS(m.get_display(), expected);
      }
   }
};


class simple_cipher_test_suite : public CxxTest::TestSuite
{
public:

   void setUp()
   {
      m.reset(new enigma_machine({"I", "II", "III"}, {}));
      m->set_display('A', 'A', 'A');
      plaintext = "AAAAA";
      ciphertext = "BDZGO";
   }

   void test_simple_encrypt()
   {
      std::vector<char> buffer(plaintext.size());
      m->process_text(plaintext.c_str(), buffer.data(), plaintext.size());
      TS_ASSERT_EQUALS(ciphertext, std::string(buffer.begin(), buffer.end()));
   }

   void test_simple_decrypt()
   {
      std::vector<char> buffer(plaintext.size());
      m->process_text(ciphertext.c_str(), buffer.data(), ciphertext.size());
      TS_ASSERT_EQUALS(plaintext, std::string(buffer.begin(), buffer.end()));
   }

private:
   std::unique_ptr<enigma_machine> m;
   std::string plaintext;
   std::string ciphertext;
};


// This example taken from Dirk Rijmenants' simulator manual.
//
// It is credited to Frode Weierud and Geoff Sullivan.
// http://cryptocellar.com
//
class actual_decrypt_test_suite : public CxxTest::TestSuite
{
public:
   void setUp()
   {
      m.reset(new enigma_machine({"II", "IV", "V"}, {1, 20, 11}, "B",
               "AV BS CG DL FU HZ IN KM OW RX"));
   }

   void decrypt(const std::string& start,
                const std::string& enc_key,
                const std::string& ciphertext,
                const std::string& truth_data)
   {
      // remove spaces & Kenngruppen from ciphertext
      std::string ctext(ciphertext.begin() + 5, ciphertext.end());
      ctext = remove_spaces(ctext);

      // remove spaces from truth_data
      const std::string expected(remove_spaces(truth_data));

      // decrypt message key to get start position
      m->set_display(start[0], start[1], start[2]);
      std::string key = m->process_text(enc_key);

      // decrypt the message with the key
      m->set_display(key[0], key[1], key[2]);

      const std::string plaintext = m->process_text(ctext);

      TS_ASSERT_EQUALS(plaintext, expected);
   }

   void test_decrpyt_1()
   {
      const std::string ciphertext =
                  "RFUGZ EDPUD NRGYS ZRCXN"
                  "UYTPO MRMBO FKTBZ REZKM"
                  "LXLVE FGUEY SIOZV EQMIK"
                  "UBPMM YLKLT TDEIS MDICA"
                  "GYKUA CTCDO MOHWX MUUIA"
                  "UBSTS LRNBZ SZWNR FXWFY"
                  "SSXJZ VIJHI DISHP RKLKA"
                  "YUPAD TXQSP INQMA TLPIF"
                  "SVKDA SCTAC DPBOP VHJK";

      const std::string truth_data =
                  "AUFKL XABTE ILUNG XVONX"
                  "KURTI NOWAX KURTI NOWAX"
                  "NORDW ESTLX SEBEZ XSEBE"
                  "ZXUAF FLIEG ERSTR ASZER"
                  "IQTUN GXDUB ROWKI XDUBR"
                  "OWKIX OPOTS CHKAX OPOTS"
                  "CHKAX UMXEI NSAQT DREIN"
                  "ULLXU HRANG ETRET ENXAN"
                  "GRIFF XINFX RGTX";

      decrypt("WXC", "KCH", ciphertext, truth_data);
   }

   void test_decrpyt_2()
   {
      const std::string ciphertext =
          "FNJAU SFBWD NJUSE GQOBH"
          "KRTAR EEZMW KPPRB XOHDR"
          "OEQGB BGTQV PGVKB VVGBI"
          "MHUSZ YDAJQ IROAX SSSNR"
          "EHYGG RPISE ZBOVM QIEMM"
          "ZCYSG QDGRE RVBIL EKXYQ"
          "IRGIR QNRDN VRXCY YTNJR";

      const std::string truth_data =
          "DREIG EHTLA NGSAM ABERS"
          "IQERV ORWAE RTSXE INSSI"
          "EBENN ULLSE QSXUH RXROE"
          "MXEIN SXINF RGTXD REIXA"
          "UFFLI EGERS TRASZ EMITA"
          "NFANG XEINS SEQSX KMXKM"
          "XOSTW XKAME NECXK";

      decrypt("CRS", "YPJ", ciphertext, truth_data);
   }

private:
   std::unique_ptr<enigma_machine> m;
};


// This is the Kriegsmarine example from Dirk Rijmenants' simulator manual.
//
// It is credited to Stefan Krah and the M4 project:
// http://www.bytereef.org/m4_project.html
//
class kriegsmarine_test_suite : public CxxTest::TestSuite
{
public:

   void test_decrypt()
   {
      const std::string stecker = "1/20 2/12 4/6 7/10 8/13 14/23 15/16 17/25 18/26 22/24";

      enigma_machine machine({"Beta", "II", "IV", "I"}, {0, 0, 0, 21}, "B-Thin", stecker);

      std::string ciphertext = remove_spaces(
        "FCLC QRKN NCZW VUSX PNYM INHZ XMQX SFWX WLKJ AHSH NMCO CCAK UQPM KCSM"
        "HKSE INJU SBLK IOSX CKUB HMLL XCSJ USRR DVKO HULX WCCB GVLI YXEO AHXR"
        "HKKF VDRE WEZL XOBA FGYU JQUK GRTV UKAM EURB VEKS UHHV OYHA BCJW MAKL"
        "FKLM YFVN RIZR VVRT KOFD ANJM OLBG FFLE OPRG TFLV RHOW OPBE KVWM UQFM"
        "PWPA RMFH AGKX IIBG FCLC QRKM VA");

      // remove the message indicators from the message (the first and last 2
      // groups of the message -- it appears the last partial group 'VA' should
      // be removed also)

      ciphertext = std::string(ciphertext.begin() + 8, ciphertext.end() - 10);

      machine.set_display('V', 'J', 'N', 'A');
      const std::string plaintext = machine.process_text(ciphertext);

      const std::string truth_data = remove_spaces(
         "VONV ONJL OOKS JHFF TTTE"
         "INSE INSD REIZ WOYY QNNS"
         "NEUN INHA LTXX BEIA NGRI"
         "FFUN TERW ASSE RGED RUEC"
         "KTYW ABOS XLET ZTER GEGN"
         "ERST ANDN ULAC HTDR EINU"
         "LUHR MARQ UANT ONJO TANE"
         "UNAC HTSE YHSD REIY ZWOZ"
         "WONU LGRA DYAC HTSM YSTO"
         "SSEN ACHX EKNS VIER MBFA"
         "ELLT YNNN NNNO OOVI ERYS"
         "ICHT EINS NULL");

      TS_ASSERT_EQUALS(plaintext, truth_data);
   }

};
