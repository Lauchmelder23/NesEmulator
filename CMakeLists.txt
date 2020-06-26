# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 20)

if(WIN32)
	set(SDL2_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/SDLFramework/3rdparty/include/SDL)
	set(SDL2_LIBRARIES 
		${CMAKE_SOURCE_DIR}/SDLFramework/3rdparty/lib/SDL2.lib 
		${CMAKE_SOURCE_DIR}/SDLFramework/3rdparty/lib/SDL2main.lib
	)

	set(SDL2_TTF_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/NES Emulator/3rdparty/include")
	set(SDL2_TTF_LIBRARIES 
		"${CMAKE_SOURCE_DIR}/NES Emulator/3rdparty/lib/SDL2_ttf.lib"
	)
else()
	find_package(SDL2 REQUIRED)
	find_package(SDL2TTF REQUIRED)
endif(WIN32)

project ("NES Emulator")

# Include sub-projects.
add_subdirectory ("SDLFramework")
add_subdirectory ("NES Emulator")
