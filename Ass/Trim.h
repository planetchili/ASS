#pragma once
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

// trim from start (in place)
static inline void ltrim( std::string &s )
{
	s.erase( s.begin(),std::find_if( s.begin(),s.end(),
		[]( char c ) { return !std::isspace( c ); } ) );

}

// trim from end (in place)
static inline void rtrim( std::string &s )
{
	s.erase( std::find_if( s.rbegin(),s.rend(),
		[]( char c ) { return !std::isspace( c ); } ).base(),s.end() );
}

// trim from both ends (in place)
static inline void trim( std::string &s )
{
	ltrim( s );
	rtrim( s );
}

// trim from start (copying)
static inline std::string ltrimmed( std::string s )
{
	ltrim( s );
	return s;
}

// trim from end (copying)
static inline std::string rtrimmed( std::string s )
{
	rtrim( s );
	return s;
}

// trim from both ends (copying)
static inline std::string trimmed( std::string s )
{
	trim( s );
	return s;
}