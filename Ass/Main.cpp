#include <iostream>
#include "Trim.h"
#include <map>
#include <any>
#include <optional>
#include <sstream>
#include <exception>
#include <locale>
#include <vector>
#include <conio.h>

class Instruction {};

class Directive {};

std::map<std::string,std::any> symbols;

std::map<std::string,Instruction> instructions;

std::map<std::string,Directive> directives;

class Label {};
class Variable {};

void strip_comment( std::string& line )
{
	const auto pos = line.find_first_of( ';' );
	if( pos != std::string::npos )
	{
		line.erase( pos );
	}
}

std::optional<std::string> extract_token_white( std::string& s )
{
	ltrim( s );
	if( s.size() > 0 )
	{
		const auto i = std::find_if( s.begin(),s.end(),[]( char c ) { return c == ' ' || c == '\t'; } );
		if( i != s.end() )
		{
			auto r = std::string( s.begin(),i );
			s.erase( s.begin(),i );
			return r;
		}
		else
		{
			return std::move( s );
		}
	}
	return {};
}

bool is_label( const std::string& s )
{
	return s.size() > 1 && 
		s.back() == ':' && 
		!std::isdigit( s.front() ) &&
		std::all_of( s.begin(),std::prev( s.end() ),[]( char c ) { 
		return std::isalnum( c ) || c == '_'; }
	);
}

bool is_name( const std::string& s )
{
	return s.size() > 0 &&
		!std::isdigit( s.front() ) &&
		std::all_of( s.begin(),s.end(),[]( char c ) {
		return std::isalnum( c ) || c == '_'; }
	);
}

bool is_directive( const std::string& s )
{
	return s.size() > 1 &&
		s.front() == '.' &&
		std::all_of( s.begin() + 1,s.end(),
			[]( char c ) { return std::isalnum( c ); }
	);
}

void process_line( std::string& line,int lineNum )
{
	strip_comment( line );

	{
		auto t = extract_token_white( line );

		if( !t.has_value() )
		{
			return;
		}

		if( is_label( t.value() ) )
		{
			// remove the colon
			t.value().pop_back();
			if( symbols.count( t.value() ) > 0 )
			{
				std::stringstream msg;
				msg << "Cannot create label at line <" << lineNum << ">! Symbol [" << t.value() << "] already exists!";
				throw std::exception( msg.str().c_str() );
			}

			if( extract_token_white( line ).has_value() )
			{
				std::stringstream msg;
				msg << "Cannot create label at line <" << lineNum << ">! There is other garbage on that line!";
				throw std::exception( msg.str().c_str() );
			}
			// add label
			symbols.emplace( t.value(),Label() );
		}
		else if( is_name( t.value() ) )
		{
			if( symbols.count( t.value() ) > 0 )
			{
				std::stringstream msg;
				msg << "Found a symbol at line <" << lineNum << ">! Symbol [" << t.value() << "] already exists and I hate you!";
				throw std::exception( msg.str().c_str() );
			}
			else if( instructions.count( t.value() ) > 0 )
			{
				// process instruction
			}
			else // not instruction; must be a directive!
			{
				auto d = extract_token_white( line );
				if( !d.has_value() )
				{
					std::stringstream msg;
					msg << "Found a symbol at line <" << lineNum << ">! Symbol [" << t.value() << "]. Expected directive afterwards. Found nothing. Is sad now.";
					throw std::exception( msg.str().c_str() );
				}
				else if( !is_directive( d.value() ) )
				{
					std::stringstream msg;
					msg << "Found a symbol at line <" << lineNum << ">! Symbol [" << t.value() << "]. Expected directive afterwards. Found ["
						<< d.value() << "]. Wtf bro?";
					throw std::exception( msg.str().c_str() );
				}
				else
				{
					d.value().erase( d.value().begin(),d.value().begin() + 1 );
					if( directives.count( d.value() ) == 0 )
					{
						std::stringstream msg;
						msg << "Unknown directive at line <" << lineNum << ">! After new symbol [" << t.value() << "], found [."
							<< d.value() << "]. Do you even code bro?";
						throw std::exception( msg.str().c_str() );
					}

					// process directive
				}
			}
		}
	}
}

int main( int argc,char* argv[] )
{
	std::string line = "ahfsdhf ";
	strip_comment( line );
	std::cout << line << std::endl;

	std::string tok = "   fuk: dnsi ..   \t\t moo \t";

	for( std::optional<std::string> os = extract_token_white( tok );
		os.has_value(); os = extract_token_white( tok ) )
	{
		std::cout << os.value() << std::endl;
	}

	std::vector<std::string> test = {
		"   fidn:  ",
		"thend: dith",
		"9dnti:",
		"jappa9__: ;shitty fuck fuck"
		"poop ax,9",
		"    fidn:    ; tooy tooul &&&&___9",
		"  : ",
		"fidn",
		"shippy  ",
		"derp bik",
		"derp .dicks"
	};

	int count = 0;
	for( auto l : test )
	{
		try
		{
			process_line( l,count );
		}
		catch( const std::exception& e )
		{
			std::cout << e.what() << std::endl;
		}
		count++;
	}

	std::cout << "SYMBOLS!" << std::endl;
	for( const auto& s : symbols )
	{
		std::cout << s.first << std::endl;
	}
	
	while( !_kbhit() );
	return 0;
}