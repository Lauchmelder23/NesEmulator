#include "Mos6502.hpp"
#include "Bus.hpp"

#include <sstream>
#include <string.h>
#include <iostream>

Mos6502::Mos6502() :
	m_pBus(nullptr),
	m_uAcc(0x00), m_uX(0x00), m_uY(0x00), m_uSP(0x00), m_uPC(0x0000),
	m_uFetched(0x00), m_uOpcode(0x00), m_uCycles(0x00), m_uCyclesTotal(0),
	m_isHalted(false)
{

	m_vecLookup = 
	{
		// 0x00
		MAKE(BRK, IMP, 7),
		MAKE(ORA, IDX, 6),
		MAKE_ILLEGAL("JAM", ILL_JAM, IMP, 0), 
		MAKE_ILLEGAL("SLO*", ILL_SLO, IDX, 8),
		MAKE_ILLEGAL("NOP*", ILL_NOP, ZPG, 3),
		MAKE(ORA, ZPG, 3),
		MAKE(ASL, ZPG, 5),
		MAKE_ILLEGAL("SLO*", ILL_SLO, ZPG, 5),
		MAKE(PHP, IMP, 3),
		MAKE(ORA, IMM, 2),
		MAKE(ASL, ACC, 2),
		MAKE_ILLEGAL("ANC**", ILL_ANC, IMM, 2),
		MAKE_ILLEGAL("NOP*", ILL_NOP, ABS, 4),
		MAKE(ORA, ABS, 4),
		MAKE(ASL, ABS, 6),
		MAKE_ILLEGAL("SLO*", ILL_SLO, ABS, 6),

		// 0x10
		MAKE(BPL, REL, 2),
		MAKE(ORA, IDY, 5),
		MAKE_ILLEGAL("JAM", ILL_JAM, IMP, 0),
		MAKE_ILLEGAL("SLO*", ILL_SLO, IDY, 8),
		MAKE_ILLEGAL("NOP*", ILL_NOP, ZPX, 4),
		MAKE(ORA, ZPX, 4),
		MAKE(ASL, ZPX, 6),
		MAKE_ILLEGAL("SLO*", ILL_SLO, ZPX, 6),
		MAKE(CLC, IMP, 2),
		MAKE(ORA, ABY, 4),
		MAKE_ILLEGAL("NOP*", ILL_NOP, IMP, 2),
		MAKE_ILLEGAL("SLO*", ILL_SLO, ABY, 7),
		MAKE_ILLEGAL("NOP*", ILL_NOP, ABX, 4),
		MAKE(ORA, ABX, 4),
		MAKE(ASL, ABX, 7),
		MAKE_ILLEGAL("SLO*", ILL_SLO, ABX, 7),

		// 0x20
		MAKE(JSR, ABS, 6),
		MAKE(AND, IDX, 6),
		MAKE_ILLEGAL("JAM", ILL_JAM, IMP, 0),
		MAKE_ILLEGAL("RLA*", ILL_RLA, IDX, 8),
		MAKE(BIT, ZPG, 3),
		MAKE(AND, ZPG, 3),
		MAKE(ROL, ZPG, 5),
		MAKE_ILLEGAL("RLA*", ILL_RLA, ZPG, 5),
		MAKE(PLP, IMP, 4),
		MAKE(AND, IMM, 2),
		MAKE(ROL, ACC, 2),
		MAKE_ILLEGAL("ANC**", ILL_ANC, IMM, 2),
		MAKE(BIT, ABS, 4),
		MAKE(AND, ABS, 4),
		MAKE(ROL, ABS, 6),
		MAKE_ILLEGAL("RAL*", ILL_RLA, ABS, 6),

		// 0x30
		MAKE(BMI, REL, 2),
		MAKE(AND, IDY, 5),
		MAKE_ILLEGAL("JAM", ILL_JAM, IMP, 0),
		MAKE_ILLEGAL("RLA*", ILL_RLA, IDY, 8),
		MAKE_ILLEGAL("NOP*", ILL_NOP, ZPX, 4),
		MAKE(AND, ZPX, 4),
		MAKE(ROL, ZPX, 6),
		MAKE_ILLEGAL("RLA*", ILL_RLA, ZPX, 6),
		MAKE(SEC, IMP, 2),
		MAKE(AND, ABY, 4),
		MAKE_ILLEGAL("NOP*", ILL_NOP, IMP, 2),
		MAKE_ILLEGAL("RLA*", ILL_RLA, ABY, 7),
		MAKE_ILLEGAL("NOP*", ILL_NOP, ABX, 4),
		MAKE(AND, ABX, 4),
		MAKE(ROL, ABX, 7),
		MAKE_ILLEGAL("RLA*", ILL_RLA, ABX, 7),

		// 0x40
		MAKE(RTI, IMP, 6),
		MAKE(EOR, IDX, 6),
		MAKE_ILLEGAL("JAM", ILL_JAM, IMP, 0),
		MAKE_ILLEGAL("SRE*", ILL_SRE, IDX, 8),
		MAKE_ILLEGAL("NOP*", ILL_NOP, ZPG, 3),
		MAKE(EOR, ZPG, 3),
		MAKE(LSR, ZPG, 5),
		MAKE_ILLEGAL("SRE*", ILL_SRE, ZPG, 5),
		MAKE(PHA, IMP, 3),
		MAKE(EOR, IMM, 2),
		MAKE(LSR, ACC, 2),
		MAKE_ILLEGAL("ALR**", ILL_ALR, IMM, 2),
		MAKE(JMP, ABS, 3),
		MAKE(EOR, ABS, 4),
		MAKE(LSR, ABS, 6),
		MAKE_ILLEGAL("SRE*", ILL_SRE, ABS, 6),

		// 0x50
		MAKE(BVC, REL, 2),
		MAKE(EOR, IDY, 5),
		MAKE_ILLEGAL("JAM", ILL_JAM, IMP, 0),
		MAKE_ILLEGAL("SRE*", ILL_SRE, IDY, 8),
		MAKE_ILLEGAL("NOP*", ILL_NOP, ZPX, 4),
		MAKE(EOR, ZPX, 4),
		MAKE(LSR, ZPX, 6),
		MAKE_ILLEGAL("SRE*", ILL_SRE, ZPX, 6),
		MAKE(CLI, IMP, 2),
		MAKE(EOR, ABY, 4),
		MAKE_ILLEGAL("NOP*", ILL_NOP, IMP, 2),
		MAKE_ILLEGAL("SRE*", ILL_SRE, ABY, 7),
		MAKE_ILLEGAL("NOP*", ILL_NOP, ABX, 4),
		MAKE(EOR, ABX, 4),
		MAKE(LSR, ABX, 7),
		MAKE_ILLEGAL("SRE*", ILL_SRE, ABX, 7),

		// 0x60
		MAKE(RTS, IMP, 6),
		MAKE(ADC, IDX, 6),
		MAKE_ILLEGAL("JAM", ILL_JAM, IMP, 0),
		MAKE_ILLEGAL("RRA*", ILL_RRA, IDX, 8),
		MAKE_ILLEGAL("NOP*", ILL_NOP, ZPG, 3),
		MAKE(ADC, ZPG, 3),
		MAKE(ROR, ZPG, 5),
		MAKE_ILLEGAL("RRA*", ILL_RRA, ZPG, 5),
		MAKE(PLA, IMP, 4),
		MAKE(ADC, IMM, 2),
		MAKE(ROR, ACC, 2),
		MAKE_ILLEGAL("ARR**", ILL_ARR, IMM, 2),
		MAKE(JMP, IND, 5),
		MAKE(ADC, ABS, 4),
		MAKE(ROR, ABS, 6),
		MAKE_ILLEGAL("RRA*", ILL_RRA, ABS, 6),

		// 0x70
		MAKE(BVS, REL, 2),
		MAKE(ADC, IDY, 5),
		MAKE_ILLEGAL("JAM", ILL_JAM, IMP, 0),
		MAKE_ILLEGAL("RRA*", ILL_RRA, IDY, 8),
		MAKE_ILLEGAL("NOP*", ILL_NOP, ZPX, 4),
		MAKE(ADC, ZPX, 4),
		MAKE(ROR, ZPX, 6),
		MAKE_ILLEGAL("RRA*", ILL_RRA, ZPX, 6),
		MAKE(SEI, IMP, 2),
		MAKE(ADC, ABY, 4),
		MAKE_ILLEGAL("NOP*", ILL_NOP, IMP, 2),
		MAKE_ILLEGAL("RRA*", ILL_RRA, ABY, 7),
		MAKE_ILLEGAL("NOP*", ILL_NOP, ABX, 4),
		MAKE(ADC, ABX, 4),
		MAKE(ROR, ABX, 7),
		MAKE_ILLEGAL("RRA*", ILL_RRA, ABX, 7),

		// 0x80
		MAKE_ILLEGAL("NOP*", ILL_NOP, IMM, 2),
		MAKE(STA, IDX, 6),
		MAKE_ILLEGAL("NOP*t", ILL_NOP, IMM, 2),
		MAKE_ILLEGAL("SAX*", ILL_SAX, IDX, 6),
		MAKE(STY, ZPG, 3),
		MAKE(STA, ZPG, 3),
		MAKE(STX, ZPG, 3),
		MAKE_ILLEGAL("SAX*", ILL_SAX, ZPG, 3),
		MAKE(DEY, IMP, 2),
		MAKE_ILLEGAL("NOP*", ILL_NOP, IMM, 2),
		MAKE(TXA, IMP, 2),
		MAKE_ILLEGAL("XAA**", ILL_XAA, IMM, 2),
		MAKE(STY, ABS, 4),
		MAKE(STA, ABS, 4),
		MAKE(STX, ABS, 4),
		MAKE_ILLEGAL("SAX*", ILL_SAX, ABS, 4),

		// 0x90
		MAKE(BCC, REL, 2),
		MAKE(STA, IDY, 6),
		MAKE_ILLEGAL("JAM", ILL_JAM, IMP, 0),
		MAKE_ILLEGAL("AHX**", ILL_AHX, IDY, 6),
		MAKE(STY, ZPX, 4),
		MAKE(STA, ZPX, 4),
		MAKE(STX, ZPY, 4),
		MAKE_ILLEGAL("SAX*", ILL_SAX, ZPY, 4),
		MAKE(TYA, IMP, 2),
		MAKE(STA, ABY, 5),
		MAKE(TXS, IMP, 2),
		MAKE_ILLEGAL("TAS**", ILL_TAS, ABY, 5),
		MAKE_ILLEGAL("SHY**", ILL_SHY, ABX, 5),
		MAKE(STA, ABX, 5),
		MAKE_ILLEGAL("SHX**", ILL_SHX, ABY, 5),
		MAKE_ILLEGAL("AHX**", ILL_AHX, ABY, 5),

		// 0xA0
		MAKE(LDY, IMM, 2),
		MAKE(LDA, IDX, 6),
		MAKE(LDX, IMM, 2),
		MAKE_ILLEGAL("LAX*", ILL_LAX, IDX, 6),
		MAKE(LDY, ZPG, 3),
		MAKE(LDA, ZPG, 3),
		MAKE(LDX, ZPG, 3),
		MAKE_ILLEGAL("LAX", ILL_LAX, ZPG, 3),
		MAKE(TAY, IMP, 2),
		MAKE(LDA, IMM, 2),
		MAKE(TAX, IMP, 2),
		MAKE_ILLEGAL("LAX**", ILL_LAX, IMM, 2),
		MAKE(LDY, ABS, 4),
		MAKE(LDA, ABS, 4),
		MAKE(LDX, ABS, 4),
		MAKE_ILLEGAL("LAX*", ILL_LAX, ABS, 4),

		// 0xB0
		MAKE(BCS, REL, 2),
		MAKE(LDA, IDY, 5),
		MAKE_ILLEGAL("JAM", ILL_JAM, IMP, 0),
		MAKE_ILLEGAL("LAX*", ILL_LAX, IDY, 5),
		MAKE(LDY, ZPX, 4),
		MAKE(LDA, ZPX, 4),
		MAKE(LDX, ZPY, 4),
		MAKE_ILLEGAL("LAX*", ILL_LAX, ZPY, 4),
		MAKE(CLV, IMP, 2),
		MAKE(LDA, ABY, 4),
		MAKE(TSX, IMP, 2),
		MAKE_ILLEGAL("LAS**", ILL_LAS, ABY, 4),
		MAKE(LDY, ABX, 4),
		MAKE(LDA, ABX, 4),
		MAKE(LDX, ABY, 4),
		MAKE_ILLEGAL("LAX*", ILL_LAX, ABY, 4),

		// 0xC0
		MAKE(CPY, IMM, 2),
		MAKE(CMP, IDX, 6),
		MAKE_ILLEGAL("NOP*t", ILL_NOP, IMM, 2),
		MAKE_ILLEGAL("DCP*", ILL_DCP, IDX, 8),
		MAKE(CPY, ZPG, 3),
		MAKE(CMP, ZPG, 3),
		MAKE(DEC, ZPG, 5),
		MAKE_ILLEGAL("DCP*", ILL_DCP, ZPG, 5),
		MAKE(INY, IMP, 2),
		MAKE(CMP, IMM, 2),
		MAKE(DEX, IMP, 2),
		MAKE_ILLEGAL("AXS**", ILL_AXS, IMM, 2),
		MAKE(CPY, ABS, 4),
		MAKE(CMP, ABS, 4),
		MAKE(DEC, ABS, 6),
		MAKE_ILLEGAL("DCP*", ILL_DCP, ABS, 6),

		// 0xD0
		MAKE(BNE, REL, 2),
		MAKE(CMP, IDY, 5),
		MAKE_ILLEGAL("JAM", ILL_JAM, IMP, 0),
		MAKE_ILLEGAL("DCP*", ILL_DCP, IDY, 8),
		MAKE_ILLEGAL("NOP*", ILL_NOP, ZPX, 4),
		MAKE(CMP, ZPX, 4),
		MAKE(DEC, ZPX, 6),
		MAKE_ILLEGAL("DCP*", ILL_DCP, ZPX, 6),
		MAKE(CLD, IMP, 2),
		MAKE(CMP, ABY, 4),
		MAKE_ILLEGAL("NOP*", ILL_NOP, IMP, 2),
		MAKE_ILLEGAL("DCP*", ILL_DCP, ABY, 7),
		MAKE_ILLEGAL("NOP*", ILL_NOP, ABX, 4),
		MAKE(CMP, ABX, 4),
		MAKE(DEC, ABX, 7),
		MAKE_ILLEGAL("DCP*", ILL_DCP, ABX, 7),

		// 0xE0
		MAKE(CPX, IMM, 2),
		MAKE(SBC, IDX, 6),
		MAKE_ILLEGAL("NOP*t", ILL_NOP, IMM, 2),
		MAKE_ILLEGAL("ISC*", ILL_ISC, IDX, 8),
		MAKE(CPX, ZPG, 3),
		MAKE(SBC, ZPG, 3),
		MAKE(INC, ZPG, 5),
		MAKE_ILLEGAL("ISC*", ILL_ISC, ZPG, 5),
		MAKE(INX, IMP, 2),
		MAKE(SBC, IMM, 2),
		MAKE(NOP, IMP, 2),
		MAKE_ILLEGAL("SBC*", ILL_SBC, IMM, 2),
		MAKE(CPX, ABS, 4),
		MAKE(SBC, ABS, 4),
		MAKE(INC, ABS, 6),
		MAKE_ILLEGAL("ISC*", ILL_ISC, ABS, 6),

		// 0xF0
		MAKE(BEQ, REL, 2),
		MAKE(SBC, IDY, 5),
		MAKE_ILLEGAL("JAM", ILL_JAM, IMP, 0),
		MAKE_ILLEGAL("ISC*", ILL_ISC, IDY, 8),
		MAKE_ILLEGAL("NOP*", ILL_NOP, ZPX, 4),
		MAKE(SBC, ZPX, 4),
		MAKE(INC, ZPX, 6),
		MAKE_ILLEGAL("ISC*", ILL_ISC, ZPX, 6),
		MAKE(SED, IMP, 2),
		MAKE(SBC, ABY, 4),
		MAKE_ILLEGAL("NOP*", ILL_NOP, IMP, 2),
		MAKE_ILLEGAL("ISC*", ILL_ISC, ABY, 7),
		MAKE_ILLEGAL("NOP*", ILL_NOP, ABX, 4),
		MAKE(SBC, ABX, 4),
		MAKE(INC, ABX, 7),
		MAKE_ILLEGAL("ISC*", ILL_ISC, ABX, 7),
		};

	// Reset();
}

