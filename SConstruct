env = Environment(
   tools = ['default', 'cxxtest'],
   CXXFLAGS='-std=c++0x -Wall -Wextra -pedantic -O3',
   CPPPATH='#/enigma',
   LIBS='enigma',
   LIBPATH='#/build',
)

SConscript(['enigma/SConscript'], exports='env', variant_dir='build', duplicate=0)
