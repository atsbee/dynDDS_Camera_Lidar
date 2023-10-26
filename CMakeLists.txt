cmake_minimum_required(VERSION 3.16.3)

project(ScanDynamicProject VERSION 1 LANGUAGES CXX)

# Find requirements
if(NOT fastcdr_FOUND)
    find_package(fastcdr REQUIRED)
endif()

if(NOT fastrtps_FOUND)
    find_package(fastrtps REQUIRED)
endif()

# # Check C++11
# include(CheckCXXCompilerFlag)
# if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
#     check_cxx_compiler_flag(-std=c++11 SUPPORTS_CXX11)
#     check_cxx_compiler_flag(-std=c++11 SUPPORTS_CXX11)
#     if(NOT SUPPORTS_CXX11)
#         message(FATAL_ERROR "Compiler doesn't support C++11")
#     endif()
# endif()

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

# Use GLOB to collect source files
file(GLOB PUB_SRC
    "dynddspub/ScanDynamicPub.cpp"
    "dynddspub/putData.cpp"
)

file(GLOB SUB_SRC
    "dynddssub/ScanDynamicSub.cpp"
)

message(STATUS "Configuring ScanDynamic example...")

# Dummy Lidar publisher
add_executable(dummyLidarPub dummyPub/dummyLidarPub.cpp ${PUB_SRC})
target_link_libraries(dummyLidarPub fastrtps fastcdr)

# Dummy Camera angles publisher
add_executable(dummyCameraAnglePub dummyPub/dummyCameraAnglePub.cpp ${PUB_SRC})
target_link_libraries(dummyCameraAnglePub fastrtps fastcdr)

# Dummy Camera publisher
add_executable(dummyCameraPub dummyPub/dummyCameraPub.cpp ${PUB_SRC})
target_link_libraries(dummyCameraPub fastrtps fastcdr)

# Lidar publisher
add_executable(LidarDynamicPub lidarPub/LidarDynamicPub.cpp ${PUB_SRC})
target_link_libraries(LidarDynamicPub fastrtps fastcdr sl_lidar_sdk)

# # Camera angles publisher
# add_executable(CameraAnglePub cameraAnglesPub/cameraAnglesDynamicPub.cpp ${PUB_SRC})
# target_link_libraries(CameraAnglePub fastrtps fastcdr aditof_sdk)

# # Camera publisher
# add_executable(CameraDynamicPub cameraPub/cameraDynamicPub.cpp ${PUB_SRC})
# target_link_libraries(CameraDynamicPub fastrtps fastcdr aditof_sdk)

# Visualization subscriber
add_executable(visumainSub visuSub/visuDynamicSub.cpp ${SUB_SRC} visuSub/visualization.cpp)
target_link_libraries(visumainSub fastrtps fastcdr sfml-graphics sfml-window sfml-system)

# Camera subscriber
find_package (OpenCV REQUIRED)
include_directories (${OpenCV_INCLUDE_DIRS})
add_executable(cameraSub cameraSub/cameraDynamicSub.cpp ${SUB_SRC})
target_link_libraries(cameraSub fastrtps fastcdr ${OpenCV_LIBS})





# ramtest pub
add_executable(ramonePub ramtestPub/ramonePub.cpp ${PUB_SRC})
target_link_libraries(ramonePub fastrtps fastcdr)

# ramtest sub
add_executable(ramoneSub ramtestPub/ramoneSub.cpp ${SUB_SRC})
target_link_libraries(ramoneSub fastrtps fastcdr)

# ramtest pub
add_executable(ramtenPub ramtestPub/ramtenPub.cpp ${PUB_SRC})
target_link_libraries(ramtenPub fastrtps fastcdr)

# ramtest sub
add_executable(ramtenSub ramtestPub/ramtenSub.cpp ${SUB_SRC})
target_link_libraries(ramtenSub fastrtps fastcdr)