## HPGL - High Perfomance Geostatistics Library (ver. 0.9.9)

## PROJECT SITE

Look for updates and other information at [http://hpgl.mit-ufa.com/](http://hpgl.mit-ufa.com/)

## DESCRIPTION

HPGL stands for High Perfomance Geostatistics Library.
HPGL was written in C++ / Python to realize some geostatistical algorithms (see full list below). The algorithms are called in Python, by executing the corresponding commands. 

HPGL utilizes some open-source components including:

1. modified version of GsTL (see \GsTL-1.3 folder)
2. TNT (Template Numerical Toolkit)
3. boost libraries boost (i.e. boost::python)

## BUILD

1. *NIX systems:
In order to build the HPGL you will need to install scons, gcc, g++, libgomp (OpenMP), python, and boost::python packages (or build them from sources on sources-based distributions).
After doing that, just type "scons -j X" from the HPGL root folder, where X is the number of CPUs which you want to use in building.

2. Windows:
First, you will need to build the boost::python library. How to do it see on the boost site (www.boost.org).
You can build HPGL by starting scons script (like in *nix building), but if you have MS Visual Studio, use the "hpgl.sln" solution instead.

## LICENSE

For non-commercial use (research, education, etc) HPGL is distributed under GPL version 2 license.
For any questions on the possibilities of commercial distribution, please contact the Authors.

## THE AUTHORS

Managment & Math:

- Savichev Vladimir
- Bezrukov Andrey

Programming (C++, Python), testing, support:

- Muharlyamov Arthur
- Barskiy Konstantin
- Nasibullina Dina
- Safin Rustam

## ACKNOWLEDGEMENTS

- The Authors wish to thank Andre Journel for his valuable support and indefatigable enthusiasm.
- The Authors also thank Iskander Shafikov for his assistance with the English translations and the User Guide cover