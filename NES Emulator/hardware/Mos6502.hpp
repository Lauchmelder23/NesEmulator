#pragma once
#include <functional>
#include <map>
#include <sstream>

#include "../util.hpp"

///////////////// Internal values for opcodes //////////////////
#pragma region Opcodes
#define ADC 0x00
#define AND	0x01
#define ASL	0x02
#define BCC	0x03
#define BCS	0x04
#define BEQ	0x05
#define BIT	0x06
#define BMI	0x07
#define BNE	0x08
#define BPL	0x09
#define BRK	0x0A
#define BVC	0x0B
#define BVS	0x0C
#define CLC	0x0D
#define CLD	0x0E
#define CLI	0x0F
#define CLV	0x10
#define CMP	0x11
#define CPX	0x12
#define CPY	0x13
#define DEC	0x14
#define DEX	0x15
#define DEY	0x16
#define EOR	0x17
#define INC	0x18
#define INX	0x19
#define INY	0x1A
#define JMP	0x1B
#define JSR	0x1C
#define LDA	0x1D
#define LDX	0x1E
#define LDY	0x1F
#define LSR	0x20
#define NOP	0x21
#define ORA	0x22
#define PHA	0x23
#define PHP	0x24
#define PLA	0x25
#define PLP	0x26
#define ROL	0x27
#define ROR	0x28
#define RTI	0x29
#define RTS	0x2A
#define SBC	0x2B
#define SEC	0x2C
#define SED	0x2D
#define SEI	0x2E
#define STA	0x2F
#define STX	0x30
#define STY	0x31
#define TAX	0x32
#define TAY	0x33
#define TSX	0x34
#define TXA	0x35
#define TXS	0x36
#define TYA	0x37
#define UOP 0xFF
#pragma endregion 

///////////////// Internal values for addressing modes /////////////////
#pragma region AddrModes
#define IMP 0x00
#define ACC 0x01
#define IMM 0x02
#define ZPG 0x03
#define ABS 0x04
#define REL 0x05
#define IND 0x06
#define ZPX 0x07
#define ZPY 0x08
#define ABX 0x09
#define ABY 0x0A
#define IDX 0x0B
#define IDY 0x0C
#pragma endregion

class Bus;

class Mos6502
{
public:
	typedef union Status
	{
		BYTE Raw;

		struct sFlags
		{

			BYTE Carry : 1;
			BYTE Zero : 1;
			BYTE Interrupt : 1;
			BYTE Decimal : 1;
			BYTE Break : 1;
			BYTE Unused : 1;
			BYTE Overflow : 1;
			BYTE Negative : 1;
		} Flag;

		std::string AsString()
		{
			std::stringstream ss;
			ss << (Flag.Negative ? "N" : "n")
				<< (Flag.Overflow ? "O" : "o")
				<< (Flag.Unused ? "U" : "u")
				<< (Flag.Break ? "B" : "b") << " "
				<< (Flag.Decimal ? "D" : "d")
				<< (Flag.Interrupt ? "I" : "i")
				<< (Flag.Zero ? "Z" : "z")
				<< (Flag.Carry ? "C" : "c");
			return ss.str();
		}
	} Status;

	typedef BYTE AddressMode;
	typedef BYTE Operation;

	struct Instruction
	{
		std::string name		= "";
		Operation operation		= 0xFF;
		AddressMode addressMode = 0xFF;
		BYTE cycles = 0;
	};

public:
	Mos6502();
	~Mos6502();

	void ConnectBus(Bus* bus) { m_pBus = bus; }
	bool Done() { return (m_uCycles == 0); }
	
	void Tick();
	void Reset();
	void IRQ();	// Interrupt Request
	void NMI(); // Non-maskable interrupt

	std::map<WORD, std::string> Disassemble(WORD begin, WORD end);
	uint64_t GetCycles() const { return m_uCyclesTotal; }

	// Registers
	Status m_oStatus;	// Status register (Flags)
	BYTE m_uAcc;		// Accumulator
	BYTE m_uX, m_uY;	// X/Y registers
	BYTE m_uSP;			// Stack pointer
	WORD m_uPC;			// Program counter

private:
	BYTE Read(WORD address);
	void Write(WORD address, BYTE value);

	bool Execute();
	BYTE Fetch();

private:
	Bus* m_pBus;

	WORD m_uFetchedFrom;
	BYTE m_uFetched;
	bool m_bSwitchedPage;
	BYTE m_uOpcode, m_uCycles;
	uint64_t m_uCyclesTotal;

	std::vector<Instruction> m_vecLookup;
};
