# CMakeList.txt : CMake project for NES Emulator, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (nesemu "NES Emulator.cpp"
	"NESWindow.hpp"
 "hardware/Bus.cpp" "hardware/Mos6502.cpp")

target_include_directories (nesemu PRIVATE
	hardware
	${PROJECT_SOURCE_DIR}/SDLFramework/3rdparty/include/SDL
	${PROJECT_SOURCE_DIR}/SDLFramework/src/sdlf
)

target_link_libraries (nesemu PRIVATE
	${PROJECT_SOURCE_DIR}/SDLFramework/3rdparty/lib/SDL2.lib
	sdlf
)

add_custom_command (TARGET nesemu POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/SDLFramework/3rdparty/lib/SDL2.dll $<TARGET_FILE_DIR:nesemu>
)