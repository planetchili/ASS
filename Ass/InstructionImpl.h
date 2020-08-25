#pragma once

// this file must be included in ONLY ONE TL
// because of the the global strings

#include "Instruction.h"
#include "Assembler.h"

template<unsigned char param_bits>
class ImmediateJumpTemplate : public Instruction
{
	static_assert((param_bits & ~0b111u) == 0u && param_bits != 0x111u,"bad bits in imm jmp");
	static constexpr unsigned char opcode_domain = 0b00100000u;
public:
	virtual void Process( Assembler& ass,const std::string& mne,std::string& rest,int line ) const override
	{
		auto t = extract_token_white( rest );
		if( !t.has_value() )
		{
			// no destination name
			std::stringstream msg;
			msg << "Assembling instruction " << mne << " at line <" << line << ">! There is no destination!";
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
		ass.Emit( opcode_domain | param_bits );

		// add the references
		ass.AddLabelReference( t.value(),ass.GetAddress(),line );

		// emit destination placeholder byte
		ass.Emit( 0xEEu );
	}
};

template<unsigned char opcode_domain>
class StandardInstructionTemplate : public Instruction
{
	static_assert((opcode_domain & 0b11u) == 0u,"bad opcode domain bits in std inst");
public:
	virtual void Process( Assembler& ass,const std::string& mne,std::string& rest,int line ) const override
	{
		auto d = extract_token_white( rest );
		if( !d.has_value() )
		{
			// no destination
			std::stringstream msg;
			msg << "Assembling instruction " << mne << " at line <" << line << ">! There is no source or destination!";
			throw std::exception( msg.str().c_str() );
		}

		if( !is_register_name( d.value() ) )
		{
			// no valid destination name
			std::stringstream msg;
			msg << "Assembling instruction " << mne << " at line <" << line << ">! Invalid destination register name [";
			msg << d.value() << "]!!";
			throw std::exception( msg.str().c_str() );
		}

		if( !try_consume_comma( rest ) )
		{
			std::stringstream msg;
			msg << "Assembling instruction " << mne << " at line <" << line << ">! Expected comma (',') found [";
			msg << extract_token_white( rest ).value_or( "" ) << "]!!";
			throw std::exception( msg.str().c_str() );
		}
		
		auto s = extract_token_white( rest );
		if( !s.has_value() )
		{
			// no destination
			std::stringstream msg;
			msg << "Assembling instruction " << mne << " at line <" << line << ">! There is no source!";
			throw std::exception( msg.str().c_str() );
		}
		
		// check for garbage after source param
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
		
		ProcessParams( ass,mne,d.value(),s.value(),line );
	}
	void ProcessParams( Assembler& ass,const std::string& mne,std::string& d,std::string& s,int line ) const
	{
		auto s_int_type = int_literal_type( s );
		bool s_is_address_ref = try_strip_address( s );
		if( s_int_type != IntLiteralType::Not ) // source is int literal
		{
			// operate on register with int literal
			unsigned char param_bits = 0u;
			if( d == "b" )
			{
				param_bits |= 0b11;
			}
			// emit opcode byte
			ass.Emit( opcode_domain | param_bits );
			// emit immediate parameter byte
			ass.Emit( parse_int_literal( s,s_int_type ) );
		}
		else if( s_is_address_ref )
		{
			// load immediate address of variable
			unsigned char param_bits = 0u;
			if( d == "b" )
			{
				param_bits |= 0b11;
			}
			// emit opcode byte
			ass.Emit( opcode_domain | param_bits );
			// add the symbol reference that will resolve and fill the placeholder
			ass.AddVariableReference( s,ass.GetAddress(),line );
			// emit immediate parameter placeholder byte
			ass.Emit( 0xEEu );
		}
		else if( is_register_name( s ) ) // source is register
		{
			if( s == d )
			{
				// source may not equal destination
				std::stringstream msg;
				msg << "Assembling instruction " << mne << " at line <" << line << ">! Source cannot be same as destination!";
				throw std::exception( msg.str().c_str() );
			}

			// operate on register with register
			unsigned char param_bits = 0u;
			if( d == "b" )
			{
				param_bits |= 0b10;
			}
			if( s == "b" )
			{
				param_bits |= 0b01;
			}
			// emit opcode byte
			ass.Emit( opcode_domain | param_bits );
		}
		else // bad source name
		{
			// garbage after the dest name
			std::stringstream msg;
			msg << "Assembling instruction " << mne << " at line <" << line << ">! Bad source [";
			msg << s << "]!!";
			throw std::exception( msg.str().c_str() );
		}
	}
};

class MoveInstruction : public Instruction
{
	static constexpr unsigned char opcode_domain_ind_src = 0b00000100;
	static constexpr unsigned char opcode_domain_ind_dst = 0b00001000;
public:
	virtual void Process( Assembler& ass,const std::string& mne,std::string& rest,int line ) const override
	{
		auto d = extract_token_white( rest );
		if( !d.has_value() )
		{
			// no destination
			std::stringstream msg;
			msg << "Assembling instruction " << mne << " at line <" << line << ">! There is no source or destination!";
			throw std::exception( msg.str().c_str() );
		}

		const bool d_is_indirect = try_strip_indirect( d.value() );
		if( !is_register_name( d.value() ) )
		{
			// no valid destination name
			std::stringstream msg;
			msg << "Assembling instruction " << mne << " at line <" << line << ">! Invalid destination register name [";
			msg << d.value() << "]!!";
			throw std::exception( msg.str().c_str() );
		}

		if( !try_consume_comma( rest ) )
		{
			std::stringstream msg;
			msg << "Assembling instruction " << mne << " at line <" << line << ">! Expected comma (',') found [";
			msg << extract_token_white( rest ).value_or( "" ) << "]!!";
			throw std::exception( msg.str().c_str() );
		}

		auto s = extract_token_white( rest );
		if( !s.has_value() )
		{
			// no destination
			std::stringstream msg;
			msg << "Assembling instruction " << mne << " at line <" << line << ">! There is no source!";
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
		
		const bool s_is_indirect = try_strip_indirect( s.value() );
		if( s_is_indirect && d_is_indirect )
		{
			// src dst both indirect
			std::stringstream msg;
			msg << "Assembling instruction " << mne << " at line <" << line << ">! Source and dest cannot both be indirect!!";
			throw std::exception( msg.str().c_str() );
		}

		if( !(s_is_indirect || d_is_indirect) )
		{
			directMov.ProcessParams( ass,mne,d.value(),s.value(),line );
		}
		else if( d_is_indirect ) // store to memory indirect
		{
			auto s_int_type = int_literal_type( s.value() );
			if( s_int_type != IntLiteralType::Not ) // source is int literal
			{
				std::stringstream msg;
				msg << "Assembling instruction " << mne << " at line <" << line << ">! Invalid source no literals allowed in indirect mov!! [";
				msg << s.value() << "]!!";
				throw std::exception( msg.str().c_str() );
			}
			else if( is_register_name( s.value() ) )
			{
				// register source
				unsigned char param_bits = 0u;
				if( d.value() == "b" )
				{
					param_bits |= 0b10;
				}
				if( s.value() == "b" )
				{
					param_bits |= 0b01;
				}
				// emit opcode byte
				ass.Emit( opcode_domain_ind_dst | param_bits );
			}
			else
			{
				std::stringstream msg;
				msg << "Assembling instruction " << mne << " at line <" << line << ">! Invalid source register name [";
				msg << s.value() << "]!!";
				throw std::exception( msg.str().c_str() );
			}
		}
		else // indirect source
		{
			// register source
			unsigned char param_bits = 0u;
			if( d.value() == "b" )
			{
				param_bits |= 0b10;
			}
			if( s.value() == "b" )
			{
				param_bits |= 0b01;
			}
			// emit opcode byte
			ass.Emit( opcode_domain_ind_src | param_bits );			
		}
	}
private:
	StandardInstructionTemplate<0b00000000> directMov;
};