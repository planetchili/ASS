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
	testCode << "_: ;comment" << std::endl;
	testCode << "" << std::endl;
	testCode << "def:" << std::endl;

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