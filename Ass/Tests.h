#pragma once

#include <iostream>
#include <sstream>
#include <conio.h>
#include "Assembler.h"
#include "Classify.h"

void test()
{
	std::stringstream testCode;
	//testCode << "foo .org 89" << std::endl;
	testCode << ".org 278" << std::endl;
	testCode << "label:" << std::endl;
	testCode << "mov a,[a]" << std::endl;
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
	testCode << "add b  ,  0x69" << std::endl;
	testCode << ".org 3" << std::endl;
	//testCode << "sub b ,0x27 ad" << std::endl;
	//testCode << "add a," << std::endl;
	testCode << "sub b,a" << std::endl;
	//testCode << "mov a 0x42" << std::endl;
	//testCode << "mov [a],0x11" << std::endl;
	testCode << "mov [a],b" << std::endl;
	testCode << "mov b,[a]" << std::endl;
	//testCode << "mov [a],[a]" << std::endl;
	testCode << "mov a,&var" << std::endl;
	//testCode << "mov a,&lobo" << std::endl;
	testCode << "var .db 0x99" << std::endl;
	testCode << ".db 0x69" << std::endl;

	try
	{
		Assembler a( testCode );
		a.Assemble( "out.txt" );
	}
	catch( const std::exception& e )
	{
		std::cout << "Fatal error: " << e.what() << std::endl;
	}
}

void test2()
{
	try
	{
		Assembler a( "mul.asm" );
		a.Assemble( "mul.txt" );
	}
	catch( const std::exception& e )
	{
		std::cout << "Fatal error: " << e.what() << std::endl;
	}
}

void test_classifier()
{
	std::cout << "These should be true" << std::endl;
	std::cout << std::boolalpha << (ParamType::Name == classify_param( "riger" )) << std::endl;
	std::cout << std::boolalpha << (ParamType::LiteralInteger == classify_param( "0x34" )) << std::endl;
	std::cout << std::boolalpha << (ParamType::Register == classify_param( "a" )) << std::endl;
	std::cout << std::boolalpha << (ParamType::RegisterIndirect == classify_param( "[b]" )) << std::endl;
	std::cout << std::boolalpha << (ParamType::NameAddress == classify_param( "&poo" )) << std::endl;
	std::cout << std::boolalpha << (ParamType::Unknown == classify_param( "69_doggy" )) << std::endl;
	

	std::cout << std::endl << "These should be false" << std::endl;
	std::cout << std::boolalpha << (ParamType::Name == classify_param( "a" )) << std::endl;
	std::cout << std::boolalpha << (ParamType::LiteralInteger == classify_param( "[b]" )) << std::endl;
	std::cout << std::boolalpha << (ParamType::Register == classify_param( "dig" )) << std::endl;
	std::cout << std::boolalpha << (ParamType::RegisterIndirect == classify_param( "0b01" )) << std::endl;
	std::cout << std::boolalpha << (ParamType::NameAddress == classify_param( "&a" )) << std::endl;
	std::cout << std::boolalpha << (ParamType::Unknown == classify_param( "&sigger9" )) << std::endl;
}

int run_tests()
{
	test();
	test2();
	test_classifier();

	while( !_kbhit() );
	return 0;
}