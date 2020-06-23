#include "Mos6502.hpp"
#include "Bus.hpp"

#define MAKE(x, y, z) {std::bind(&Mos6502::x, this), std::bind(&Mos6502::y, this), z}

Mos6502::Mos6502() :
	m_pBus(nullptr), m_oStatus(),
	m_uAcc(0x00), m_uX(0x00), m_uY(0x00), m_uSP(0x00), m_uPC(0x0000),
	m_uFetched(0x00), m_uAbsAddress(0x0000), m_nRelAddress(0x0000), m_uOpcode(0x00), m_uCycles(0x00)
{
	m_vecLookup = 
	{
		// 0x00
		MAKE(BRK, IMP, 7),
		MAKE(ORA, IDX, 6),
		MAKE(UOP, IMP, 2),
		MAKE(UOP, IMP, 2),
		MAKE(UOP, IMP, 2),
		MAKE(ORA, ZPG, 3),
		MAKE(ASL, ZPG, 5),
		MAKE(UOP, IMP, 2),
		MAKE(PHP, IMP, 3),
		MAKE(ORA, IMM, 2),
		MAKE(ASL, IMP, 2),
		MAKE(UOP, IMP, 2),
		MAKE(UOP, IMP, 2),
		MAKE(ORA, ABS, 4),
		MAKE(ASL, ABS, 6),
		MAKE(UOP, IMP, 2),

		// 0x10
		MAKE(BPL, REL, 2),
		MAKE(ORA, IDY, 5),
		MAKE(UOP, IMP, 2),
		MAKE(UOP, IMP, 2),
		MAKE(UOP, IMP, 2),
		MAKE(ORA, ZPX, 4),
		MAKE(ASL, ZPX, 6),
		MAKE(UOP, IMP, 2),
		MAKE(CLC, IMP, 2),
		MAKE(ORA, ABY, 4),
		MAKE(UOP, IMP, 2),
		MAKE(UOP, IMP, 2),
		MAKE(UOP, IMP, 2),
		MAKE(ORA, ABX, 4),
		MAKE(ASL, ABX, 7),
		MAKE(UOP, IMP, 2),

		// 0x20
		MAKE(JSR, ABS, 6),
		MAKE(AND, IDX, 6),
		MAKE(UOP, IMP, 2),
		MAKE(UOP, IMP, 2),
		MAKE(BIT, ZPG, 3),
		MAKE(AND, ZPG, 3),
		MAKE(ROL, ZPG, 5),
		MAKE(UOP, IMP, 2),
		MAKE(PLP, IMP, 4),
		MAKE(AND, IMM, 2),
		MAKE(ROL, IMP, 2),
		MAKE(UOP, IMP, 2),
		MAKE(BIT, ABS, 4),
		MAKE(AND, ABS, 4),
		MAKE(ROL, ABS, 6),
		MAKE(UOP, IMP, 2),

		// 0x30
		MAKE(BMI, REL, 2),
		MAKE(AND, IDY, 5),
		MAKE(UOP, IMP, 2),
		MAKE(UOP, IMP, 2),
		MAKE(UOP, IMP, 2),
		MAKE(AND, ZPX, 4),
		MAKE(ROL, ZPX, 6),
		MAKE(UOP, IMP, 2),
		MAKE(SEC, IMP, 2),
		MAKE(AND, ABY, 4),
		MAKE(UOP, IMP, 2),
		MAKE(UOP, IMP, 2),
		MAKE(UOP, IMP, 2),
		MAKE(AND, ABX, 4),
		MAKE(ROL, ABX, 7),
		MAKE(UOP, IMP, 2),

		// 0x40
		MAKE(RTI, IMP, 6),
		MAKE(EOR, IDX, 6),
		MAKE(UOP, IMP, 2),
		MAKE(UOP, IMP, 2),
		MAKE(UOP, IMP, 2),
		MAKE(EOR, ZPG, 3),
		MAKE(LSR, ZPG, 5),
		MAKE(UOP, IMP, 2),
		MAKE(PHA, IMP, 3),
		MAKE(EOR, IMM, 2),
		MAKE(LSR, IMP, 2),
		MAKE(UOP, IMP, 2),
		MAKE(JMP, ABS, 3),
		MAKE(EOR, ABS, 4),
		MAKE(LSR, ABS, 6),
		MAKE(UOP, IMP, 2),

		// 0x50
		MAKE(BVC, REL, 2),
		MAKE(EOR, IDY, 5),
		MAKE(UOP, IMP, 2),
		MAKE(UOP, IMP, 2),
		MAKE(UOP, IMP, 2),
		MAKE(EOR, ZPX, 4),
		MAKE(LSR, ZPX, 6),
		MAKE(UOP, IMP, 2),
		MAKE(CLI, IMP, 2),
		MAKE(EOR, ABY, 4),
		MAKE(UOP, IMP, 2),
		MAKE(UOP, IMP, 2),
		MAKE(UOP, IMP, 2),
		MAKE(EOR, ABX, 4),
		MAKE(LSR, ABX, 7),
		MAKE(UOP, IMP, 2),

		// 0x60
		MAKE(RTS, IMP, 6),
		MAKE(ADC, INX, 6),
		MAKE(UOP, IMP, 2),
		MAKE(UOP, IMP, 2),
		MAKE(UOP, IMP, 2),
		MAKE(ADC, ZPG, 3),
		MAKE(ROR, ZPG, 5),
		MAKE(UOP, IMP, 2),
		MAKE(PLA, IMP, 4),
		MAKE(ADC, IMM, 2),
		MAKE(ROR, IMP, 2),
		MAKE(UOP, IMP, 2),
		MAKE(JMP, IND, 5),
		MAKE(ADC, ABS, 4),
		MAKE(ROR, ABS, 6),
		MAKE(UOP, IMP, 2),

		// 0x70
		MAKE(BVS, REL, 2),
		MAKE(ADC, IDY, 5),
		MAKE(UOP, IMP, 2),
		MAKE(UOP, IMP, 2),
		MAKE(UOP, IMP, 2),
		MAKE(ADC, ZPX, 4),
		MAKE(ROR, ZPX, 6),
		MAKE(UOP, IMP, 2),
		MAKE(SEI, IMP, 2),
		MAKE(ADC, ABY, 4),
		MAKE(UOP, IMP, 2),
		MAKE(UOP, IMP, 2),
		MAKE(UOP, IMP, 2),
		MAKE(ADC, ABX, 4),
		MAKE(ROR, ABX, 7),
		MAKE(UOP, IMP, 2),

		// 0x80
		MAKE(UOP, IMP, 2),
		MAKE(STA, IDX, 6),
		MAKE(UOP, IMP, 2),
		MAKE(UOP, IMP, 2),
		MAKE(STY, ZPG, 3),
		MAKE(STA, ZPG, 3),
		MAKE(STX, ZPG, 3),
		MAKE(UOP, IMP, 2),
		MAKE(DEY, IMP, 2),
		MAKE(UOP, IMP, 2),
		MAKE(TXA, IMP, 2),
		MAKE(UOP, IMP, 2),
		MAKE(STY, ABS, 4),
		MAKE(STA, ABS, 4),
		MAKE(STX, ABS, 4),
		MAKE(UOP, IMP, 2),

		// 0x90
		MAKE(BCC, REL, 2),
		MAKE(STA, IDY, 6),
		MAKE(UOP, IMP, 2),
		MAKE(UOP, IMP, 2),
		MAKE(STY, ZPX, 4),
		MAKE(STA, ZPX, 4),
		MAKE(STX, ZPY, 4),
		MAKE(UOP, IMP, 2),
		MAKE(TYA, IMP, 2),
		MAKE(STA, ABY, 5),
		MAKE(TXS, IMP, 2),
		MAKE(UOP, IMP, 2),
		MAKE(UOP, IMP, 2),
		MAKE(STA, ABX, 5),
		MAKE(UOP, IMP, 2),
		MAKE(UOP, IMP, 2),

		// 0xA0
		MAKE(LDY, IMM, 2),
		MAKE(LDA, IDX, 6),
		MAKE(LDX, IMM, 2),
		MAKE(UOP, IMP, 2),
		MAKE(LDY, ZPG, 3),
		MAKE(LDA, ZPG, 3),
		MAKE(LDX, ZPG, 3),
		MAKE(UOP, IMP, 2),
		MAKE(TAY, IMP, 2),
		MAKE(LDA, IMM, 2),
		MAKE(TAX, IMP, 2),
		MAKE(UOP, IMP, 2),
		MAKE(LDY, ABS, 4),
		MAKE(LDA, ABS, 4),
		MAKE(LDX, ABS, 4),
		MAKE(UOP, IMP, 2),

		// 0xB0
		MAKE(BCS, REL, 2),
		MAKE(LDA, IDY, 5),
		MAKE(UOP, IMP, 2),
		MAKE(UOP, IMP, 2),
		MAKE(LDY, ZPX, 4),
		MAKE(LDA, ZPX, 4),
		MAKE(LDX, ZPY, 4),
		MAKE(UOP, IMP, 2),
		MAKE(CLV, IMP, 2),
		MAKE(LDA, ABY, 4),
		MAKE(TSX, IMP, 2),
		MAKE(UOP, IMP, 2),
		MAKE(LDY, ABX, 4),
		MAKE(LDA, ABX, 4),
		MAKE(LDX, ABY, 4),
		MAKE(UOP, IMP, 2),

		// 0xC0
		MAKE(CPY, IMM, 2),
		MAKE(CMP, IDX, 6),
		MAKE(UOP, IMP, 2),
		MAKE(UOP, IMP, 2),
		MAKE(CPY, ZPG, 3),
		MAKE(CMP, ZPG, 3),
		MAKE(DEC, ZPG, 5),
		MAKE(UOP, IMP, 2),
		MAKE(INY, IMP, 2),
		MAKE(CMP, IMM, 2),
		MAKE(DEX, IMP, 2),
		MAKE(UOP, IMP, 2),
		MAKE(CPY, ABS, 4),
		MAKE(CMP, ABS, 4),
		MAKE(DEC, ABS, 6),
		MAKE(UOP, IMP, 2),

		// 0xD0
		MAKE(BNE, REL, 2),
		MAKE(CMP, IDY, 5),
		MAKE(UOP, IMP, 2),
		MAKE(UOP, IMP, 2),
		MAKE(UOP, IMP, 2),
		MAKE(CMP, ZPX, 4),
		MAKE(DEC, ZPX, 6),
		MAKE(UOP, IMP, 2),
		MAKE(CLD, IMP, 2),
		MAKE(CMP, ABY, 4),
		MAKE(UOP, IMP, 2),
		MAKE(UOP, IMP, 2),
		MAKE(UOP, IMP, 2),
		MAKE(CMP, ABX, 4),
		MAKE(DEC, ABX, 7),
		MAKE(UOP, IMP, 2),

		// 0xE0
		MAKE(CPX, IMM, 2),
		MAKE(SBC, IDX, 6),
		MAKE(UOP, IMP, 2),
		MAKE(UOP, IMP, 2),
		MAKE(CPX, ZPG, 3),
		MAKE(SBC, ZPG, 3),
		MAKE(INC, ZPG, 5),
		MAKE(UOP, IMP, 2),
		MAKE(INX, IMP, 2),
		MAKE(SBC, IMM, 2),
		MAKE(NOP, IMP, 2),
		MAKE(UOP, IMP, 2),
		MAKE(CPX, ABS, 4),
		MAKE(SBC, ABS, 4),
		MAKE(INC, ABS, 6),
		MAKE(UOP, IMP, 2),

		// 0xF0
		MAKE(BEQ, REL, 2),
		MAKE(SBC, IDY, 5),
		MAKE(UOP, IMP, 2),
		MAKE(UOP, IMP, 2),
		MAKE(UOP, IMP, 2),
		MAKE(SBC, ZPX, 4),
		MAKE(INC, ZPX, 6),
		MAKE(UOP, IMP, 2),
		MAKE(SED, IMP, 2),
		MAKE(SBC, ABY, 4),
		MAKE(UOP, IMP, 2),
		MAKE(UOP, IMP, 2),
		MAKE(UOP, IMP, 2),
		MAKE(SBC, ABX, 4),
		MAKE(INC, ABX, 7),
		MAKE(UOP, IMP, 2),
		};
}

