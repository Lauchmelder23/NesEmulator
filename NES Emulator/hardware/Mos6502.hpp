#pragma once
#include <functional>

#include "../util.hpp"

class Bus;

class Mos6502
{
public:
	typedef struct sFlags
	{
		BYTE Carry : 1 = 1;
		BYTE Zero : 1 = 1;
		BYTE Interrupt : 1 = 1;
		BYTE Decimal : 1 = 1;
		BYTE Break : 1 = 1;
		BYTE Unused : 1 = 1;
		BYTE Overflow : 1 = 1;
		BYTE Negative : 1 = 1;
	} Flags;

	typedef std::function<BYTE()> AddressMode;
	typedef std::function<BYTE()> Operation;

	struct Instruction
	{
		Operation operation		= nullptr;
		AddressMode addressMode = nullptr;
		BYTE cycles = 0;
	};

public:
	Mos6502();
	~Mos6502();

	void ConnectBus(Bus* bus) { m_pBus = bus; }

#pragma region Addressing Modes
	BYTE IMP();	// Implicit,	the destination is implied
	BYTE IMM();	// Immediate,	the operand is the value
	BYTE ZPG(); // Zero Page,	the value is at an 8-bit address on the zero page
	BYTE ABS();	// Absolute,	the value is at an address in memory
	BYTE REL(); // Relative,	8-bit signed offset from the PC
	BYTE IND(); // Indirect,	an address is stored at the address of the operand
	
	BYTE ZPX(); // Zero page indexed
	BYTE ZPY(); // Zero page indexed
	BYTE ABX(); // Absolute indexed
	BYTE ABY(); // Absolute indexed
	BYTE IDX();	// Indexed indirect
	BYTE IDY(); // Indirect indexed
#pragma endregion

#pragma region Opcodes
	BYTE ADC(); // Add with carry
	BYTE AND(); // Bitwise and
	BYTE ASL(); // Arithmetic <<
	BYTE BCC(); // Branch on carry clear
	BYTE BCS(); // Branch on carry set
	BYTE BEQ(); // Branch on zero set
	BYTE BIT(); // Bit test (?)
	BYTE BMI(); // Branch on negative set
	BYTE BNE(); // Branch on zero clear
	BYTE BPL(); // Branch on negative clear
	BYTE BRK(); // Break / Interrupt
	BYTE BVC(); // Branch on overflow clear
	BYTE BVS(); // Branch on overflow set
	BYTE CLC(); // Clear carry
	BYTE CLD(); // Clear decimal
	BYTE CLI(); // Clear interrupt disable
	BYTE CLV(); // Clear overflow
	BYTE CMP(); // Compare with accumulator
	BYTE CPX(); // Compare with X
	BYTE CPY(); // Compare with Y
	BYTE DEC(); // Decrement
	BYTE DEX(); // Decrement X
	BYTE DEY(); // Decrement Y
	BYTE EOR(); // XOR with accumulator
	BYTE INC(); // Increment
	BYTE INX(); // Increment X
	BYTE INY(); // Increment Y
	BYTE JMP(); // Jump
	BYTE JSR(); // Jump subroutine
	BYTE LDA(); // Load accumulator
	BYTE LDX(); // Load X
	BYTE LDY(); // Load Y
	BYTE LSR(); // Logical >>
	BYTE NOP(); // No operation
	BYTE ORA(); // OR with accumulator
	BYTE PHA(); // Push accumulator
	BYTE PHP(); // Push status register
	BYTE PLA(); // Pull accumulator
	BYTE PLP(); // Pull status register
	BYTE ROL(); // Rotate left
	BYTE ROR(); // Rotate right
	BYTE RTI(); // Return from interrupt
	BYTE RTS(); // Return from subroutine
	BYTE SBC(); // Subtract with carry
	BYTE SEC(); // Set carry
	BYTE SED(); // Set decimal
	BYTE SEI(); // Set interrupt disable
	BYTE STA(); // Store accumulator
	BYTE STX(); // Store X
	BYTE STY(); // Store Y
	BYTE TAX(); // Transfer accumulator to X
	BYTE TAY(); // Transfer accumulator to Y
	BYTE TSX(); // Transfer SP to X
	BYTE TXA(); // Transfer X to accumulator
	BYTE TXS(); // Transfer X to SP
	BYTE TYA(); // Transfer Y to accumulator

	BYTE UOP(); // Unknown Opcode
#pragma endregion
	
	void Tick();
	void Reset();
	void IRQ();
	void NMI();

private:
	BYTE Read(WORD address);
	void Write(WORD address, BYTE value);

	BYTE Fetch();

private:
	Bus* m_pBus;

	// Registers
	Flags m_oStatus;	// Status register (Flags)
	BYTE m_uAcc;		// Accumulator
	BYTE m_uX, m_uY;	// X/Y registers
	BYTE m_uSP;			// Stack pointer
	WORD m_uPC;			// Program counter

	BYTE m_uFetched;
	WORD m_uAbsAddress;
	int8_t m_nRelAddress;
	BYTE m_uOpcode, m_uCycles;

	std::vector<Instruction> m_vecLookup;
};
