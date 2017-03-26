#include <iostream>
#include "Trim.h"
#include <thread>
#include <map>
#include <any>
#include <optional>
#include <sstream>
#include <exception>

std::map<std::string,std::any> symbols;

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

void process_line( std::string& line )
{
	strip_comment( line );


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

	using namespace std::chrono_literals;
	std::this_thread::sleep_for( 1500ms );
	return 0;
}