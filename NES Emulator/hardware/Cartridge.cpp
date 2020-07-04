#include "Cartridge.hpp"

#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>

Cartridge::Cartridge(const char* filename) :
	m_pCHRMemory(nullptr), m_pPRGMemory(nullptr), m_pUsedMapper(nullptr)
{
	Header header;

	std::ifstream ifs(filename, std::ios::binary);
	if (!ifs.is_open())
	{
		std::stringstream ss;
		ss << "Failed to open file " << filename << ": " << std::strerror(errno);
		throw ss.str().c_str();
	}

	ifs.read(reinterpret_cast<char*>(&header), sizeof(Header));	// Naughty

	// I dont care about trainer data
	if (BIT_(2, header.MapperFlag1))
		ifs.seekg(512, std::ios_base::cur);

	m_uMapperID = ((header.MapperFlag1 & 0xF0) >> 4) | (header.MapperFlag2 & 0xF0);

	// Read PRG memory
	m_uPRGBanks = header.PRGsize;
	m_pPRGMemory = new BYTE[0x4000 * (uint64_t)m_uPRGBanks];
	ifs.read(reinterpret_cast<char*>(m_pPRGMemory), 0x4000 * (uint64_t)m_uPRGBanks);

	// Read CHR memory
	m_uCHRBanks = header.CHRsize;
	m_pCHRMemory = new BYTE[0x2000 * (uint64_t)m_uCHRBanks];
	ifs.read(reinterpret_cast<char*>(m_pCHRMemory), 0x2000 * (uint64_t)m_uCHRBanks);

	switch (m_uMapperID)
	{
	case 0: m_pUsedMapper = new Mapper_000(m_uPRGBanks, m_uCHRBanks); break;
	}

	if (m_pUsedMapper == nullptr)
	{
		std::stringstream ss;
		ss << "This ROM uses a Mapper that this emulator doesn't (yet) support! Mapper ID is: " << (WORD)m_uMapperID;
		throw ss.str().c_str();
	}

	ifs.close();
}

Cartridge::~Cartridge()
{
	delete[] m_pCHRMemory;
	m_pCHRMemory = nullptr;

	delete[] m_pPRGMemory;
	m_pPRGMemory = nullptr;
}

bool Cartridge::ReadCPU(WORD address, BYTE& value)
{
	WORD mappedAddress = 0;
	if (m_pUsedMapper->MappedReadCPU(address, mappedAddress))
	{
		value = m_pPRGMemory[mappedAddress];
		return true;
	}

	return false;
}

bool Cartridge::WriteCPU(WORD address, BYTE value)
{
	WORD mappedAddress = 0;
	if (m_pUsedMapper->MappedWriteCPU(address, mappedAddress))
	{
		m_pPRGMemory[mappedAddress] = value;
		return true;
	}

	return false;
}

bool Cartridge::ReadPPU(WORD address, BYTE& value)
{
	WORD mappedAddress = 0;
	if (m_pUsedMapper->MappedReadPPU(address, mappedAddress))
	{
		value = m_pCHRMemory[mappedAddress];
		return true;
	}

	return false;
}

bool Cartridge::WritePPU(WORD address, BYTE value)
{
	WORD mappedAddress = 0;
	if (m_pUsedMapper->MappedWritePPU(address, mappedAddress))
	{
		m_pCHRMemory[mappedAddress] = value;
		return true;
	}

	return false;
}
