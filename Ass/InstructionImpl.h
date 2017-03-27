#pragma once

// this file must be included in ONLY ONE TL
// because of the the global strings

#include "Instruction.h"
#include "Assembler.h"

template<const char* name,unsigned int bits>
class ImmediateJumpTemplate : public Instruction
{
	static_assert((bits & ~0b111u) == 0u && bits != 0x111u,"bad bits in imm jmp");
	static_assert(name != nullptr,"give me a name in imm jmp");
public:
	virtual const char* GetName() const override { return name; }
	virtual void Process( Assembler& ass,std::string rest,int line ) const override
	{
		auto t = extract_token_white( rest );
		if( !t.has_value() )
		{
			// no destination name
			std::stringstream msg;
			msg << "Assembling instruction jmp at line <" << line << ">! There is destination!";
			throw std::exception( msg.str().c_str() );
		}

		if( !is_name( t.value() ) )
		{
			// no valid destination name
			std::stringstream msg;
			msg << "Assembling instruction jmp at line <" << line << ">! Invalid destination of [";
			msg << t.value() << "]!!";
			throw std::exception( msg.str().c_str() );
		}

		{
			auto garbage = extract_token_white( rest );
			if( garbage.has_value() )
			{
				// garbage after the dest name
				std::stringstream msg;
				msg << "Assembling instruction jmp at line <" << line << ">! What is this garbage??? [";
				msg << garbage.value() << "]!!";
				throw std::exception( msg.str().c_str() );
			}
		}

		// emit opcode byte
		ass.Emit( bits | 0x20u );

		// add the references
		ass.AddLabelReference( t.value(),ass.GetAddress(),line );

		// emit destination placeholder byte
		ass.Emit( 0xEEu );
	}
};

// consider template bullshit for string literal in future
char mne_jmp[] = "jmp";
class JmpInstruction : public ImmediateJumpTemplate<mne_jmp,0b011>
{};

char mne_je[] = "je";
class JeInstruction : public ImmediateJumpTemplate<mne_je,0b000>
{};

char mne_ja[] = "ja";
class JaInstruction : public ImmediateJumpTemplate<mne_ja,0b001>
{};

char mne_jb[] = "jb";
class JbInstruction : public ImmediateJumpTemplate<mne_jb,0b010>
{};

char mne_jne[] = "jne";
class JneInstruction : public ImmediateJumpTemplate<mne_jne,0b100>
{};

char mne_jna[] = "jna";
class JnaInstruction : public ImmediateJumpTemplate<mne_jna,0b101>
{};

char mne_jnb[] = "jnb";
class JnbInstruction : public ImmediateJumpTemplate<mne_jnb,0b110>
{};