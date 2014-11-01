+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
PACC: Portable Agile C++ Classes

Copyright (C) 2005-2009
by Marc Parizeau <parizeau@gel.ulaval.ca> and other contributors:
   Sylvain Comtois
   Frederic Drolet
   Nicolas Dube
   Marc Dubreuil
   Michel Fortin
   Christian Gagne
   Frederic Jean
   Guillaume Mayer
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

ABOUT
=====

PACC is a collection of Portable Agile C++ Classes that can be 
retrieved from http:www.sourceforge.net/projects/pacc/. It is said 
to be portable because it can run on both Unix and Windows platforms. 
In general, the PACC classes have been tested and used under Linux, 
MacOSX, and Windows 2000/XP. They are also known to compile 
succesfully on AIX.

The PACC collection is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public License as 
published by the Free Software Foundation; either version 2.1 of the 
License, or (at your option) any later version. Read the License.txt
file for more details.

It is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

DOCUMENTATION
=============

Consult the PACC web page (http://manitou.gel.ulaval.ca/PACC) 
to get all the necessary information to use this framework.

INSTALLATION
============

To install the PACC library, you need first to install CMake 
(http://www.cmake.org), version 2.6 are later.

To build and install PACC, follow these easy instructions:

1- Create an empty directory anywhere outside of the PACC 
   sub-tree;

  > mkdir pacc-cmake

2- Set this directory as the current directory;

  > cd pacc-cmake

3- Run CMake with the path to the PACC source sub-tree;

  > cmake ../PACC
  
  (here the example assumes that the PACC directory is a 
   sibling of the current directory)

4- Run make to compile the sources and build the library;

  > make

5- Run make again with the "install" target to install the 
   library in a default OS dependent location (determined by 
   CMake);

  > sudo make install
  > password: enter-your-admin-password
  
  On Unix, it might be /usr/local/lib. If you want to change 
  this location, you need during step 3 to specify the new
  location to CMake, by changing variable CMAKE_INSTALL_PREFIX. 
  For example:
  
  > cmake -D CMAKE_INSTALL_PREFIX=my-own-specific-path ../PACC

  will configure the makefile to install the library in
  the directory pointed by "my-own-specific-path".

By default, CMake will compile the sources in release mode 
(with optimization flags and without debug symbols). If you want 
to trace your code into PACC, you need to tell CMake to compile 
with adequate debug flags using the "CMAKE_BUILD_TYPE=Debug" 
variable. This variable can be set on the command line, just like 
the "CMAKE_INSTALL_PREFIX" variable above:

  > cmake -D CMAKE_BUILD_TYPE=Debug ../PACC
  
 An alternative is to run CMake in interactive mode:

  > cmake -i ../PACC
  
CMake will then ask you questions so that you can configure every 
aspect of the build process, by allowing you to update any of its 
internal build variables. Refer to the CMake manual for further 
details.
If you have install the CMake GUI, you may also want to use it
instead of command-line interface.


Some variables are specific to PACC and should be considered :

  - PACC_LIBRARY_TYPE = [STATIC | SHARED] defines whether CMake 
  should build static or shared (dynamic) library. On Unix 
  platforms and on Windows without the MinGW (GNU) compiler, 
  default behavior is to SHARED. When using the Microsoft VC++
  compiler, default is to STATIC, because PACC can not be build
  as shared library with this compiler at this time.

  - PACC_ZLIB = [bool] allows to disable the use of ZLib
  compression library even if CMake found it in the system

  - PACC_USE_PARALLEL_STL = [bool] is only available on recent
  versions of the GCC compiler. It allows the compiler to use OpenMP
  in order to parallelize STL algorithms. You should be warned that
  the parallel STL containers can not be casted in normal STL
  containers. In doubt, leave it to False / Off.

  - PACC_CREATE_*** is a set of variables used to tell to CPack what 
  kind of binary package it should make when called by "cpack" or
  "make package".
  