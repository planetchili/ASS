#pragma once

#include "Directive.h"
#include "Assembler.h"

class DefineByteDirective : public Directive
{
public:
	virtual void Process( Assembler& ass,const std::string& dir,std::string rest,int line ) const override
	{
		auto p = extract_token_white( rest );
		if( !p.has_value() )
		{
			// no data
			std::stringstream msg;
			msg << "Processing directive " << dir << " at line <" << line << ">! No data!";
			throw std::exception( msg.str().c_str() );
		}
		
		// check for garbage after directive
		{
			auto garbage = extract_token_white( rest );
			if( garbage.has_value() )
			{
				std::stringstream msg;
				msg << "Processing directive " << dir << " at line <" << line << ">! What is this garbage??? [";
				msg << garbage.value() << "]!!";
				throw std::exception( msg.str().c_str() );
			}
		}

		if( p.value() == "?" )
		{
			ass.Emit( 0xEEu );
		}

		const auto lit_type = int_literal_type( p.value() );
		if( lit_type != IntLiteralType::Not )
		{
			ass.Emit( parse_int_literal( p.value(),lit_type ) );
		}
		else
		{
			// bad value
			std::stringstream msg;
			msg << "Processing directive " << dir << " at line <" << line << ">! What is this garbage??? [";
			msg << p.value() << "]!!";
			throw std::exception( msg.str().c_str() );
		}
	}
	virtual void Process( Assembler& ass,const std::string& dir,std::string label,std::string rest,int line ) const override
	{
		// add variable to symbol table
		ass.AddVariable( label,ass.GetAddress(),line );
		Process( ass,dir,rest,line );
	}
};

class OriginDirective : public Directive
{
public:
	virtual void Process( Assembler& ass,const std::string& dir,std::string rest,int line ) const override
	{
		auto p = extract_token_white( rest );
		if( !p.has_value() )
		{
			// no data
			std::stringstream msg;
			msg << "Processing directive " << dir << " at line <" << line << ">! No data!";
			throw std::exception( msg.str().c_str() );
		}

		// check for garbage after directive
		{
			auto garbage = extract_token_white( rest );
			if( garbage.has_value() )
			{
				std::stringstream msg;
				msg << "Processing directive " << dir << " at line <" << line << ">! What is this garbage??? [";
				msg << garbage.value() << "]!!";
				throw std::exception( msg.str().c_str() );
			}
		}

		const auto lit_type = int_literal_type( p.value() );
		if( lit_type != IntLiteralType::Not )
		{
			ass.SetAddress( parse_int_literal( p.value() ) );
		}
		else
		{
			// bad value
			std::stringstream msg;
			msg << "Processing directive " << dir << " at line <" << line << ">! What is this garbage??? [";
			msg << p.value() << "]!!";
			throw std::exception( msg.str().c_str() );
		}
	}
};