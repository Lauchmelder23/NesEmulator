#pragma once

#include "../util.hpp"

#include "Mapper.hpp"

#define MIRROR_VERTICAL 0x00
#define MIRROR_HORIZONTAL 0x01
#define MIRROR_ONESCREEN 0x02
#define MIRROR_FOURSCREEN 0x03

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

	Mapper* UsedMapper;
	uint8_t UsedMirroring;

private:
	BYTE* m_pPRGMemory;
	BYTE* m_pCHRMemory;

	uint8_t m_uMapperID;
	uint8_t m_uPRGBanks;
	uint8_t m_uCHRBanks;

public:
	bool ReadCPU(WORD address, BYTE& value);
	bool WriteCPU(WORD address, BYTE value);

	bool ReadPPU(WORD address, BYTE& value);
	bool WritePPU(WORD address, BYTE value);
};