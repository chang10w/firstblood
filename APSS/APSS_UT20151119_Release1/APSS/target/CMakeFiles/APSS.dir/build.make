# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.6

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canoncical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Produce verbose output by default.
VERBOSE = 1

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /root/U-TEST_RT/APSS

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/U-TEST_RT/APSS/target

# Include any dependencies generated for this target.
include CMakeFiles/APSS.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/APSS.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/APSS.dir/flags.make

CMakeFiles/APSS.dir/APSS.o: CMakeFiles/APSS.dir/flags.make
CMakeFiles/APSS.dir/APSS.o: ../APSS.cc
	$(CMAKE_COMMAND) -E cmake_progress_report /root/U-TEST_RT/APSS/target/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/APSS.dir/APSS.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/APSS.dir/APSS.o -c /root/U-TEST_RT/APSS/APSS.cc

CMakeFiles/APSS.dir/APSS.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/APSS.dir/APSS.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /root/U-TEST_RT/APSS/APSS.cc > CMakeFiles/APSS.dir/APSS.i

CMakeFiles/APSS.dir/APSS.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/APSS.dir/APSS.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /root/U-TEST_RT/APSS/APSS.cc -o CMakeFiles/APSS.dir/APSS.s

CMakeFiles/APSS.dir/APSS.o.requires:
.PHONY : CMakeFiles/APSS.dir/APSS.o.requires

CMakeFiles/APSS.dir/APSS.o.provides: CMakeFiles/APSS.dir/APSS.o.requires
	$(MAKE) -f CMakeFiles/APSS.dir/build.make CMakeFiles/APSS.dir/APSS.o.provides.build
.PHONY : CMakeFiles/APSS.dir/APSS.o.provides

CMakeFiles/APSS.dir/APSS.o.provides.build: CMakeFiles/APSS.dir/APSS.o
.PHONY : CMakeFiles/APSS.dir/APSS.o.provides.build

CMakeFiles/APSS.dir/ApsParaSvr.o: CMakeFiles/APSS.dir/flags.make
CMakeFiles/APSS.dir/ApsParaSvr.o: ../ApsParaSvr.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /root/U-TEST_RT/APSS/target/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/APSS.dir/ApsParaSvr.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/APSS.dir/ApsParaSvr.o -c /root/U-TEST_RT/APSS/ApsParaSvr.cpp

CMakeFiles/APSS.dir/ApsParaSvr.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/APSS.dir/ApsParaSvr.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /root/U-TEST_RT/APSS/ApsParaSvr.cpp > CMakeFiles/APSS.dir/ApsParaSvr.i

CMakeFiles/APSS.dir/ApsParaSvr.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/APSS.dir/ApsParaSvr.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /root/U-TEST_RT/APSS/ApsParaSvr.cpp -o CMakeFiles/APSS.dir/ApsParaSvr.s

CMakeFiles/APSS.dir/ApsParaSvr.o.requires:
.PHONY : CMakeFiles/APSS.dir/ApsParaSvr.o.requires

CMakeFiles/APSS.dir/ApsParaSvr.o.provides: CMakeFiles/APSS.dir/ApsParaSvr.o.requires
	$(MAKE) -f CMakeFiles/APSS.dir/build.make CMakeFiles/APSS.dir/ApsParaSvr.o.provides.build
.PHONY : CMakeFiles/APSS.dir/ApsParaSvr.o.provides

CMakeFiles/APSS.dir/ApsParaSvr.o.provides.build: CMakeFiles/APSS.dir/ApsParaSvr.o
.PHONY : CMakeFiles/APSS.dir/ApsParaSvr.o.provides.build

CMakeFiles/APSS.dir/ApsHDSvr.o: CMakeFiles/APSS.dir/flags.make
CMakeFiles/APSS.dir/ApsHDSvr.o: ../ApsHDSvr.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /root/U-TEST_RT/APSS/target/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/APSS.dir/ApsHDSvr.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/APSS.dir/ApsHDSvr.o -c /root/U-TEST_RT/APSS/ApsHDSvr.cpp

CMakeFiles/APSS.dir/ApsHDSvr.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/APSS.dir/ApsHDSvr.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /root/U-TEST_RT/APSS/ApsHDSvr.cpp > CMakeFiles/APSS.dir/ApsHDSvr.i

