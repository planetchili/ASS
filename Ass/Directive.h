#pragma once

#include <string>

class Directive
{
public:
	virtual ~Directive() = 0;
	virtual void Process( class Assembler& ass,std::string rest,int line ) const = 0;
	virtual void Process( class Assembler& ass,std::string label,std::string rest,int line ) const
	{
		throw std::exception( (std::string( "Directive [" ) + GetName() + "] does not support labels [" + label + "]").c_str() );
	}
	virtual const char* GetName() const = 0;
	bool operator<( const Directive& other ) const
	{
		return GetName() < other.GetName();
	}
};