Mos6502::~Mos6502()
{
	// Don't delete m_pBus, it might be used elsewhere
}


void Mos6502::Tick()
{
	if (!m_isHalted)
	{
		//if (m_uCyclesTotal == 14711)
		//	__debugbreak();

		if (m_uCycles == 0)
		{

			m_uOpcode = Read(m_uPC);	// Read Opcode from memory
			m_uPC++;

			m_uCycles = m_vecLookup[m_uOpcode].cycles;

			Fetch();	// Get Addressing mode, write the value for the operation to m_uFetched
			bool m_bSwitch = Execute();
			if (m_bSwitchedPage && m_bSwitch)
				m_uCycles++;
		}

		m_uCycles--;
		m_uCyclesTotal++;
	}
}

void Mos6502::Reset()
{
	m_uAcc = 0x00;
	m_uX = 0x00;
	m_uY = 0x00;
	m_uSP = 0xFD;
	m_oStatus.Raw = 0x34;
	m_uCycles = 7;
	m_uCyclesTotal = 0;

	m_uPC = 0xC000; //TO_WORD(Read(0xFFFC), Read(0xFFFD));	// 0xFFFC is the reset vector

	m_uFetched = 0x00;
}

void Mos6502::IRQ()
{
	if (!m_oStatus.Flag.Interrupt)
	{
		// Push current PC to stack
		PushTo((m_uPC & 0xFF00) >> 8);
		PushTo(m_uPC & 0x00FF);

		// Push status to stack
		m_oStatus.Flag.Break = false;
		m_oStatus.Flag.Unused = true;
		m_oStatus.Flag.Interrupt = true;
		PushTo(m_oStatus.Raw);	// TODO: Untested, might break

		// Get new PC from Interrupt vector 0xFFFE
		m_uPC = TO_WORD(Read(0xFFFE), Read(0xFFFF));

		m_uCycles = 7;
	}
}