CMakeFiles/APSS.dir/ApsHDSvr.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/APSS.dir/ApsHDSvr.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /root/U-TEST_RT/APSS/ApsHDSvr.cpp -o CMakeFiles/APSS.dir/ApsHDSvr.s

CMakeFiles/APSS.dir/ApsHDSvr.o.requires:
.PHONY : CMakeFiles/APSS.dir/ApsHDSvr.o.requires

CMakeFiles/APSS.dir/ApsHDSvr.o.provides: CMakeFiles/APSS.dir/ApsHDSvr.o.requires
	$(MAKE) -f CMakeFiles/APSS.dir/build.make CMakeFiles/APSS.dir/ApsHDSvr.o.provides.build
.PHONY : CMakeFiles/APSS.dir/ApsHDSvr.o.provides

CMakeFiles/APSS.dir/ApsHDSvr.o.provides.build: CMakeFiles/APSS.dir/ApsHDSvr.o
.PHONY : CMakeFiles/APSS.dir/ApsHDSvr.o.provides.build

CMakeFiles/APSS.dir/ApsXmlOp.o: CMakeFiles/APSS.dir/flags.make
CMakeFiles/APSS.dir/ApsXmlOp.o: ../ApsXmlOp.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /root/U-TEST_RT/APSS/target/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/APSS.dir/ApsXmlOp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/APSS.dir/ApsXmlOp.o -c /root/U-TEST_RT/APSS/ApsXmlOp.cpp

CMakeFiles/APSS.dir/ApsXmlOp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/APSS.dir/ApsXmlOp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /root/U-TEST_RT/APSS/ApsXmlOp.cpp > CMakeFiles/APSS.dir/ApsXmlOp.i

CMakeFiles/APSS.dir/ApsXmlOp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/APSS.dir/ApsXmlOp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /root/U-TEST_RT/APSS/ApsXmlOp.cpp -o CMakeFiles/APSS.dir/ApsXmlOp.s

CMakeFiles/APSS.dir/ApsXmlOp.o.requires:
.PHONY : CMakeFiles/APSS.dir/ApsXmlOp.o.requires

CMakeFiles/APSS.dir/ApsXmlOp.o.provides: CMakeFiles/APSS.dir/ApsXmlOp.o.requires
	$(MAKE) -f CMakeFiles/APSS.dir/build.make CMakeFiles/APSS.dir/ApsXmlOp.o.provides.build
.PHONY : CMakeFiles/APSS.dir/ApsXmlOp.o.provides

CMakeFiles/APSS.dir/ApsXmlOp.o.provides.build: CMakeFiles/APSS.dir/ApsXmlOp.o
.PHONY : CMakeFiles/APSS.dir/ApsXmlOp.o.provides.build

CMakeFiles/APSS.dir/HDMessageHandle.o: CMakeFiles/APSS.dir/flags.make
CMakeFiles/APSS.dir/HDMessageHandle.o: ../HDMessageHandle.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /root/U-TEST_RT/APSS/target/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/APSS.dir/HDMessageHandle.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/APSS.dir/HDMessageHandle.o -c /root/U-TEST_RT/APSS/HDMessageHandle.cpp

CMakeFiles/APSS.dir/HDMessageHandle.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/APSS.dir/HDMessageHandle.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /root/U-TEST_RT/APSS/HDMessageHandle.cpp > CMakeFiles/APSS.dir/HDMessageHandle.i

CMakeFiles/APSS.dir/HDMessageHandle.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/APSS.dir/HDMessageHandle.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /root/U-TEST_RT/APSS/HDMessageHandle.cpp -o CMakeFiles/APSS.dir/HDMessageHandle.s

CMakeFiles/APSS.dir/HDMessageHandle.o.requires:
.PHONY : CMakeFiles/APSS.dir/HDMessageHandle.o.requires

CMakeFiles/APSS.dir/HDMessageHandle.o.provides: CMakeFiles/APSS.dir/HDMessageHandle.o.requires
	$(MAKE) -f CMakeFiles/APSS.dir/build.make CMakeFiles/APSS.dir/HDMessageHandle.o.provides.build
.PHONY : CMakeFiles/APSS.dir/HDMessageHandle.o.provides

CMakeFiles/APSS.dir/HDMessageHandle.o.provides.build: CMakeFiles/APSS.dir/HDMessageHandle.o
.PHONY : CMakeFiles/APSS.dir/HDMessageHandle.o.provides.build

