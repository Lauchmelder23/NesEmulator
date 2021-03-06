#include "Mos6502.hpp"
#include "Bus.hpp"

#include <sstream>
#include <iomanip>

#define MAKE(x, y, z) {#x, x, y, z}
#define SERVE(x) return m_uFetched = x
#define TO_WORD(lo, hi) ((static_cast<WORD>(hi) << 8) | lo)
#define HEX(p, x, w) p << std::setfill('0') << std::setw(w) << std::hex << std::uppercase << (WORD)x

#define Push(x) Write(0x0100 + (m_uSP--), x)
#define Pop() Read(0x0100 + (++m_uSP))

Mos6502::Mos6502() :
	m_pBus(nullptr),
	m_uAcc(0x00), m_uX(0x00), m_uY(0x00), m_uSP(0x00), m_uPC(0x0000),
	m_uFetched(0x00), m_uOpcode(0x00), m_uCycles(0x00), m_uCyclesTotal(0)
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

	// Reset();
}

Mos6502::~Mos6502()
{
	// Don't delete m_pBus, it might be used elsewhere
}


void Mos6502::Tick()
{
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

void Mos6502::Reset()
{
	m_uAcc = 0x00;
	m_uX = 0x00;
	m_uY = 0x00;
	m_uSP = 0xFD;
	m_oStatus.Raw = 0b00000000;
	m_oStatus.Flag.Unused = true;

	m_uPC = TO_WORD(Read(0xFFFC), Read(0xFFFD));	// 0xFFFC is the reset vector

	m_uFetched = 0x00;
	m_uCycles = 0;
}

void Mos6502::IRQ()
{
	if (!m_oStatus.Flag.Interrupt)
	{
		// Push current PC to stack
		Push((m_uPC & 0xFF00) >> 8);
		Push(m_uPC & 0x00FF);

		// Push status to stack
		m_oStatus.Flag.Break = false;
		m_oStatus.Flag.Unused = true;
		m_oStatus.Flag.Interrupt = true;
		Push(m_oStatus.Raw);	// TODO: Untested, might break

		// Get new PC from Interrupt vector 0xFFFE
		m_uPC = TO_WORD(Read(0xFFFE), Read(0xFFFF));

		m_uCycles = 7;
	}
}

void Mos6502::NMI()
{
	// Push current PC to stack
	Push((m_uPC & 0xFF00) >> 8);
	Push(m_uPC & 0x00FF);

	// Push status to stack
	m_oStatus.Flag.Break = false;
	m_oStatus.Flag.Unused = true;
	m_oStatus.Flag.Interrupt = true;
	Push(m_oStatus.Raw);	// TODO: Untested, might break

	// Get new PC from Interrupt vector 0xFFFE
	m_uPC = TO_WORD(Read(0xFFFA), Read(0xFFFB));

	m_uCycles = 7;
}

std::map<WORD, std::string> Mos6502::Disassemble(WORD begin, WORD end)
{
	// Disassembles the RAM from $begin to $end
	std::map<WORD, std::string> disassemble;

	for (WORD ptr = begin; ptr <= end;)
	{
		Instruction i = m_vecLookup[Read(ptr)];
		std::stringstream ss;
		ss << HEX("$", ptr, 4) << "  ";

		// Do different things depending on addressing mode
		// Very ugly, but it works I guess. It's okay, this code doesn't need to be efficient

		BYTE op = Read(ptr);
		BYTE lo = Read(ptr + 1);
		BYTE hi = Read(ptr + 2);
		switch (i.addressMode)
		{
		case IMP:
			ss << HEX("", op, 2) << "\t\t  " << i.name << " \t";
			ptr++;
			break;

		case ACC:
			ss << HEX("", op, 2) << "\t\t  " << i.name << " ";
			ss << " %acc";
			ptr++;
			break;

		case IMM:
			ss << HEX("", op, 2) << " " << HEX("", lo, 2) << "\t  " << i.name << " ";
			ss << HEX("#$", lo, 2);
			ptr += 2;
			break;

		case ZPG:
			ss << HEX("", op, 2) << " " << HEX("", hi, 2) << "\t  " << i.name << " ";
			ss << HEX("$00", lo, 4);
			ptr += 2;
			break;

		case ABS:
		{
			ss << HEX("", op, 2) << " " << HEX("", lo, 2)
				<< " " << HEX("", hi, 2) << "\t  " << i.name << " ";
			//BYTE lo = Read(++opcodeAddress);
			//BYTE hi = Read(++opcodeAddress);
			// WORD test = TO_WORD(Read(++opcodeAddress), Read(++opcodeAddress));
			ss << HEX("$", TO_WORD(lo, hi), 4);
			ptr += 3;
		} break;

		case REL:
			ss << HEX("", op, 2) << " " << HEX("", lo, 2) << "\t  " << i.name << " ";
			ss << HEX("$", ptr + (int8_t)lo + 2, 4);
			ptr += 2;
			break;

		case IND:
			ss << HEX("", op, 2) << " " << HEX("", lo, 2)
				<< " " << HEX("", hi, 2) << "\t  " << i.name << " ";
			ss << "[" << HEX("$", TO_WORD(lo, hi), 4) << "]";
			ptr += 3;
			break;

		case ZPX:
			ss << HEX("", op, 2) << " " << HEX("", lo, 2) << "\t  " << i.name << " ";
			ss << "(" << HEX("$00", lo, 4) << " + X)";
			ptr += 2;
			break;

		case ZPY:
			ss << HEX("", op, 2) << " " << HEX("", lo, 2) << "\t  " << i.name << " ";
			ss << "(" << HEX("$00", lo, 4) << " + Y)";
			ptr += 2;
			break;

		case ABX:
			ss << HEX("", op, 2) << " " << HEX("", lo, 2)
				<< " " << HEX("", hi, 2) << "\t  " << i.name << " ";
			ss << "(" << HEX("$", TO_WORD(lo, hi), 4) << " + X)";
			ptr += 3;
			break;

		case ABY:
			ss << HEX("", op, 2) << " " << HEX("", lo, 2)
				<< " " << HEX("", hi, 2) << "\t  " << i.name << " ";
			ss << "(" << HEX("$", TO_WORD(lo, hi), 4) << " + Y)";
			ptr += 3;
			break;

		case IDX:
			ss << HEX("", op, 2) << " " << HEX("", lo, 2)
				<< " " << HEX("", hi, 2) << "\t  " << i.name << " ";
			ss << "[" << HEX("$", TO_WORD(lo, hi), 4) << " + X]";
			ptr += 3;
			break;

		case IDY:
			ss << HEX("", op, 2) << " " << HEX("", lo, 2)
				<< " " << HEX("", hi, 2) << "\t  " << i.name << " ";
			ss << "([" << HEX("$", TO_WORD(lo, hi), 4) << "] + Y)";
			ptr += 3;
			break;

		default:
			ss << "(Unknown addressing mode)";
			ptr++;
			break;
		}

		disassemble.insert(std::make_pair(ptr, ss.str()));
	}

	return disassemble;
}

BYTE Mos6502::Read(WORD address)
{
	return m_pBus->Read(address);
}

void Mos6502::Write(WORD address, BYTE value)
{
	m_pBus->Write(address, value);
}

bool Mos6502::Execute()
{
	/*
	std::cout << std::hex << "$" << m_uPC <<
		": " << m_vecLookup[m_uOpcode].name << 
		"   (" << std::hex << static_cast<WORD>(m_uOpcode) << ")" << std::endl;
	*/
	switch (m_vecLookup[m_uOpcode].operation)
	{
	case AND:	// ACC = ACC & Memory
	{
		m_uAcc = m_uAcc & m_uFetched;

		// Set Flags if needed
		m_oStatus.Flag.Zero = (m_uAcc == 0x00);
		m_oStatus.Flag.Negative = (m_uAcc & 0x80);

		return true;
	}

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
	}

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
	}

	case BEQ:	// Branch on equal (Z = 1)
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
	}

	case RTI:	// Return from interrupt
	{
		// Retrieve Status from stack
		m_oStatus.Raw = Pop();
		m_oStatus.Raw &= ~m_oStatus.Flag.Break;
		m_oStatus.Raw &= ~m_oStatus.Flag.Unused;

		// Retrieve PC from stack
		m_uPC = TO_WORD(Pop(), Pop());

		return false;
	}

	default:
		throw "Unknown Opcode encountered";
	}
}

