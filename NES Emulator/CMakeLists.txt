# CMakeList.txt : CMake project for NES Emulator, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (nesemu "NES Emulator.cpp"
	"NESWindow.hpp" "NESWindow.cpp"
 "hardware/Bus.cpp" "hardware/Mos6502.cpp")

target_include_directories (nesemu PRIVATE
	hardware
	${SDL2_INCLUDE_DIRS}
	${SDL2TTF_INCLUDE_DIR}
	${PROJECT_SOURCE_DIR}/SDLFramework/src/sdlf
)

target_link_libraries (nesemu PRIVATE
	${SDL2_LIBRARIES}
	${SDL2_TTF_LIBRARY}
	sdlf
)

if(WIN32)
add_custom_command (TARGET nesemu POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/SDLFramework/3rdparty/lib/SDL2.dll $<TARGET_FILE_DIR:nesemu>
	COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/NES Emulator/3rdparty/lib/libfreetype-6.dll" $<TARGET_FILE_DIR:nesemu>
	COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/NES Emulator/3rdparty/lib/SDL2_ttf.dll" $<TARGET_FILE_DIR:nesemu>
	COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/NES Emulator/3rdparty/lib/zlib1.dll" $<TARGET_FILE_DIR:nesemu>
)
endif(WIN32)

add_custom_command (TARGET nesemu POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/NES Emulator/resources/PressStart2p.ttf" $<TARGET_FILE_DIR:nesemu>
	COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/NES Emulator/resources/nestest.nes" $<TARGET_FILE_DIR:nesemu>
)