void Mos6502::NMI()
{
	// Push current PC to stack
	PushTo((m_uPC & 0xFF00) >> 8);
	PushTo(m_uPC & 0x00FF);

	// Push status to stack
	m_oStatus.Flag.Break = false;
	m_oStatus.Flag.Unused = true;
	m_oStatus.Flag.Interrupt = true;
	PushTo(m_oStatus.Raw);	// TODO: Untested, might break

	// Get new PC from Interrupt vector 0xFFFE
	m_uPC = TO_WORD(Read(0xFFFA), Read(0xFFFB));

	m_uCycles = 7;
}

std::map<WORD, std::string> Mos6502::Disassemble(WORD begin, WORD end)
{
	// Disassembles the RAM from $begin to $end
	std::map<WORD, std::string> disassemble;

	for (uint32_t ptr = begin; ptr <= end; ptr++)
	{
		Instruction i = m_vecLookup[Read(ptr)];
		std::stringstream ss;

		// Do different things depending on addressing mode
		// Very ugly, but it works I guess. It's okay, this code doesn't need to be efficient

		WORD orig = ptr;
		BYTE op = Read(ptr);
		BYTE lo = Read(ptr + 1);
		BYTE hi = Read(ptr + 2);
		switch (i.addressMode)
		{
		case IMP:
			ss << HEX("", op, 2) << "\t\t  " << i.name << " \t";
			break;

		case ACC:
			ss << HEX("", op, 2) << "\t\t  " << i.name << " ";
			ss << " A";
			break;

		case IMM:
			ss << HEX("", op, 2) << " " << HEX("", lo, 2) << "\t  " << i.name << " ";
			ss << HEX("#$", lo, 2);
			break;

		case ZPG:
			ss << HEX("", op, 2) << " " << HEX("", lo, 2) << "\t  " << i.name << " ";
			ss << HEX("$", lo, 2);
			break;

		case ABS:
			ss << HEX("", op, 2) << " " << HEX("", lo, 2)
				<< " " << HEX("", hi, 2) << "\t  " << i.name << " ";
			ss << HEX("$", TO_WORD(lo, hi), 4);
		break;

		case REL:
			ss << HEX("", op, 2) << " " << HEX("", lo, 2) << "\t  " << i.name << " ";
			ss << HEX("$", ptr + (int8_t)lo + 2, 4);
			break;

		case IND:
			ss << HEX("", op, 2) << " " << HEX("", lo, 2)
				<< " " << HEX("", hi, 2) << "\t  " << i.name << " ";
			ss << "(" << HEX("$", TO_WORD(lo, hi), 4) << ")";
			break;

		case ZPX:
			ss << HEX("", op, 2) << " " << HEX("", lo, 2) << "\t  " << i.name << " ";
			ss << "(" << HEX("$", lo, 2) << " + X)";
			break;

		case ZPY:
			ss << HEX("", op, 2) << " " << HEX("", lo, 2) << "\t  " << i.name << " ";
			ss << "(" << HEX("$", lo, 2) << ", Y)";
			break;

		case ABX:
			ss << HEX("", op, 2) << " " << HEX("", lo, 2)
				<< " " << HEX("", hi, 2) << "\t  " << i.name << " ";
			ss << HEX("$", TO_WORD(lo, hi), 4) << ", X";
			break;

		case ABY:
			ss << HEX("", op, 2) << " " << HEX("", lo, 2)
				<< " " << HEX("", hi, 2) << "\t  " << i.name << " ";
			ss << HEX("$", TO_WORD(lo, hi), 4) << ", Y";
			break;

		case IDX:
			ss << HEX("", op, 2) << " " << HEX("", lo, 2) << "\t  " << i.name << " ";
			ss << "(" << HEX("$", lo, 2) << ", X)";
			break;

		case IDY:
			ss << HEX("", op, 2) << " " << HEX("", lo, 2) << "\t  " << i.name << " ";
			ss << "(" << HEX("$", lo, 2) << "), Y";
			break;

		default:
			ss << "(Unknown addressing mode)";
			break;
		}

		disassemble.insert(std::make_pair(orig, ss.str()));
	}

	return disassemble;
}

