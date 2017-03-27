#pragma once

#include <string>

class Instruction
{
public:
	virtual ~Instruction() {};
	virtual void Process( class Assembler& ass,const std::string& mne,std::string& rest,int line ) const = 0;
};

class AliasInstruction : public Instruction
{
public:
	AliasInstruction( const Instruction* pMain )
		:
		pMain( pMain )
	{}
	virtual void Process( class Assembler& ass,const std::string& mne,std::string& rest,int line ) const override
	{
		pMain->Process( ass,mne,rest,line );
	}
private:
	const Instruction* const pMain;
};