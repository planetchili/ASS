#pragma once

// this file must be included in ONLY ONE TL
// because of the the global strings

#include "Instruction.h"
#include "Assembler.h"

class AliasInstruction : public Instruction
{

};

template<unsigned int bits>
class ImmediateJumpTemplate : public Instruction
{
	static_assert((bits & ~0b111u) == 0u && bits != 0x111u,"bad bits in imm jmp");
public:
	virtual void Process( Assembler& ass,const std::string& mne,std::string& rest,int line ) const override
	{
		auto t = extract_token_white( rest );
		if( !t.has_value() )
		{
			// no destination name
			std::stringstream msg;
			msg << "Assembling instruction " << mne << " at line <" << line << ">! There is destination!";
			throw std::exception( msg.str().c_str() );
		}

		if( !is_name( t.value() ) )
		{
			// no valid destination name
			std::stringstream msg;
			msg << "Assembling instruction " << mne << " at line <" << line << ">! Invalid destination of [";
			msg << t.value() << "]!!";
			throw std::exception( msg.str().c_str() );
		}

		{
			auto garbage = extract_token_white( rest );
			if( garbage.has_value() )
			{
				// garbage after the dest name
				std::stringstream msg;
				msg << "Assembling instruction " << mne << " at line <" << line << ">! What is this garbage??? [";
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