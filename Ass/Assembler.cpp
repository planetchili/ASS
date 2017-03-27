#include "Assembler.h"
#include "InstructionImpl.h"

void Assembler::RegisterOperations()
{
	RegisterInstruction<ImmediateJumpTemplate<0b011>>( "jmp" );
	RegisterInstruction<ImmediateJumpTemplate<0b000>>( "je" );
	RegisterAlias( "je","jz" );
	RegisterInstruction<ImmediateJumpTemplate<0b001>>( "ja" );
	RegisterInstruction<ImmediateJumpTemplate<0b010>>( "jb" );
	RegisterAlias( "jb","jnc" );
	RegisterInstruction<ImmediateJumpTemplate<0b100>>( "jne" );
	RegisterAlias( "jne","jnz" );
	RegisterInstruction<ImmediateJumpTemplate<0b101>>( "jna" );
	RegisterInstruction<ImmediateJumpTemplate<0b110>>( "jnb" );
	RegisterAlias( "jnb","jc" );
}