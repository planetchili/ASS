#pragma once

#include <string>
#include "Parsing.h"

class Instruction
{
public:
	virtual ~Instruction() = 0;
	virtual void Process( class Assembler& ass,std::string rest,int line ) const = 0;
	virtual const char* GetName() const = 0;
	bool operator<( const Instruction& other ) const
	{
		return GetName() < other.GetName();
	}
};


class JumpInstruction : public Instruction
{
public:
	virtual const char* GetName() const override { return name; }
	virtual void Process( class Assembler& ass,std::string rest,int line ) const override;
public:
	static constexpr char* name = "jmp";
};


//class JumpInstruction : public Instruction
//{
//public:
//	virtual const char* GetName() const override { return name; }
//	virtual void Process( class Assembler& ass,std::string rest,int line ) const override
//	{
//		
//	}
//public:
//	static constexpr char* name = "jmp";
//};