CMakeFiles/APSS.dir/CommonUtility.o: CMakeFiles/APSS.dir/flags.make
CMakeFiles/APSS.dir/CommonUtility.o: ../CommonUtility.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /root/U-TEST_RT/APSS/target/CMakeFiles $(CMAKE_PROGRESS_6)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/APSS.dir/CommonUtility.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/APSS.dir/CommonUtility.o -c /root/U-TEST_RT/APSS/CommonUtility.cpp

CMakeFiles/APSS.dir/CommonUtility.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/APSS.dir/CommonUtility.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /root/U-TEST_RT/APSS/CommonUtility.cpp > CMakeFiles/APSS.dir/CommonUtility.i

CMakeFiles/APSS.dir/CommonUtility.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/APSS.dir/CommonUtility.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /root/U-TEST_RT/APSS/CommonUtility.cpp -o CMakeFiles/APSS.dir/CommonUtility.s

CMakeFiles/APSS.dir/CommonUtility.o.requires:
.PHONY : CMakeFiles/APSS.dir/CommonUtility.o.requires

CMakeFiles/APSS.dir/CommonUtility.o.provides: CMakeFiles/APSS.dir/CommonUtility.o.requires
	$(MAKE) -f CMakeFiles/APSS.dir/build.make CMakeFiles/APSS.dir/CommonUtility.o.provides.build
.PHONY : CMakeFiles/APSS.dir/CommonUtility.o.provides

CMakeFiles/APSS.dir/CommonUtility.o.provides.build: CMakeFiles/APSS.dir/CommonUtility.o
.PHONY : CMakeFiles/APSS.dir/CommonUtility.o.provides.build

# Object files for target APSS
APSS_OBJECTS = \
"CMakeFiles/APSS.dir/APSS.o" \
"CMakeFiles/APSS.dir/ApsParaSvr.o" \
"CMakeFiles/APSS.dir/ApsHDSvr.o" \
"CMakeFiles/APSS.dir/ApsXmlOp.o" \
"CMakeFiles/APSS.dir/HDMessageHandle.o" \
"CMakeFiles/APSS.dir/CommonUtility.o"

# External object files for target APSS
APSS_EXTERNAL_OBJECTS =

libAPSS.so: CMakeFiles/APSS.dir/APSS.o
libAPSS.so: CMakeFiles/APSS.dir/ApsParaSvr.o
libAPSS.so: CMakeFiles/APSS.dir/ApsHDSvr.o
libAPSS.so: CMakeFiles/APSS.dir/ApsXmlOp.o
libAPSS.so: CMakeFiles/APSS.dir/HDMessageHandle.o
libAPSS.so: CMakeFiles/APSS.dir/CommonUtility.o
libAPSS.so: CMakeFiles/APSS.dir/build.make
libAPSS.so: CMakeFiles/APSS.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX shared library libAPSS.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/APSS.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/APSS.dir/build: libAPSS.so
.PHONY : CMakeFiles/APSS.dir/build

CMakeFiles/APSS.dir/requires: CMakeFiles/APSS.dir/APSS.o.requires
CMakeFiles/APSS.dir/requires: CMakeFiles/APSS.dir/ApsParaSvr.o.requires
CMakeFiles/APSS.dir/requires: CMakeFiles/APSS.dir/ApsHDSvr.o.requires
CMakeFiles/APSS.dir/requires: CMakeFiles/APSS.dir/ApsXmlOp.o.requires
CMakeFiles/APSS.dir/requires: CMakeFiles/APSS.dir/HDMessageHandle.o.requires
CMakeFiles/APSS.dir/requires: CMakeFiles/APSS.dir/CommonUtility.o.requires
.PHONY : CMakeFiles/APSS.dir/requires

CMakeFiles/APSS.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/APSS.dir/cmake_clean.cmake
.PHONY : CMakeFiles/APSS.dir/clean

CMakeFiles/APSS.dir/depend:
	cd /root/U-TEST_RT/APSS/target && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/U-TEST_RT/APSS /root/U-TEST_RT/APSS /root/U-TEST_RT/APSS/target /root/U-TEST_RT/APSS/target /root/U-TEST_RT/APSS/target/CMakeFiles/APSS.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/APSS.dir/depend
