==========
Cpp-Enigma
==========
A historically accurate Enigma Machine library written in C++11
---------------------------------------------------------------

:Author: Brian Neal <bgneal@gmail.com>
:Version: 0.1
:Date: July 11, 2012
:Home Page: https://bitbucket.org/bgneal/cpp-enigma/
:License: MIT License (see LICENSE.txt)
:Support: https://bitbucket.org/bgneal/cpp-enigma/issues


Overview
--------

**Cpp-Enigma** is a C++11 library for simulating the `Enigma machines`_ used
by the German armed forces (Wehrmacht) during World War 2. Cpp-Enigma makes it
possible to both encrypt and decrypt messages that can be sent to, or received
from, actual Enigma machines used by the German army (Heer), air force
(Luftwaffe), and navy (Kriegsmarine).

It is my hope that library will be useful to Enigma enthusiasts, historians, and
students interested in cryptography.

Cpp-Enigma is basically a C++11 port of my `Py-Enigma
<https://bitbucket.org/bgneal/enigma>`_ library, which is written in Python 3.
Cpp-Enigma was created when I started doing performance intensive hill-climbing
searches for Enigma keys. The Python library is very convenient for quick
experiments and simple brute force attacks when most of the key parameters are
known. Cpp-Enigma is for when you need much greater performance, such as a
cipher-text only attack involving hill-climbing.


Quick Example
-------------

This example shows how the library can be used to decode a message using the
procedure employed by the German army::
   
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

This program prints::

   THEXRUSSIANSXAREXCOMINGX


Requirements
------------

Cpp-Enigma is written in C++, and uses features found in the C++11 standard. The
code was developed on GCC 4.6.3.

Cpp-Enigma ships with SCons_ build files. 

Cpp-Enigma's unit tests require the CxxTest_ unit test framework. This is only
required if you plan on running or modifying the tests.

Cpp-Enigma has no other requirements or dependencies.


Installation
------------

You may download a tarball or .zip file of the latest code using the "get
source" link on the `Cpp-Enigma Bitbucket page`_. Alternatively if you use
Mercurial_, you can clone the repository with the following command::

   $ hg clone https://bitbucket.org/bgneal/cpp-enigma

Once you have obtained a copy of the source and installed SCons_ the software
can be compiled on GCC with::

   $ scons

If you have CxxTest_ installed and configured to work with SCons_, you may run
the unit tests with::

   $ scons check

I have not currently compiled the code on other compilers or environments.
Patches or pull requests are welcome.

Documentation
-------------

Currently there is no documentation for Cpp-Enigma. However the design and API
is almost identical to Py-Enigma, which is well documented. I will quote from
the Py-Enigma README file below:

   The latest documentation is available at `Read the Docs
   <http://readthedocs.org/projects/py-enigma/>`_. There you can `browse the
   documentation online <http://readthedocs.org/docs/py-enigma/en/latest/>`_, or
   `download it in a variety of formats
   <http://readthedocs.org/projects/py-enigma/downloads/>`_.

   Sources for the documentation are also included in Sphinx_ format. If you
   install Sphinx you can generate the documentation in several output formats.


Support
-------

Support is provided at the `issue tracker`_ at the `Cpp-Enigma Bitbucket page`_.
If you have general questions or comments, please feel free to email me (address
at the top of this file). 

And please, if you use Cpp-Enigma for anything, even if it is just learning,
please let me know!


Acknowledgements & References
-----------------------------

This software would not have been possible without the thorough and detailed
descriptions of the Enigma machine on Dirk Rijmenants' incredible `Cipher
Machines and Cryptology website`_. In particular, his `Technical Details of the
Enigma Machine`_ page was a gold mine of information.

Dirk has also written an `Enigma simulator`_ in Visual Basic. Although I did not
look at his source code, I did use his simulator to check the operation of
Cpp-Enigma.

I would also like to recommend the photos and video at Dr. Thomas B. Perera's
`Enigma Museum`_.

Another good website is `The Enigma and the Bombe`_ by Graham Ellsbury.

A nice video which shows the basic components and operation of the Enigma
Machine is on YouTube: `Nadia Baker & Enigma demo`_.


.. _Enigma machines: http://en.wikipedia.org/wiki/Enigma_machine
.. _Cpp-Enigma Bitbucket page: https://bitbucket.org/bgneal/cpp-enigma
.. _Mercurial: http://mercurial.selenic.com/
.. _issue tracker: https://bitbucket.org/bgneal/cpp-enigma/issues
.. _SCons: http://www.scons.org/
.. _CxxTest: http://cxxtest.com/
.. _Sphinx: http://sphinx.pocoo.org/
.. _Cipher Machines and Cryptology website: http://users.telenet.be/d.rijmenants/index.htm
.. _Technical Details of the Enigma Machine: http://users.telenet.be/d.rijmenants/en/enigmatech.htm
.. _Enigma simulator: http://users.telenet.be/d.rijmenants/en/enigmasim.htm
.. _Enigma Museum: http://w1tp.com/enigma/
.. _The Enigma and the Bombe: http://www.ellsbury.com/enigmabombe.htm
.. _Nadia Baker & Enigma demo: http://youtu.be/HBHYAzuVeWc
