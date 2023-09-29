cmake_minimum_required(VERSION 3.16.3)

project(ScanDynamicProject VERSION 1 LANGUAGES CXX)

# Find requirements
if(NOT fastcdr_FOUND)
    find_package(fastcdr REQUIRED)
endif()

if(NOT fastrtps_FOUND)
    find_package(fastrtps REQUIRED)
endif()

# Check C++11
include(CheckCXXCompilerFlag)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    check_cxx_compiler_flag(-std=c++11 SUPPORTS_CXX11)
    check_cxx_compiler_flag(-std=c++11 SUPPORTS_CXX11)
    if(NOT SUPPORTS_CXX11)
        message(FATAL_ERROR "Compiler doesn't support C++11")
    endif()
endif()

# Check C++17
include(CheckCXXCompilerFlag)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    check_cxx_compiler_flag(-std=c++17 SUPPORTS_CXX17)
    check_cxx_compiler_flag(-std=c++17 SUPPORTS_CXX17)
    if(NOT SUPPORTS_CXX17)
        message(FATAL_ERROR "Compiler doesn't support C++17")
    endif()
endif()
#set c++17 as standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



message(STATUS "Configuring ScanDynamic example...")



# # dummy publisher
# add_executable(ScanDynamicPub1 ../dummmyPub/mainDynamicPub1.cpp dynddspub/ScanDynamicPub.cpp)
# target_link_libraries(ScanDynamicPub1 fastrtps fastcdr)
# add_executable(ScanDynamicPub2 mainDynamicPub2.cpp ../dynddspub/ScanDynamicPub.cpp)
# target_link_libraries(ScanDynamicPub2 fastrtps fastcdr)

# # Lidar publisher
# add_executable(LidarDynamicPub ../lidarPub/LidarDynamicPub.cpp ../dynddspub/ScanDynamicPub.cpp)
# target_link_libraries(LidarDynamicPub fastrtps fastcdr sl_lidar_sdk)

# # visualization subscriber
# add_executable(visumainSub visuDynamicSub.cpp ScanDynamicSub.cpp visualization.cpp)
# target_link_libraries(visumainSub fastrtps fastcdr sfml-graphics sfml-window sfml-system)

# Dummy publisher
add_executable(ScanDynamicPub1 dummyPub/mainDynamicPub1.cpp dynddspub/ScanDynamicPub.cpp)
target_link_libraries(ScanDynamicPub1 fastrtps fastcdr)

add_executable(ScanDynamicPub2 dummyPub/mainDynamicPub2.cpp dynddspub/ScanDynamicPub.cpp)
target_link_libraries(ScanDynamicPub2 fastrtps fastcdr)

add_executable(dummyCameraAnglePub dummyPub/dummyCameraAnglePub.cpp dynddspub/ScanDynamicPub.cpp)
target_link_libraries(dummyCameraAnglePub fastrtps fastcdr)

# Lidar publisher
add_executable(LidarDynamicPub lidarPub/LidarDynamicPub.cpp dynddspub/ScanDynamicPub.cpp)
target_link_libraries(LidarDynamicPub fastrtps fastcdr sl_lidar_sdk)

# Visualization subscriber
add_executable(visumainSub visuSub/visuDynamicSub.cpp dynddssub/ScanDynamicSub.cpp visuSub/visualization.cpp)
target_link_libraries(visumainSub fastrtps fastcdr sfml-graphics sfml-window sfml-system)

# # Camera subscriber
# find_package (OpenCV REQUIRED)
# include_directories (${OpenCV_INCLUDE_DIRS})
# add_executable(cameraSub cameraSub/cameraDynamicSub.cpp dynddssub/ScanDynamicSub.cpp)
# target_link_libraries(cameraSub fastrtps fastcdr ${OpenCV_LIBS})

# dummy camera publisher
add_executable(dummyCameraPub dummyPub/dummyCameraPub.cpp dynddspub/ScanDynamicPub.cpp)
target_link_libraries(dummyCameraPub fastrtps fastcdr)