BYTE Mos6502::Read(WORD address)
{
	return m_pBus->ReadCPU(address);
}

void Mos6502::Write(WORD address, BYTE value)
{
	m_pBus->WriteCPU(address, value);
}

bool Mos6502::Execute()
{
	switch (m_vecLookup[m_uOpcode].operation)
	{
	case ADC:	// ACC = ACC + M + C
	{
		WORD result = (WORD)m_uAcc + m_uFetched + m_oStatus.Flag.Carry;
		m_oStatus.Flag.Carry = false;

		m_oStatus.Flag.Carry = BIT_(8, result);
		m_oStatus.Flag.Zero = ((result & 0x00FF) == 0);
		m_oStatus.Flag.Negative = BIT_(7, result);

		// If accumulator and fetched have the same sign AND the sum and fetched have a different sign
		// then we have overflow
		// Checks if the previous are both true -----------------------------------------------------|
		//        Checks if signs are different ------------------------------------|                |
		//            Checks if signs are equal ---|                                |                |
		//                                         V                                V                V
		//                        |---------------------------------|   |---------------------|   |----|
		m_oStatus.Flag.Overflow = (~((WORD)m_uFetched ^ (WORD)m_uAcc) & (result ^ (WORD)m_uAcc) & 0x0080) >> 7;

		m_uAcc = result & 0x00FF;

		return true;
	}

	case AND:	// ACC = ACC & Memory
	{
		m_uAcc = m_uAcc & m_uFetched;

		// Set Flags if needed
		m_oStatus.Flag.Zero = (m_uAcc == 0x00);
		m_oStatus.Flag.Negative = BIT_(7, m_uAcc);

		return true;
	}

	case ASL:	// Arithmetic shift left
	{
		WORD result = (WORD)m_uFetched << 1;

		m_oStatus.Flag.Carry = BIT_(8, result);
		m_oStatus.Flag.Zero = ((result & 0x00FF) == 0);
		m_oStatus.Flag.Negative = BIT_(7, result);

		if (m_vecLookup[m_uOpcode].addressMode == ACC)
			m_uAcc = (result & 0x00FF);
		else
			Write(m_uFetchedFrom, result & 0x00FF);

		return false;

	};

	case BCC:	// Branch on Carry Clear
	{
		if (!m_oStatus.Flag.Carry)
		{
			m_uCycles++;
			WORD jumpTo = m_uPC + static_cast<int8_t>(m_uFetched);	// TODO: If something breaks, its here

			if ((jumpTo & 0xFF00) != (m_uPC & 0xFF00))
				m_uCycles++;

			m_uPC = jumpTo;
		}

		return false;
	};

	case BCS:	// Branch on Carry Set
	{
		if (m_oStatus.Flag.Carry)
		{
			m_uCycles++;
			WORD jumpTo = m_uPC + static_cast<int8_t>(m_uFetched);	// TODO: If something breaks, its here

			if ((jumpTo & 0xFF00) != (m_uPC & 0xFF00))
				m_uCycles++;

			m_uPC = jumpTo;
		}

		return false;
	};

	case BEQ:	// Branch on equal (Z = 1)
	{
		if (m_oStatus.Flag.Zero)
		{
			m_uCycles++;
			WORD jumpTo = m_uPC + static_cast<int8_t>(m_uFetched);	// TODO: If something breaks, its here

			if ((jumpTo & 0xFF00) != (m_uPC & 0xFF00))
				m_uCycles++;

			m_uPC = jumpTo;
		}

		return false;
	};

	case BIT:	// Test bits in memory with accumulator
	{
		BYTE result = m_uAcc & m_uFetched;
		
		m_oStatus.Flag.Zero = (result == 0x00);
		m_oStatus.Flag.Negative = BIT_(7, m_uFetched);
		m_oStatus.Flag.Overflow = BIT_(6, m_uFetched);

		return false;
	};

	case BMI:	// Branch on Minus
	{
		if (m_oStatus.Flag.Negative)
		{
			m_uCycles++;
			WORD jumpTo = m_uPC + static_cast<int8_t>(m_uFetched);	// TODO: If something breaks, its here

			if ((jumpTo & 0xFF00) != (m_uPC & 0xFF00))
				m_uCycles++;

			m_uPC = jumpTo;
		}

		return false;
	}

	case BNE:	// Branch on not equal (Z = 0)
	{
		if (!m_oStatus.Flag.Zero)
		{
			m_uCycles++;
			WORD jumpTo = m_uPC + static_cast<int8_t>(m_uFetched);	// TODO: If something breaks, its here

			if ((jumpTo & 0xFF00) != (m_uPC & 0xFF00))
				m_uCycles++;

			m_uPC = jumpTo;
		}

		return false;
	};

	case BPL:	// Branch on plus
	{
		if (!m_oStatus.Flag.Negative)
		{
			m_uCycles++;
			WORD jumpTo = m_uPC + static_cast<int8_t>(m_uFetched);	// TODO: If something breaks, its here

			if ((jumpTo & 0xFF00) != (m_uPC & 0xFF00))
				m_uCycles++;

			m_uPC = jumpTo;
		}

		return false;
	}

	case BRK:	// Break
	{
		m_uPC++;
		m_oStatus.Flag.Interrupt = true;

		PushTo((m_uPC & 0xFF00) >> 8);
		PushTo(m_uPC & 0x00FF);

		m_oStatus.Flag.Break = true;
		PushTo(m_oStatus.Raw);
		m_oStatus.Flag.Break = false;

		m_uPC = TO_WORD(Read(0xFFFE), Read(0xFFFF));

		return false;
	};

	case BVC:	// Branch on overflow clear
	{
		if (!m_oStatus.Flag.Overflow)
		{
			m_uCycles++;
			WORD jumpTo = m_uPC + static_cast<int8_t>(m_uFetched);	// TODO: If something breaks, its here

			if ((jumpTo & 0xFF00) != (m_uPC & 0xFF00))
				m_uCycles++;

			m_uPC = jumpTo;
		}

		return false;
	}

	case BVS:	// Branch on overflow set
	{
		if (m_oStatus.Flag.Overflow)
		{
			m_uCycles++;
			WORD jumpTo = m_uPC + static_cast<int8_t>(m_uFetched);	// TODO: If something breaks, its here

			if ((jumpTo & 0xFF00) != (m_uPC & 0xFF00))
				m_uCycles++;

			m_uPC = jumpTo;
		}

		return false;
	}

	case CLC:	// Clear Carry
	{
		m_oStatus.Flag.Carry = false;
		return false;
	}

	case CLD:	// Clear Carry
	{
		m_oStatus.Flag.Decimal = false;
		return false;
	}

	case CLI:	// Clear interrupt
	{
		m_oStatus.Flag.Interrupt = false;
		return false;
	}

	case CLV:	// Clear Overflow
	{
		m_oStatus.Flag.Overflow = false;
		return false;
	}

	case CMP:	// Compare Accumulator and memory
	{
		m_oStatus.Flag.Carry = (m_uAcc >= m_uFetched);
		m_oStatus.Flag.Zero = (m_uAcc == m_uFetched);
		m_oStatus.Flag.Negative = BIT_(7, (m_uAcc - m_uFetched));

		return true;
	}

	case CPX:	// Compary X and Memory
	{
		m_oStatus.Flag.Carry = (m_uX >= m_uFetched);
		m_oStatus.Flag.Zero = (m_uX == m_uFetched);
		m_oStatus.Flag.Negative = BIT_(7, (m_uX - m_uFetched));

		return false;
	}

	case CPY:	// Compary Y and Memory
	{
		m_oStatus.Flag.Carry = (m_uY >= m_uFetched);
		m_oStatus.Flag.Zero = (m_uY == m_uFetched);
		m_oStatus.Flag.Negative = BIT_(7, (m_uY - m_uFetched));

		return false;
	}

	case DEC:	// Decrement memory by 1
	{
		BYTE result = --m_uFetched;
		
		m_oStatus.Flag.Zero = (result == 0x00);
		m_oStatus.Flag.Negative = BIT_(7, result);

		Write(m_uFetchedFrom, result);

		return false;
	}

	case DEX:	// Decrement X by 1
	{
		m_uX--;

		m_oStatus.Flag.Zero = (m_uX == 0x00);
		m_oStatus.Flag.Negative = BIT_(7, m_uX);

		return false;
	}

	case DEY:	// Decrement Y by 1
	{
		m_uY--;

		m_oStatus.Flag.Zero = (m_uY == 0x00);
		m_oStatus.Flag.Negative = BIT_(7, m_uY);

		return false;
	}

	case EOR:	// XOR
	{
		m_uAcc ^= m_uFetched;

		m_oStatus.Flag.Zero = (m_uAcc == 0x00);
		m_oStatus.Flag.Negative = BIT_(7, m_uAcc);

		return true;
	}

	case INC:	// Increment memory by 1
	{
		BYTE result = ++m_uFetched;

		m_oStatus.Flag.Zero = (result == 0x00);
		m_oStatus.Flag.Negative = BIT_(7, result);

		Write(m_uFetchedFrom, result);

		return false;
	}

	case INX:	// Increment X by 1
	{
		m_uX++;

		m_oStatus.Flag.Zero = (m_uX == 0x00);
		m_oStatus.Flag.Negative = BIT_(7, m_uX);

		return false;
	}

	case INY:	// Increment Y by 1
	{
		m_uY++;

		m_oStatus.Flag.Zero = (m_uY == 0x00);
		m_oStatus.Flag.Negative = BIT_(7, m_uY);

		return false;
	}

	case JMP:	// Jump to address
	{
		m_uPC = m_uFetchedFrom;

		return false;
	}
	
	case JSR:	// Jump to subroutine
	{
		m_uPC--;

		PushTo((m_uPC & 0xFF00) >> 8);
		PushTo(m_uPC & 0x00FF);

		m_uPC = m_uFetchedFrom;

		return false;
	}

	case LDA:	// Load Accumulator
	{
		m_uAcc = m_uFetched;

		m_oStatus.Flag.Zero = (m_uAcc == 0x00);
		m_oStatus.Flag.Negative = BIT_(7, m_uAcc);

		return true;
	}

	case LDX:	// Load X
	{
		m_uX = m_uFetched;

		m_oStatus.Flag.Zero = (m_uX == 0x00);
		m_oStatus.Flag.Negative = BIT_(7, m_uX);

		return true;
	}

	case LDY:	// Load Y
	{
		m_uY = m_uFetched;

		m_oStatus.Flag.Zero = (m_uY == 0x00);
		m_oStatus.Flag.Negative = BIT_(7, m_uY);

		return true;
	}

	case LSR:	// Logical shift right
	{
		WORD result = (WORD)m_uFetched >> 1;

		m_oStatus.Flag.Carry = BIT_(0, m_uFetched);
		m_oStatus.Flag.Zero = (result == 0x00);
		m_oStatus.Flag.Negative = BIT_(7, result);

		if (m_vecLookup[m_uOpcode].addressMode == ACC)
			m_uAcc = (result & 0x00FF);
		else
			Write(m_uFetchedFrom, result & 0x00FF);

		return false;
	}

	case NOP:	// No operation
	{
		return false;
	}

	case ORA:	// Logical inclusive OR
	{
		m_uAcc |= m_uFetched;

		m_oStatus.Flag.Zero = (m_uAcc == 0x00);
		m_oStatus.Flag.Negative = BIT_(7, m_uAcc);

		return true;
	}

	case PHA:	// Push accumulator
	{
		PushTo(m_uAcc);
		return false;
	}

	case PHP:	// Push status
	{
		PushTo(m_oStatus.Raw);
		return false;
	}

	case PLA:	// Pull accumulator
	{
		m_uAcc = PopFrom();

		m_oStatus.Flag.Zero = (m_uAcc == 0x00);
		m_oStatus.Flag.Negative = BIT_(7, m_uAcc);

		return false;
	}

	case PLP:	// Pull status
	{
		m_oStatus.Raw = PopFrom();
		return false;
	}

	case ROL:	// Rotate left
	{
		WORD result = (m_uFetched << 1);
		result |= m_oStatus.Flag.Carry;	// Set bit 0 to carry

		m_oStatus.Flag.Carry = BIT_(8, result);
		m_oStatus.Flag.Zero = (result == 0);
		m_oStatus.Flag.Negative = BIT_(7, result);

		if (m_vecLookup[m_uOpcode].addressMode == ACC)
			m_uAcc = (result & 0x00FF);
		else
			Write(m_uFetchedFrom, result & 0x00FF);

		return false;
	}

	case ROR:	// Rotate right
	{
		WORD result = (m_oStatus.Flag.Carry << 7);
		result |= (m_uFetched >> 1);

		m_oStatus.Flag.Carry = BIT_(0, m_uFetched);
		m_oStatus.Flag.Zero = (result == 0);
		m_oStatus.Flag.Negative = BIT_(7, result);

		if (m_vecLookup[m_uOpcode].addressMode == ACC)
			m_uAcc = (result & 0x00FF);
		else
			Write(m_uFetchedFrom, result & 0x00FF);

		return false;
	}

	case RTI:	// Return from interrupt
	{
		// Retrieve Status from stack
		m_oStatus.Raw = PopFrom();
		m_oStatus.Flag.Break = 0;

		// Retrieve PC from stack
		BYTE lo = PopFrom();
		BYTE hi = PopFrom();
		m_uPC = TO_WORD(lo, hi);

		return false;
	};

	case RTS:	// Return from subroutine
	{
		BYTE lo = PopFrom();
		BYTE hi = PopFrom();
		m_uPC = TO_WORD(lo, hi);
		m_uPC++;

		return false;
	}

	case SBC:	// ACC = ACC - M - C
	{
		WORD result = (WORD)m_uAcc - m_uFetched - (1 - m_oStatus.Flag.Carry);
		m_oStatus.Flag.Carry = false;

		m_oStatus.Flag.Carry = !BIT_(8, result);
		m_oStatus.Flag.Zero = ((result & 0x00FF) == 0x00);
		m_oStatus.Flag.Negative = BIT_(7, result);

		// If accumulator and fetched have the same sign AND the sum and fetched have a different sign
		// then we have overflow
		// Checks if the previous are both true ----------------------------------------------------|
		//        Checks if signs are different ------------------------------------|               |
		//            Checks if signs are equal ---|                                |               |
		//                                         V                                V               V
		//                        |--------------------------------|   |---------------------|   |----|
		m_oStatus.Flag.Overflow = (((WORD)m_uFetched ^ (WORD)m_uAcc) & (result ^ (WORD)m_uAcc) & 0x0080) >> 7;

		m_uAcc = result & 0x00FF;

		return true;
	}

	case SEC:	// Set Carry Flag
	{
		m_oStatus.Flag.Carry = true;

		return false;
	}

	case SED:	// Set Decimal Flag
	{
		m_oStatus.Flag.Decimal = true;

		return false;
	}

	case SEI:	// Set Interrupt Flag
	{
		m_oStatus.Flag.Interrupt = true;

		return false;
	}

	case STA:	// Store accumulator
	{
		Write(m_uFetchedFrom, m_uAcc);

		return false;
	}

	case STX:	// Store X
	{
		Write(m_uFetchedFrom, m_uX);

		return false;
	}

	case STY:	// Store Y
	{
		Write(m_uFetchedFrom, m_uY);

		return false;
	}

	case TAX:	// A -> X
	{
		m_uX = m_uAcc;

		m_oStatus.Flag.Zero = (m_uX == 0x00);
		m_oStatus.Flag.Negative = BIT_(7, m_uX);

		return false;
	}

	case TAY:	// A -> Y
	{
		m_uY = m_uAcc;

		m_oStatus.Flag.Zero = (m_uY == 0x00);
		m_oStatus.Flag.Negative = BIT_(7, m_uY);

		return false;
	}

	case TSX:	// SP -> X
	{
		m_uX = m_uSP;

		m_oStatus.Flag.Zero = (m_uX == 0x00);
		m_oStatus.Flag.Negative = BIT_(7, m_uX);

		return false;
	}

	case TXA:	// X -> A
	{
		m_uAcc = m_uX;

		m_oStatus.Flag.Zero = (m_uAcc == 0x00);
		m_oStatus.Flag.Negative = BIT_(7, m_uAcc);

		return false;
	}

	case TXS:	// X -> SP
	{
		m_uSP = m_uX;

		return false;
	}

	case TYA:	// Y -> A
	{
		m_uAcc = m_uY;

		m_oStatus.Flag.Zero = (m_uAcc == 0x00);
		m_oStatus.Flag.Negative = BIT_(7, m_uAcc);

		return false;
	}

	
	case ILL_AHX:	// ADDR = A & X & H
	{
		Write(m_uFetchedFrom, m_uAcc & m_uX & HI(m_uFetchedFrom));

		return false;
	}

	case ILL_ALR:	// AND + LSR
	{
		m_uAcc = m_uAcc & m_uFetched;
		WORD result = m_uAcc >> 1;

		m_oStatus.Flag.Carry = BIT_(0, result);
		m_oStatus.Flag.Zero = (result == 0x00);
		m_oStatus.Flag.Negative = BIT_(7, result);

		m_uAcc = result & 0x00FF;

		return false;
	}

	case ILL_ANC:	// A = A & IMM
	{
		m_uAcc = m_uAcc & m_uFetched;

		m_oStatus.Flag.Carry = BIT_(7, m_uAcc);
		m_oStatus.Flag.Negative = BIT_(7, m_uAcc);
		m_oStatus.Flag.Zero = (m_uAcc == 0x00);

		return false;
	}

	case ILL_ARR:	// AND + ROR
	{
		m_uAcc = m_uAcc & m_uFetched;
		WORD result = (m_oStatus.Flag.Carry << 7);
		result |= (m_uFetched >> 1);

		m_oStatus.Flag.Carry = BIT_(0, m_uFetched);
		m_oStatus.Flag.Zero = (result == 0);
		m_oStatus.Flag.Negative = BIT_(7, result);

		m_uAcc = result & 0x00FF;

		return false;
	}

	case ILL_AXS:	// X = A & X - ADDR
	{
		WORD result = (m_uAcc & m_uX) - m_uFetched;

		m_oStatus.Flag.Carry = BIT_(8, result);
		m_oStatus.Flag.Zero = ((result & 0xFF) == 0x00);
		m_oStatus.Flag.Negative = BIT_(7, result);

		m_uX = (result & 0x00FF);

		return false;
	}

	case ILL_DCP:	// ADDR = DEC + CMP
	{
		Write(m_uFetchedFrom, --m_uFetched);
		
		m_oStatus.Flag.Carry = (m_uAcc >= m_uFetched);
		m_oStatus.Flag.Zero = (m_uAcc == m_uFetched);
		m_oStatus.Flag.Negative = BIT_(7, (m_uAcc - m_uFetched));


		return false;
	}

	case ILL_ISC:	//ADDR = INC + SBC
	{
		Write(m_uFetchedFrom, ++m_uFetched);
		WORD result = (WORD)m_uAcc - m_uFetched - (1 - m_oStatus.Flag.Carry);
		m_oStatus.Flag.Carry = false;

		m_oStatus.Flag.Carry = !BIT_(8, result);
		m_oStatus.Flag.Zero = ((result & 0x00FF) == 0x00);
		m_oStatus.Flag.Negative = BIT_(7, result);

		m_oStatus.Flag.Overflow = (((WORD)m_uFetched ^ (WORD)m_uAcc) & (result ^ (WORD)m_uAcc) & 0x0080) >> 7;

		m_uAcc = result & 0x00FF;

		return false;
	}

	case ILL_JAM:	// Literally halts the CPU and ends the universe
	{
		m_isHalted = true;
		return false;
	}

	case ILL_LAS:	// SP, X, A = ADDR & SP
	{
		BYTE result = m_uFetched & m_uSP;

		m_oStatus.Flag.Negative = BIT_(7, result);
		m_oStatus.Flag.Zero = (result == 0x00);

		m_uAcc = result;
		m_uX = result;
		m_uSP = result;

		return true;
	}

	case ILL_LAX:	// A, X = Fetched
	{
		m_uAcc = m_uFetched;
		m_uX = m_uFetched;

		m_oStatus.Flag.Zero = (m_uFetched == 0x00);
		m_oStatus.Flag.Negative = BIT_(7, m_uFetched);

		return true;
	}

	case ILL_NOP:	// Illegal NOP
	{
		return true;
	}

	case ILL_RLA:	// ROL + AND
	{
		WORD result = (m_uFetched << 1);
		result |= m_oStatus.Flag.Carry;	// Set bit 0 to carry
		Write(m_uFetchedFrom, result);

		m_oStatus.Flag.Carry = BIT_(8, result);
		m_oStatus.Flag.Zero = ((result & 0x00FF) == 0);
		m_oStatus.Flag.Negative = BIT_(7, result);

		m_uAcc = m_uAcc & result;

		return false;
	}

	case ILL_RRA:	// ROR + ADC
	{
		WORD result = (m_oStatus.Flag.Carry << 7);
		result |= (m_uFetched >> 1);
		m_oStatus.Flag.Carry = BIT_(0, m_uFetched);
		Write(m_uFetchedFrom, result);

		WORD addition = m_uAcc + result + m_oStatus.Flag.Carry;

		m_oStatus.Flag.Carry = BIT_(8, addition);
		m_oStatus.Flag.Zero = ((addition & 0x00FF) == 0);
		m_oStatus.Flag.Negative = BIT_(7, addition);
		m_oStatus.Flag.Overflow = (~((WORD)result ^ (WORD)m_uAcc) & (addition ^ (WORD)m_uAcc) & 0x0080) >> 7;

		m_uAcc = addition & 0x00FF;

		return false;
	}

	case ILL_SAX:	// ADDR = A & X
	{
		Write(m_uFetchedFrom, m_uAcc & m_uX);

		return false;
	}

	case ILL_SBC:	// ACC - ADDR
	{
		WORD result = (WORD)m_uAcc - m_uFetched - (1 - m_oStatus.Flag.Carry);
		m_oStatus.Flag.Carry = false;

		m_oStatus.Flag.Carry = !BIT_(8, result);
		m_oStatus.Flag.Zero = ((result & 0x00FF) == 0x00);
		m_oStatus.Flag.Negative = BIT_(7, result);

		m_oStatus.Flag.Overflow = (((WORD)m_uFetched ^ (WORD)m_uAcc) & (result ^ (WORD)m_uAcc) & 0x0080) >> 7;

		m_uAcc = result & 0x00FF;

		return true;
	}

	case ILL_SHX:
	{
		Write(m_uFetchedFrom, m_uX & HI(m_uFetchedFrom));

		return false;
	}

	case ILL_SHY:	// ADDR = Y & H
	{
		Write(m_uFetchedFrom, m_uY & HI(m_uFetchedFrom));

		return false;
	}

	case ILL_SLO:	// ASL + ORA
	{
		WORD result = (WORD)m_uFetched << 1;
		Write(m_uFetchedFrom, result & 0x00FF);
		m_uAcc = m_uAcc | result;

		m_oStatus.Flag.Carry = BIT_(8, result);
		m_oStatus.Flag.Zero = (m_uAcc == 0);
		m_oStatus.Flag.Negative = BIT_(7, m_uAcc);

		return false;
	}

	case ILL_SRE:
	{
		WORD result = (WORD)m_uFetched >> 1;
		Write(m_uFetchedFrom, result);

		m_uAcc = m_uAcc ^ result;

		m_oStatus.Flag.Carry = BIT_(0, m_uFetched);
		m_oStatus.Flag.Zero = (m_uAcc == 0x00);
		m_oStatus.Flag.Negative = BIT_(7, m_uAcc);

		return false;
	}

	case ILL_TAS:	// SP = A & X. ADDR = SP & H (wtf?)
	{
		m_uSP = m_uAcc & m_uX;
		Write(m_uFetchedFrom, m_uSP & HI(m_uFetchedFrom));

		return false;
	}

	case ILL_XAA:	// TXA + AND
	{
		m_uAcc = m_uX & m_uFetched;

		m_oStatus.Flag.Negative = BIT_(7, m_uAcc);
		m_oStatus.Flag.Zero = (m_uAcc == 0x00);

		return false;
	}

	default:
		throw std::string("Unknown Opcode encountered (") + m_vecLookup[m_uOpcode].name + std::string(")");
		break;
	} 
}