Mos6502::~Mos6502()
{
	// Don't delete m_pBus, it might be used elsewhere
}


/////////////////// ADDRESSING MODES ///////////////////////
#pragma region

BYTE Mos6502::IMP()
{
	m_uFetched = m_uAcc;
	return 0;
}

BYTE Mos6502::IMM()
{
	m_uAbsAddress = m_uPC++;
}

BYTE Mos6502::ZPG()
{
	m_uAbsAddress = Read(m_uPC++);
	m_uAbsAddress &= 0x00FF;
	return 0;
}

BYTE Mos6502::ABS()
{
	WORD low = Read(m_uPC++);
	WORD high = Read(m_uPC++);
	m_uAbsAddress = (high << 8) | low;
	return 0;
}

BYTE Mos6502::REL()
{
	// TODO: If something doesn't work, this is probably why
	m_nRelAddress = static_cast<int8_t>(Read(m_uPC++));

	return 0;
}

BYTE Mos6502::IND()
{
	WORD low = Read(m_uPC++);
	WORD high = Read(m_uPC++);
	WORD ptr = (high << 8) | low;

	// There is a hardware bug in which if you try to read from memory $xxFF
	// it will not fetch the high byte from the next page, but instead wrap around
	// and fetch it from $xx00.
	// e.g.: JMP $FF $45
	// This will not read from $45FF $4600 as expected, but rather from $45FF $4500
	// This piece of code emulates this behavior
	if (low == 0x00FF)	// Hardware Bug :)
	{
		m_uAbsAddress = (Read(ptr & 0xFF00) << 8) | Read(ptr);
	}
	else
	{
		m_uAbsAddress = (Read(ptr + 1) << 8) | Read(ptr);
	}

	return 0;
}