BYTE Mos6502::Fetch()
{
	m_bSwitchedPage = false;
	switch (m_vecLookup[m_uOpcode].addressMode)
	{
	case IMP:
		SERVE(0x00);

	case ACC:
		SERVE(m_uAcc);

	case IMM:
		SERVE(Read(m_uPC++));

	case ZPG:
		SERVE(Read(0x00FF & Read(m_uPC++)));

	case ABS:
		// I pray to god that the Read()'s are executed in order or else
		// the byte might be BE instead of LE
		SERVE(Read(
			TO_WORD(Read(m_uPC++), Read(m_uPC++))
		));

	case REL:
		SERVE(Read(m_uPC++));

	case IND:
	{
		BYTE lo = Read(m_uPC++);
		BYTE hi = Read(m_uPC++);
		SERVE(Read(
			TO_WORD(Read(TO_WORD(hi, lo + 0x01)), Read(TO_WORD(hi, lo)))
		));
	};

	case ZPX:
	{
		BYTE lo = Read(m_uPC++);
		m_bSwitchedPage = (lo + m_uX <= lo);	// If lo + X is less than lo then we wrapped around

		SERVE(Read(TO_WORD(lo + m_uX, 0x00)));
	};

	case ZPY:
	{
		BYTE lo = Read(m_uPC++);
		m_bSwitchedPage = (lo + m_uY <= lo);	// If lo + X is less than lo then we wrapped around

		SERVE(Read(TO_WORD(lo + m_uY, 0x00)));
	};

	case ABX:
	{
		BYTE lo = Read(m_uPC++);
		BYTE hi = Read(m_uPC++);
		WORD addr = TO_WORD(lo, hi) + m_uX;
		m_bSwitchedPage = (((addr & 0xFF00) >> 8) != hi);
		SERVE(Read(addr));
	};

	case ABY:
	{
		BYTE lo = Read(m_uPC++);
		BYTE hi = Read(m_uPC++);
		WORD addr = TO_WORD(lo, hi) + m_uY;
		m_bSwitchedPage = (((addr & 0xFF00) >> 8) != hi);
		SERVE(Read(addr));
	};

	case IDX:
	{
		BYTE offset = Read(m_uPC++);
		SERVE(Read(TO_WORD(Read(offset + m_uX), Read(offset + m_uX + 1))));
	};

	case IDY:
	{
		BYTE offset = Read(m_uPC++);
		BYTE lo = Read(offset);
		BYTE hi = Read(offset + 1);

		WORD addr = TO_WORD(lo, hi) + m_uY;
		m_bSwitchedPage = (((addr & 0xFF00) >> 8) != hi);

		SERVE(Read(addr));
	};

	default:
		SERVE(0x00);
	}
}

