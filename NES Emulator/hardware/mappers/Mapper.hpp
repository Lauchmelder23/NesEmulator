#pragma once

#include "../../util.hpp"

class Mapper
{
public:
	Mapper(BYTE prgBanks, BYTE chrBanks) :
		m_uPrgBanks(prgBanks), m_uChrBanks(chrBanks)
	{ }
	~Mapper() = default;

public:
	virtual bool MappedReadCPU(WORD address, WORD& mappedAddress) = 0;
	virtual bool MappedWriteCPU(WORD address, WORD& mappedAddress) = 0;
	virtual bool MappedReadPPU(WORD address, WORD& mappedAddress) = 0;
	virtual bool MappedWritePPU(WORD address, WORD& mappedAddress) = 0;

protected:
	BYTE m_uPrgBanks, m_uChrBanks;
};