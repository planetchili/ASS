#pragma once

#include <string>

class Directive
{
public:
	virtual ~Directive() {};
	virtual void Process( class Assembler& ass,const std::string& dir,std::string rest,int line ) const = 0;
	virtual void Process( class Assembler& ass,const std::string& dir,std::string label,std::string rest,int line ) const
	{
		throw std::exception( (std::string( "Directive [." ) + dir + "] does not support labels [" + label + "]").c_str() );
	}
};