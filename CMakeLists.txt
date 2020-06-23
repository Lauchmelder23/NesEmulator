# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 20)

project ("NES Emulator")

# Include sub-projects.
add_subdirectory ("SDLFramework")
add_subdirectory ("NES Emulator")
