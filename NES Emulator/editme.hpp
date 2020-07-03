#pragma once

// You are free to edit anything in here!

///////////////////////////////////////////////////////////
////////////// PRINT INSTRUCTIONS TO CONSOLE //////////////
///////////////////////////////////////////////////////////
// This will print a number of infos about the currently 
// executed instructions to the console.
//
#define PRINT_INSTRUCTIONS
//
///////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////
//////////////////// NESTEST CPU ONLY /////////////////////
///////////////////////////////////////////////////////////
// If enabled, this forces the program counter to start
// at 0xC000. DO NOT ENABLE THIS IF YOU WANT TO EXECUTE
// NORMAL ROMs. Nestest uses 0xC000 as an entrypoint for
// CPU-only tests.
//
// #define NESTEST_CPU_ONLY
//
///////////////////////////////////////////////////////////