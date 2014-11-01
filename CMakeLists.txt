# CMakeLists.txt toto
# Auto configuration script
# For PACC (Portable Agile C++ Classes), by Marc Parizeau
# Laboratoire de vision et systèmes numériques, Université Laval
# Copyright (c) 2009
#

project(PACC C CXX)
set(PACC_VERSION 1.4.1)

# This is the shared library versioning info
set(LIBRARY_VERSION 5.1.0)
#                   | | |
#            +------+ | +---+
#            |        |     |
#         current:revision:age
#            |        |     |
#            |        |     +- increment if interfaces have been added
#            |        |        set to zero if interfaces have been removed
#            |        |        or changed
#            |        +- increment if source code has changed
#            |           set to zero if current is incremented
#            +- increment if interfaces have been added, removed or changed

cmake_minimum_required(VERSION 2.6) 		# Minimum cmake version required

# Define build mode
if(NOT DEFINED CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
	message(STATUS "++ Building in release mode (default)...")
	set(CMAKE_BUILD_TYPE "RELEASE")
	set(PACC_NDEBUG true)
else(NOT DEFINED CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
	string(TOUPPER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE)
	if(CMAKE_BUILD_TYPE STREQUAL "DEBUG")
		message(STATUS "++ Building in debug mode...")
		set(PACC_NDEBUG false)
	elseif(NOT CMAKE_BUILD_TYPE STREQUAL "MINSIZEREL" AND NOT CMAKE_BUILD_TYPE STREQUAL "RELEASE" AND NOT CMAKE_BUILD_TYPE STREQUAL "RELWITHDEBINFO")
		message(STATUS "## Warning : unhandled build mode ${CMAKE_BUILD_TYPE}, will build in release mode")
		set(CMAKE_BUILD_TYPE "RELEASE" FORCE)
		set(PACC_NDEBUG true)
	else(CMAKE_BUILD_TYPE STREQUAL "DEBUG")				
		set(PACC_NDEBUG true)
	endif(CMAKE_BUILD_TYPE STREQUAL "DEBUG")
endif(NOT DEFINED CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")

# Define whether we build static or shared libs
if(NOT DEFINED PACC_LIBRARY_TYPE)
	if(MSVC)		# MSVC = Microsoft Visual C++
		message(STATUS "++ Building static library (default on Microsoft Visual C++)")
		set(PACC_LIBRARY_TYPE "STATIC" CACHE STRING "Type of the output library (static or dynamic/shared)")
	else(MSVC)
		set(PACC_LIBRARY_TYPE "SHARED" CACHE STRING "Type of the output library (static or dynamic/shared)")	
	endif(MSVC)
endif(NOT DEFINED PACC_LIBRARY_TYPE)

string(TOUPPER "${PACC_LIBRARY_TYPE}" PACC_LIBRARY_TYPE)

if(PACC_LIBRARY_TYPE STREQUAL "STATIC")
	message(STATUS "++ Building static library")
elseif(PACC_LIBRARY_TYPE STREQUAL "SHARED")
	if(MSVC)
		message("## Warning : build shared (dynamic) PACC library is not supported yet on Microsoft Visual C++ compiler")
		message("## since it requires export definitions in the source code that are not done at this time")
		message("## If you want to build dynamic library on Windows, you may want to use MinGW compiler")
		message("## PACC_LIBRARY_TYPE will be set to STATIC")
		message(STATUS "++ Building static library")
		set(PACC_LIBRARY_TYPE "STATIC" CACHE STRING "Type of the output library (static or dynamic/shared)" FORCE)
	else(MSVC)
		message(STATUS "++ Building dynamic library")
	endif(MSVC)
else(PACC_LIBRARY_TYPE STREQUAL "STATIC")
	if(MSVC)
		message(STATUS "## Warning : invalid value for PACC_LIBRARY_TYPE (${PACC_LIBRARY_TYPE}), will be set to STATIC")
		set(PACC_LIBRARY_TYPE "STATIC" CACHE STRING "Type of the output library (static or dynamic/shared)" FORCE)
		message(STATUS "++ Building static library")
	else(MSVC)
		message("## Warning : invalid value for PACC_LIBRARY_TYPE (${PACC_LIBRARY_TYPE}), will be set to SHARED")
		set(PACC_LIBRARY_TYPE "SHARED" CACHE STRING "Type of the output library (static or dynamic/shared)" FORCE)
		message(STATUS "++ Building dynamic library")
	endif(MSVC)
endif(PACC_LIBRARY_TYPE STREQUAL "STATIC")

# OS detection and packaging options
if(APPLE)
	option(PACC_CREATE_PKG "Do you want to create a PACC .dmg package?" ON)
	message(STATUS "++ MacOS (Unix-like) found")
	set(PACC_USE_UNIX true)
elseif(UNIX)
	message(STATUS "++ Unix-like found...")
	set(PACC_USE_UNIX true)

	option(PACC_CREATE_DEB "Do you want to create .deb file?" OFF)
	option(PACC_CREATE_RPM "Do you want to create .rpm file ('rpmbuild' needed)?" OFF)	
	option(PACC_CREATE_UNIX_BIN "Do you want to create Unix self installer (.sh)?" ON)

elseif(WIN32)
	message(STATUS "++ Windows found...")
	set(PACC_USE_WINDOWS TRUE)

	find_program(NSIS_FOUND NSIS PATHS "C:/Program Files/NSIS") 	# Try to locate NSIS
	if(NSIS_FOUND MATCHES NSIS_FOUND-NOTFOUND)
		message(STATUS "++ NSIS installer creator not found")
		option(PACC_CREATE_WIN32_SETUP "Create a setup program for PACC (WARNING : NSIS required, but not found)?" OFF)
	else(NSIS_FOUND MATCHES NSIS_FOUND-NOTFOUND)
		message(STATUS "++ NSIS installer creator found")
		option(PACC_CREATE_WIN32_SETUP "Create a setup program for PACC?" ON)
	endif(NSIS_FOUND MATCHES NSIS_FOUND-NOTFOUND)
endif(APPLE)

# Source packaging options
if(NOT DEFINED PACC_SOURCE_PACKAGE_TYPE)
	set(PACC_SOURCE_PACKAGE_TYPE "TGZ;ZIP" CACHE STRING "CPack source package type (TGZ, TZ, TBZ2, STGZ, ZIP)")
endif(NOT DEFINED PACC_SOURCE_PACKAGE_TYPE)

message(STATUS "++ System configuration detection...")

# Offer to disable any compilation warnings on Microsoft Visual C++
if (WIN32)
	if(MSVC AND NOT DEFINED PACC_MSVC_NOWARNINGS)
		option(PACC_MSVC_NOWARNINGS "Disable Visual C++ useless warnings?" ON)
	endif(MSVC AND NOT DEFINED PACC_MSVC_NOWARNINGS)
	if(PACC_LIBRARY_TYPE STREQUAL "SHARED")
		set(CMAKE_RUNTIME_OUTPUT_DIRECTORY lib)
	endif(PACC_LIBRARY_TYPE STREQUAL "SHARED")
endif (WIN32)

# Look for zlib
message(STATUS "++ Looking for ZLib library...")
find_package(ZLIB)

if (ZLIB_FOUND)
	message(STATUS "++ ZLib found...")
	option(PACC_ZLIB "Enable socket compression with ZLib?" ON)
else (ZLIB_FOUND)
	message(STATUS "++ Cannot find ZLib, socket compression disabled...")
endif (ZLIB_FOUND)

# Look for threads libraries
message(STATUS "++ Looking for threads libraries...")
include(FindThreads)
if(CMAKE_USE_WIN32_THREADS_INIT)
	# Windows environment
	message(STATUS "++ Using Windows threads..."  ${CMAKE_THREAD_LIBS_INIT})
	set(PACC_THREADS_WIN32 true)
elseif(CMAKE_USE_PTHREADS_INIT OR CMAKE_HP_PTHREADS_INIT)
	# POSIX environment
	message(STATUS "++ Using POSIX threads..." ${CMAKE_THREAD_LIBS_INIT})
	set(PACC_THREADS_POSIX true)
else(CMAKE_USE_WIN32_THREADS_INIT)
	# Non-supported environment
    message(SEND_ERROR "## Cannot find any thread library!")
endif(CMAKE_USE_WIN32_THREADS_INIT)

# If we are using a recent version of GCC, we can use OpenMP and Parallel version of STL algorithms (default : do not use)
if(CMAKE_COMPILER_IS_GNUCXX)
	execute_process(COMMAND gcc -dumpversion OUTPUT_VARIABLE CMAKE_CXX_COMPILER_VERSION)
	
	if(CMAKE_CXX_COMPILER_VERSION MATCHES "^3?\\.[0-9]?\\.[0-9]")
		# By the way, we check for very old versions of GCC (3.x branch)
		set(PACC_GCC_LESS_THAN_3 true)
		message("## Warning : your gcc version is very old and may not build correctly some packages.")
	endif(CMAKE_CXX_COMPILER_VERSION MATCHES "^3?\\.[0-9]?\\.[0-9]")
			
	if(CMAKE_CXX_COMPILER_VERSION MATCHES "4?\\.[3-9]?\\.[0-9]" AND NOT DEFINED PACC_USE_PARALLEL_STL)
		option(PACC_USE_PARALLEL_STL "Use parallel version of STL algorithms (OpenMP and GCC >= 4.3 needed)?" OFF)
	endif(CMAKE_CXX_COMPILER_VERSION MATCHES "4?\\.[3-9]?\\.[0-9]" AND NOT DEFINED PACC_USE_PARALLEL_STL)
endif(CMAKE_COMPILER_IS_GNUCXX)

if(UNIX)
    # Checking for some socket headers
    check_include_files("sys/types.h;sys/socket.h;netinet/in.h;arpa/inet.h;netdb.h;sys/errno.h;sys/time.h;netinet/tcp.h" TEST_SOCKET_UNIX)
    if(NOT TEST_SOCKET_UNIX)
	message(SEND_ERROR "## Cannot find some of the POSIX sockets headers!")
	message(STATUS "Make sure that the development tools of your system are correctly installed\nOn some Linux distributions, you can try a 'apt-get/rpm build-essential'\nOn MacOS, you should use the setup DVD to install development tools.")
    else(NOT TEST_SOCKET_UNIX)
	message(STATUS "++ Using POSIX sockets...")
	set(PACC_SOCKET_UNIX true)
    endif(NOT TEST_SOCKET_UNIX)
endif(UNIX)

if(WIN32 AND NOT CYGWIN)
	# Check for socket headers
	check_include_files("winsock2.h" TEST_SOCKET_WIN32)
	if(NOT TEST_SOCKET_WIN32)
		message(SEND_ERROR "## Cannot find required include file winsock2.h!")
		message(STATUS "\t This header file is required to build PACC:Socket. You can get it by downloading the Microsoft(c) Windows(c) SDK(c); you can retrieve it free of charges, for instance, at :")
		message(STATUS "\t http://microsoft.com/downloads/details.aspx?FamilyID=c17ba869-9671-4330-a63e-1fd44e0e2505")
		message(STATUS "\t Alternatively, you can install Microsoft(c) Visual(c) C++ Express, which includes those development headers and libraries.")
	endif(NOT TEST_SOCKET_WIN32)
    set(PACC_SOCKET_WIN32 true)
    message(STATUS "++ Using Win32 sockets...")
endif(WIN32 AND NOT CYGWIN)

message(STATUS "++ Adding includes paths...")

if (PACC_ZLIB)
    # Including ZLib headers
    message(STATUS "++ ZLib headers included... : ${ZLIB_LIBRAIRIES}")
    include_directories(${ZLIB_INCLUDE_DIR})
endif(PACC_ZLIB)

if(PACC_USE_PARALLEL_STL)
	find_package(OpenMP QUIET) 		# FindOpenMP macro is not available in all CMake configurations, so we try to use the "official" package
	mark_as_advanced(OpenMP_DIR) 		
	if(OpenMP_DIR MATCHES OpenMP_DIR-NOTFOUND)	# If CMake fails to locate FindOpenMP, we use our own macro
		include(CMakeMacros/FindOpenMP.cmake)
	endif(OpenMP_DIR MATCHES OpenMP_DIR-NOTFOUND)

	if(OPENMP_FOUND AND CMAKE_CXX_COMPILER_VERSION MATCHES "4?\\.[3-9]?\\.[0-9]")
		message(STATUS "++ OpenMP found...")
		message(STATUS "++ GLIBCXX_PARALLEL configuration option added...")
		add_definitions(-D_GLIBCXX_PARALLEL)
		set(PACC_PARALLEL_STL true)
	else(OPENMP_FOUND AND CMAKE_CXX_COMPILER_VERSION MATCHES "4?\\.[3-9]?\\.[0-9]")
		message("!! Warning : OpenMP not found; cannot use STL parallel algorithms without it.")
		set(PACC_PARALLEL_STL false)
	endif(OPENMP_FOUND AND CMAKE_CXX_COMPILER_VERSION MATCHES "4?\\.[3-9]?\\.[0-9]")
endif(PACC_USE_PARALLEL_STL)

message(STATUS "++ Looking for pacc sourcefiles...")
file(GLOB_RECURSE PACC_SOURCES src/PACC/*.cpp )		# Include all pacc sourcefiles and headers
file(GLOB_RECURSE PACC_ALL_HEADERS src/PACC/*.hpp)

# Include sourcefiles and headers by section
file(GLOB PACC_MATHS_SRCS	src/PACC/Math/*.cpp)
file(GLOB PACC_SOCKET_SRCS	src/PACC/Socket/*.cpp)
file(GLOB PACC_SVG_SRCS		src/PACC/SVG/*.cpp)
file(GLOB PACC_THREADING_SRCS	src/PACC/Threading/*.cpp)
file(GLOB PACC_UTILS_SRCS	src/PACC/Util/*.cpp)
file(GLOB PACC_XML_SRCS		src/PACC/XML/*.cpp)

file(GLOB PACC_MAIN_HEADER	src/PACC/PACC.hpp)
file(GLOB PACC_GEN_HEADERS	src/PACC/*.hpp )
file(GLOB PACC_Util_HEADERS	src/PACC/Util/*.hpp )
file(GLOB PACC_Threading_HEADERS	src/PACC/Threading/*.hpp )
file(GLOB PACC_Socket_HEADERS	src/PACC/Socket/*.hpp )
file(GLOB PACC_XML_HEADERS	src/PACC/XML/*.hpp )
file(GLOB PACC_Math_HEADERS	src/PACC/Math/*.hpp )
file(GLOB PACC_SVG_HEADERS	src/PACC/SVG/*.hpp )

message(STATUS "++ Configuration...")
# Writing results in configuration file
configure_file("${PROJECT_SOURCE_DIR}/src/PACC/config.hpp.in" "${CMAKE_CURRENT_BINARY_DIR}/include/PACC/config.hpp")
file(GLOB PACC_config_HEADER	"${CMAKE_CURRENT_BINARY_DIR}/include/PACC/config.hpp")
include_directories("${PROJECT_SOURCE_DIR}/src" "${CMAKE_CURRENT_BINARY_DIR}/include")

add_library(
	pacc
	${PACC_LIBRARY_TYPE}
	${PACC_SOURCES}
	${PACC_ALL_HEADERS}
	)

# Ensure that the binaries will be put in the lib directory
if(PACC_LIBRARY_TYPE STREQUAL "STATIC")
	set_target_properties(pacc PROPERTIES VERSION ${PACC_VERSION} ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib")
else(PACC_LIBRARY_TYPE STREQUAL "STATIC")
	if(WIN32)
		set_target_properties(pacc PROPERTIES VERSION ${PACC_VERSION} ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib")
	endif(WIN32)
	set_target_properties(pacc PROPERTIES VERSION ${LIBRARY_VERSION} LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib")
endif(PACC_LIBRARY_TYPE STREQUAL "STATIC")

# Linking libraries to target
target_link_libraries(pacc ${CMAKE_THREAD_LIBS_INIT})
if(PACC_ZLIB)
	target_link_libraries(pacc ${ZLIB_LIBRARIES})
endif(PACC_ZLIB)

# On Windows, we have to link to the socket library
if(PACC_SOCKET_WIN32)
	message(STATUS "++ Linking winsock2 library...")
	target_link_libraries(pacc WS2_32)
endif(PACC_SOCKET_WIN32)
if(PACC_MSVC_NOWARNINGS)
	message(STATUS "++ Disable Visual Studio compilation warnings...")
	add_definitions(/w)
endif(PACC_MSVC_NOWARNINGS)

# Installation; prefix can be set with PACC_INSTALL_PREFIX
install(TARGETS pacc 			DESTINATION 	lib)
install(FILES ${PACC_MAIN_HEADER}	DESTINATION	include)
install(FILES ${PACC_GEN_HEADERS} ${PACC_config_HEADER}	DESTINATION	include/PACC)
install(FILES ${PACC_Util_HEADERS}	DESTINATION	include/PACC/Util)
install(FILES ${PACC_Threading_HEADERS}	DESTINATION	include/PACC/Threading)
install(FILES ${PACC_Socket_HEADERS}	DESTINATION	include/PACC/Socket)
install(FILES ${PACC_XML_HEADERS}	DESTINATION	include/PACC/XML)
install(FILES ${PACC_Math_HEADERS}	DESTINATION	include/PACC/Math)
install(FILES ${PACC_SVG_HEADERS}	DESTINATION	include/PACC/SVG)

# Enable package creation
set(CPACK_PACKAGE_NAME "pacc")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/1-Readme.txt")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/2-License.txt")
set(CPACK_PACKAGE_VERSION ${PACC_VERSION})
set(CPACK_PACKAGE_CONTACT "marc.parizeau@gel.ulaval.ca")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "PACC is a collection of Portable Agile C++ Classes that can be retrieved at http://sourceforge.net/projects/pacc/. It is said to be portable because it can run on both Unix and Windows platforms. In general, the PACC classes have been tested and used under Linux, MacOSX, and Windows 2000/XP. They are also known to compile succesfully on AIX.")

# Binary packages options
if(PACC_CREATE_PKG)	# Create a .dmg
	set(CPACK_PACKAGE_FILE_NAME "pacc-${PACC_VERSION}")
	set(CPACK_GENERATOR "PackageMaker")
endif(PACC_CREATE_PKG)
if(PACC_CREATE_DEB) 	# Create a .deb
	message(STATUS "Creating .deb packages...")
	set(CPACK_GENERATOR "${CPACK_GENERATOR}" "DEB")
endif(PACC_CREATE_DEB)
if(PACC_CREATE_RPM)	# Create a .rpm
	message(STATUS "++ Warning : you need to have 'rpmbuild' installed in order to generate RPM files")
	set(CPACK_GENERATOR "${CPACK_GENERATOR}" "RPM")
endif(PACC_CREATE_RPM)
if(PACC_CREATE_UNIX_BIN)	# Create installer for UNIX (self extracting tgz, .sh)
	set(CPACK_GENERATOR "${CPACK_GENERATOR}" "STGZ")
endif(PACC_CREATE_UNIX_BIN)

if(WIN32 AND PACC_CREATE_WIN32_SETUP)	# Create Windows setup program (NSIS required)
	find_program(NSIS_FOUND NSIS PATHS "C:/Program Files/NSIS") 	# Try to locate NSIS
	if(NSIS_FOUND MATCHES NSIS_FOUND-NOTFOUND)
		message(STATUS "Warning : to pack this library into a Windows Installer executable, you will need NSIS.")
		message(STATUS " NSIS is not currently installed on your system, or its path is not defined.\n You can download it at http://nsis.sourceforge.net")
	else(NSIS_FOUND MATCHES NSIS_FOUND-NOTFOUND)
		message(STATUS "++ NSIS installer creator found")
	endif(NSIS_FOUND MATCHES NSIS_FOUND-NOTFOUND)
	set(CPACK_PACKAGE_INSTALL_DIRECTORY "PACC")
	#set(CPACK_PACKAGE_ICON "pacc_banner.bmp")
	include(InstallRequiredSystemLibraries)
endif(WIN32 AND PACC_CREATE_WIN32_SETUP)

# Source packages options
set(CPACK_SOURCE_GENERATOR ${PACC_SOURCE_PACKAGE_TYPE})
set(CPACK_SOURCE_IGNORE_FILES "/.svn/;${CMAKE_CURRENT_BINARY_DIR}")

include(CPack)
