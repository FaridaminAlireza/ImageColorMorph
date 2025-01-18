cmake_minimum_required(VERSION 3.16)

# Set the project name
project(CSVProcessor)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the include directory
include_directories(include)

include_directories(/usr/local/opt/opencv/include/opencv4/)

find_package(OpenCV REQUIRED)

# Print the OpenCV libraries being linked
message(STATUS "OpenCV libraries: ${OpenCV_LIBS}")

# Add the executable
add_executable(STATS src/color_stat.cpp)
add_executable(COLOR_MORPH src/color_morph.cpp)

# Link against OpenCV libraries
target_link_libraries(STATS ${OpenCV_LIBS})
target_link_libraries(COLOR_MORPH ${OpenCV_LIBS})

# set_target_properties(MAKEVIDEO PROPERTIES
#     MACOSX_BUNDLE TRUE
#     MACOSX_BUNDLE_BUNDLE_NAME "MyMacApp"
#     MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0"
#     MACOSX_BUNDLE_BUNDLE_VERSION "1.0"
# )