BYTE Mos6502::Fetch()
{
	m_bSwitchedPage = false;
	m_uFetchedFrom = 0x0000;
	switch (m_vecLookup[m_uOpcode].addressMode)
	{
	case IMP:
		SERVE(0x00);

	case ACC:
		SERVE(m_uAcc);

	case IMM:
		m_uFetchedFrom = m_uPC++;
		SERVE(Read(m_uFetchedFrom));

	case ZPG:
		m_uFetchedFrom = (0x00FF & Read(m_uPC++));
		SERVE(Read(m_uFetchedFrom));

	case ABS:
		// I pray to god that the Read()'s are executed in order or else
		// the byte might be BE instead of LE
		m_uFetchedFrom = TO_WORD(Read(m_uPC + 0), Read(m_uPC + 1));
		m_uPC += 2;
		SERVE(Read(m_uFetchedFrom));

	case REL:
		SERVE(Read(m_uPC++));

	case IND:
	{
		BYTE lo = Read(m_uPC++);
		BYTE hi = Read(m_uPC++);
		m_uFetchedFrom = TO_WORD(Read(TO_WORD(lo, hi)), Read(TO_WORD((lo + 0x01) & 0xFF, hi)));
		SERVE(Read(m_uFetchedFrom));
	};

	case ZPX:
	{
		BYTE lo = Read(m_uPC++);
		m_bSwitchedPage = (lo + m_uX < lo);	// If lo + X is less than lo then we wrapped around
		m_uFetchedFrom = TO_WORD((lo + m_uX) & 0xFF, 0x00);
		SERVE(Read(m_uFetchedFrom));
	};

	case ZPY:
	{
		BYTE lo = Read(m_uPC++);
		m_bSwitchedPage = (lo + m_uY < lo);	// If lo + X is less than lo then we wrapped around
		m_uFetchedFrom = TO_WORD((lo + m_uY) & 0xFF, 0x00);
		SERVE(Read(m_uFetchedFrom));
	};

	case ABX:
	{
		BYTE lo = Read(m_uPC++);
		BYTE hi = Read(m_uPC++);
		m_uFetchedFrom = TO_WORD(lo, hi) + m_uX;
		m_bSwitchedPage = (((m_uFetchedFrom & 0xFF00) >> 8) != hi);
		SERVE(Read(m_uFetchedFrom));
	};

	case ABY:
	{
		BYTE lo = Read(m_uPC++);
		BYTE hi = Read(m_uPC++);
		m_uFetchedFrom = TO_WORD(lo, hi) + m_uY;
		m_bSwitchedPage = (((m_uFetchedFrom & 0xFF00) >> 8) != hi);
		SERVE(Read(m_uFetchedFrom));
	};

	case IDX:
	{
		BYTE offset = Read(m_uPC++);
		m_uFetchedFrom = TO_WORD(Read((offset + m_uX) & 0xFF), Read((offset + m_uX + 1) & 0xFF));
		SERVE(Read(m_uFetchedFrom));
	};

	case IDY:
	{
		BYTE offset = Read(m_uPC++);
		BYTE hi = Read((offset + 1) & 0xFF);
		m_uFetchedFrom = TO_WORD(Read(offset), hi) + m_uY;
		m_bSwitchedPage = (((m_uFetchedFrom & 0xFF00) >> 8) != hi);

		SERVE(Read(m_uFetchedFrom));
	};

	default:
		SERVE(0x00);
	}
}

