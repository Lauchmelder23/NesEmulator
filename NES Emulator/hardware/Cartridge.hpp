#pragma once

#include "../util.hpp"

#include "Mapper_000.hpp"

class Cartridge
{
public:

	typedef struct sHeader
	{
		BYTE Name[4];	// Always "NES."
		BYTE PRGsize;	// PRG ROM size in 16KB units
		BYTE CHRsize;	// CHR ROM size in 8 KB units (0 -> board uses CHR RAM)
		BYTE MapperFlag1;
		BYTE MapperFlag2;
		BYTE PRGRAMsize;
		BYTE TVsystem1;
		BYTE TVsystem2;
		BYTE Unused[5];
	} Header;

public:
	Cartridge(const char* filename);
	~Cartridge();

private:
	BYTE* m_pPRGMemory;
	BYTE* m_pCHRMemory;

	uint8_t m_uMapperID;
	uint8_t m_uPRGBanks;
	uint8_t m_uCHRBanks;

	Mapper* m_pUsedMapper;

public:
	bool ReadCPU(WORD address, BYTE& value);
	bool WriteCPU(WORD address, BYTE value);

	bool ReadPPU(WORD address, BYTE& value);
	bool WritePPU(WORD address, BYTE value);
};