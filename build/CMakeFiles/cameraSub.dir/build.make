# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/jamesb/Desktop/gitter/dynDDS_Camera_Lidar

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jamesb/Desktop/gitter/dynDDS_Camera_Lidar/build

# Include any dependencies generated for this target.
include CMakeFiles/cameraSub.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/cameraSub.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/cameraSub.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/cameraSub.dir/flags.make

CMakeFiles/cameraSub.dir/cameraSub/cameraDynamicSub.cpp.o: CMakeFiles/cameraSub.dir/flags.make
CMakeFiles/cameraSub.dir/cameraSub/cameraDynamicSub.cpp.o: ../cameraSub/cameraDynamicSub.cpp
CMakeFiles/cameraSub.dir/cameraSub/cameraDynamicSub.cpp.o: CMakeFiles/cameraSub.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jamesb/Desktop/gitter/dynDDS_Camera_Lidar/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/cameraSub.dir/cameraSub/cameraDynamicSub.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/cameraSub.dir/cameraSub/cameraDynamicSub.cpp.o -MF CMakeFiles/cameraSub.dir/cameraSub/cameraDynamicSub.cpp.o.d -o CMakeFiles/cameraSub.dir/cameraSub/cameraDynamicSub.cpp.o -c /home/jamesb/Desktop/gitter/dynDDS_Camera_Lidar/cameraSub/cameraDynamicSub.cpp

CMakeFiles/cameraSub.dir/cameraSub/cameraDynamicSub.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/cameraSub.dir/cameraSub/cameraDynamicSub.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jamesb/Desktop/gitter/dynDDS_Camera_Lidar/cameraSub/cameraDynamicSub.cpp > CMakeFiles/cameraSub.dir/cameraSub/cameraDynamicSub.cpp.i

CMakeFiles/cameraSub.dir/cameraSub/cameraDynamicSub.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/cameraSub.dir/cameraSub/cameraDynamicSub.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jamesb/Desktop/gitter/dynDDS_Camera_Lidar/cameraSub/cameraDynamicSub.cpp -o CMakeFiles/cameraSub.dir/cameraSub/cameraDynamicSub.cpp.s

CMakeFiles/cameraSub.dir/dynddssub/ScanDynamicSub.cpp.o: CMakeFiles/cameraSub.dir/flags.make
CMakeFiles/cameraSub.dir/dynddssub/ScanDynamicSub.cpp.o: ../dynddssub/ScanDynamicSub.cpp
CMakeFiles/cameraSub.dir/dynddssub/ScanDynamicSub.cpp.o: CMakeFiles/cameraSub.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jamesb/Desktop/gitter/dynDDS_Camera_Lidar/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/cameraSub.dir/dynddssub/ScanDynamicSub.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/cameraSub.dir/dynddssub/ScanDynamicSub.cpp.o -MF CMakeFiles/cameraSub.dir/dynddssub/ScanDynamicSub.cpp.o.d -o CMakeFiles/cameraSub.dir/dynddssub/ScanDynamicSub.cpp.o -c /home/jamesb/Desktop/gitter/dynDDS_Camera_Lidar/dynddssub/ScanDynamicSub.cpp

CMakeFiles/cameraSub.dir/dynddssub/ScanDynamicSub.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/cameraSub.dir/dynddssub/ScanDynamicSub.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jamesb/Desktop/gitter/dynDDS_Camera_Lidar/dynddssub/ScanDynamicSub.cpp > CMakeFiles/cameraSub.dir/dynddssub/ScanDynamicSub.cpp.i

CMakeFiles/cameraSub.dir/dynddssub/ScanDynamicSub.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/cameraSub.dir/dynddssub/ScanDynamicSub.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jamesb/Desktop/gitter/dynDDS_Camera_Lidar/dynddssub/ScanDynamicSub.cpp -o CMakeFiles/cameraSub.dir/dynddssub/ScanDynamicSub.cpp.s

# Object files for target cameraSub
cameraSub_OBJECTS = \
"CMakeFiles/cameraSub.dir/cameraSub/cameraDynamicSub.cpp.o" \
"CMakeFiles/cameraSub.dir/dynddssub/ScanDynamicSub.cpp.o"

# External object files for target cameraSub
cameraSub_EXTERNAL_OBJECTS =

cameraSub: CMakeFiles/cameraSub.dir/cameraSub/cameraDynamicSub.cpp.o
cameraSub: CMakeFiles/cameraSub.dir/dynddssub/ScanDynamicSub.cpp.o
cameraSub: CMakeFiles/cameraSub.dir/build.make
cameraSub: /usr/local/lib/libfastrtps.so.2.10.0
cameraSub: /usr/local/lib/libfastcdr.a
cameraSub: /usr/local/lib/libopencv_gapi.so.4.8.1
cameraSub: /usr/local/lib/libopencv_highgui.so.4.8.1
cameraSub: /usr/local/lib/libopencv_ml.so.4.8.1
cameraSub: /usr/local/lib/libopencv_objdetect.so.4.8.1
cameraSub: /usr/local/lib/libopencv_photo.so.4.8.1
cameraSub: /usr/local/lib/libopencv_stitching.so.4.8.1
cameraSub: /usr/local/lib/libopencv_video.so.4.8.1
cameraSub: /usr/local/lib/libopencv_videoio.so.4.8.1
cameraSub: /usr/local/lib/libfoonathan_memory-0.7.3.a
cameraSub: /usr/lib/x86_64-linux-gnu/libtinyxml2.so
cameraSub: /usr/lib/x86_64-linux-gnu/libtinyxml2.so
cameraSub: /usr/lib/x86_64-linux-gnu/libssl.so
cameraSub: /usr/lib/x86_64-linux-gnu/libcrypto.so
cameraSub: /usr/local/lib/libopencv_imgcodecs.so.4.8.1
cameraSub: /usr/local/lib/libopencv_dnn.so.4.8.1
cameraSub: /usr/local/lib/libopencv_calib3d.so.4.8.1
cameraSub: /usr/local/lib/libopencv_features2d.so.4.8.1
cameraSub: /usr/local/lib/libopencv_flann.so.4.8.1
cameraSub: /usr/local/lib/libopencv_imgproc.so.4.8.1
cameraSub: /usr/local/lib/libopencv_core.so.4.8.1
cameraSub: CMakeFiles/cameraSub.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jamesb/Desktop/gitter/dynDDS_Camera_Lidar/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable cameraSub"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cameraSub.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/cameraSub.dir/build: cameraSub
.PHONY : CMakeFiles/cameraSub.dir/build

CMakeFiles/cameraSub.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/cameraSub.dir/cmake_clean.cmake
.PHONY : CMakeFiles/cameraSub.dir/clean

CMakeFiles/cameraSub.dir/depend:
	cd /home/jamesb/Desktop/gitter/dynDDS_Camera_Lidar/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jamesb/Desktop/gitter/dynDDS_Camera_Lidar /home/jamesb/Desktop/gitter/dynDDS_Camera_Lidar /home/jamesb/Desktop/gitter/dynDDS_Camera_Lidar/build /home/jamesb/Desktop/gitter/dynDDS_Camera_Lidar/build /home/jamesb/Desktop/gitter/dynDDS_Camera_Lidar/build/CMakeFiles/cameraSub.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/cameraSub.dir/depend