BYTE Mos6502::ZPX()
{
	m_uAbsAddress = Read(m_uPC++) + m_uX;
	m_uAbsAddress &= 0x00FF;
	return 0;
}

BYTE Mos6502::ZPY()
{
	m_uAbsAddress = Read(m_uPC++) + m_uY;
	m_uAbsAddress &= 0x00FF;
	return 0;

}

BYTE Mos6502::ABX()
{
	WORD low = Read(m_uPC++);
	WORD high = Read(m_uPC++);
	m_uAbsAddress = (high << 8) | low;
	m_uAbsAddress += m_uX;

	// If incrementing by X changes the memory page, another clock cycle is needed
	if ((m_uAbsAddress & 0xFF00) != (high << 8))
		return 1;

	return 0;
}

BYTE Mos6502::ABY()
{
	WORD low = Read(m_uPC++);
	WORD high = Read(m_uPC++);
	m_uAbsAddress = (high << 8) | low;
	m_uAbsAddress += m_uY;

	// If incrementing by Y changes the memory page, another clock cycle is needed
	if ((m_uAbsAddress & 0xFF00) != (high << 8))
		return 1;

	return 0;
}

BYTE Mos6502::IDX()
{
	BYTE offset = Read(m_uPC++);

	WORD low = Read(offset + m_uX);
	WORD high = Read(offset + m_uX + 1);

	m_uAbsAddress = (high << 8) | low;

	return 0;
}

