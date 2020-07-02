# CMakeList.txt : CMake project for NES Emulator, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (nesemu "NES Emulator.cpp"
	"NESWindow.hpp" "NESWindow.cpp"
 "hardware/Bus.cpp" "hardware/Mos6502.cpp"
 "hardware/RP2C02.cpp" "hardware/Cartridge.cpp"
 "editme.hpp")

target_include_directories (nesemu PRIVATE
	hardware
	hardware/mappers
	${SDL2_INCLUDE_DIRS}
	${PROJECT_SOURCE_DIR}/SDLFramework/src/sdlf
)

target_link_libraries (nesemu PRIVATE
	${SDL2_LIBRARIES}
	sdlf
)

if(WIN32)
add_custom_command (TARGET nesemu POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/SDLFramework/3rdparty/lib/SDL2.dll $<TARGET_FILE_DIR:nesemu>
)
endif(WIN32)

add_custom_command (TARGET nesemu POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/NES Emulator/resources/nestest.nes" $<TARGET_FILE_DIR:nesemu>
)

install(TARGETS nesemu RUNTIME DESTINATION bin)

install(
	FILES "${PROJECT_SOURCE_DIR}/NES Emulator/resources/nestest.nes"
	DESTINATION bin
)

if(WIN32)
	install(FILES ${PROJECT_SOURCE_DIR}/SDLFramework/3rdparty/lib/SDL2.dll DESTINATION bin)
endif(WIN32)