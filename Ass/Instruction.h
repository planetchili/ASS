#pragma once

#include <string>

class Instruction
{
public:
	virtual ~Instruction() {};
	virtual void Process( class Assembler& ass,std::string rest,int line ) const = 0;
	virtual const char* GetName() const = 0;
	bool operator<( const Instruction& other ) const
	{
		return GetName() < other.GetName();
	}
};