#pragma once

#include "Mapper.hpp"

class Mapper_000 : public Mapper
{
public:
	Mapper_000(BYTE prgBanks, BYTE chrBanks) : Mapper(prgBanks, chrBanks) {}
	~Mapper_000() = default;

public:
	bool MappedReadCPU(WORD address, DWORD& mappedAddress) override
	{
		// Only interested in 0x8000 - 0xFFFF
		if (IS_IN_RANGE(address, 0x8000, 0xFFFF))
		{
			mappedAddress = address & ((m_uPrgBanks == 1) ? 0x3FFF : 0x7FFF);
			return true;
		}

		return false;
	}

	bool MappedWriteCPU(WORD address, DWORD& mappedAddress) override
	{
		// Only interested in 0x8000 - 0xFFFF
		if (IS_IN_RANGE(address, 0x8000, 0xFFFF))
		{
			mappedAddress = address & ((m_uPrgBanks == 1) ? 0x3FFF : 0x7FFF);
			return true;
		}

		return false;
	}

	bool MappedReadPPU(WORD address, DWORD& mappedAddress) override
	{
		// Only interested in 0x0000 - 0x1FFF
		if (IS_IN_RANGE(address, 0x0000, 0x1FFF))
		{
			mappedAddress = address;
			return true;
		}

		return false;
	}

	bool MappedWritePPU(WORD address, DWORD& mappedAddress) override
	{
		// Nothing
		return false;
	}
};