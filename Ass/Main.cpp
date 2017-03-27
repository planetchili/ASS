#include <iostream>
#include <sstream>
#include <conio.h>
#include "Assembler.h"

int main( int argc,char* argv[] )
{
	// testing int literal detection
	{
		std::string num = "31";
		std::cout << num << ":" << std::boolalpha << is_int_literal( num ) << std::endl;
	}
	{
		std::string num = "0x31";
		std::cout << num << ":" << std::boolalpha << is_int_literal( num ) << std::endl;
	}
	{
		std::string num = "0b011";
		std::cout << num << ":" << std::boolalpha << is_int_literal( num ) << std::endl;
	}
	{
		std::string num = "0b3";
		std::cout << num << ":" << std::boolalpha << is_int_literal( num ) << std::endl;
	}
	{
		std::string num = "0ne4";
		std::cout << num << ":" << std::boolalpha << is_int_literal( num ) << std::endl;
	}
	{
		std::string num = "0";
		std::cout << num << ":" << std::boolalpha << is_int_literal( num ) << std::endl;
	}
	{
		std::string num = "a1";
		std::cout << num << ":" << std::boolalpha << is_int_literal( num ) << std::endl;
	}
	{
		std::string num = "1a";
		std::cout << num << ":" << std::boolalpha << is_int_literal( num ) << std::endl;
	}
	{
		std::string num = "0x";
		std::cout << num << ":" << std::boolalpha << is_int_literal( num ) << std::endl;
	}
	{
		std::string num = "0b";
		std::cout << num << ":" << std::boolalpha << is_int_literal( num ) << std::endl;
	}
	{
		std::string num = "00";
		std::cout << num << ":" << std::boolalpha << is_int_literal( num ) << std::endl;
	}


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
	testCode << "jnz label ;this will work ;)" << std::endl;
	// testCode << "jc lobbo dd ;this will work ;)" << std::endl;

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