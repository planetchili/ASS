#include "Assembler.h"
#include "InstructionImpl.h"

void Assembler::RegisterOperations()
{
	RegisterInstruction<JmpInstruction>();
}