BYTE Mos6502::IDY()
{
	BYTE offset = Read(m_uPC++);

	WORD low = Read(offset);
	WORD high = Read(offset + 1);

	m_uAbsAddress = (high << 8) | low;
	m_uAbsAddress += m_uY;

	if ((m_uAbsAddress & 0xFF00) != (high << 8))
		return 1;
	
	return 0;

	return 0;
}

BYTE Mos6502::AND()
{
	Fetch();
	m_uAcc = m_uAcc & m_uFetched;

	m_oStatus.Zero = (m_uAcc == 0x00);
	m_oStatus.Negative = (m_uAcc & 0x80);

	return 1;
}

BYTE Mos6502::BCS()
{
	Fetch();
	if (m_oStatus.Carry)
	{
		m_uCycles++;
		m_uAbsAddress = m_uPC + m_nRelAddress;

		// Add another cycle on page boundary crossing
		if ((m_uAbsAddress & 0xFF00) != (m_uPC & 0xFF00))
			m_uCycles++;

		m_uPC = m_uAbsAddress;
	}

	return 0;
}

#pragma endregion

///////////////////// INSTRUCTIONS /////////////////////////

void Mos6502::Tick()
{
	if (m_uCycles == 0)
	{
		m_uOpcode = Read(m_uPC);	// Read Opcode from memory
		m_uPC++;

		m_uCycles = m_vecLookup[m_uOpcode].cycles;

		// Get addressing mode
		BYTE additionalCycle1 = m_vecLookup[m_uOpcode].addressMode();

		// Get operation
		BYTE additionalCycle2 = m_vecLookup[m_uOpcode].operation();

		m_uCycles += (additionalCycle1 & additionalCycle2);
	}

	m_uCycles--;
}

BYTE Mos6502::Read(WORD address)
{
	return m_pBus->Read(address);
}

void Mos6502::Write(WORD address, BYTE value)
{
	m_pBus->Write(address, value);
}

BYTE Mos6502::Fetch()
{
	m_uFetched = Read(m_uAbsAddress);
	return m_uFetched;
}

