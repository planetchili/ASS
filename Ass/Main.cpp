#include <iostream>
#include <sstream>
#include <conio.h>
#include "Assembler.h"

int main( int argc,char* argv[] )
{
	std::stringstream testCode;
	testCode << "; comment" << std::endl;
	testCode << "label:" << std::endl;
	testCode << "   mab_3le:    " << std::endl;
	testCode << "_1: ;comment" << std::endl;
	testCode << "" << std::endl;
	testCode << "def:" << std::endl;
	testCode << "jmp label ;this will work ;)" << std::endl;
	testCode << "jmp _1 ;this will work ;)" << std::endl;
	// testCode << "jmp dicks ;this will not" << std::endl;
	// testCode << "jmp label idi ;this will not" << std::endl;
	testCode << "lobo:" << std::endl;
	testCode << "jmp lobo ;this will work ;)" << std::endl;
	testCode << "jne label ;this will work ;)" << std::endl;
	testCode << "jb lobo ;this will work ;)" << std::endl;

	try
	{
		Assembler a( testCode );
		a.Assemble( "out.txt" );
	}
	catch( const std::exception& e )
	{
		std::cout << "Fatal error: " << e.what() << std::endl;
	}

	while( !_kbhit() );
	return 0;
}