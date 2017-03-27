#pragma once

#include <string>

class Instruction
{
public:
	virtual ~Instruction() {};
	virtual void Process( class Assembler& ass,const std::string& mne,std::string& rest,int line ) const = 0;
};