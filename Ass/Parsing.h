#pragma once

#include <string>
#include <optional>
#include <algorithm>
#include "Trim.h"


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
		std::all_of( s.begin(),std::prev( s.end() ),[]( char c )
		{
			return std::isalnum( c ) || c == '_';
		} ) &&
		std::any_of( s.begin(),std::prev( s.end() ),[]( char c )
		{
			return std::isalnum( c );
		} );
}

bool is_name( const std::string& s )
{
	return s.size() > 0 &&
		!std::isdigit( s.front() ) &&
		std::all_of( s.begin(),s.end(),[]( char c )
		{
			return std::isalnum( c ) || c == '_';
		} ) &&
		std::any_of( s.begin(),std::prev( s.end() ),[]( char c )
		{
			return std::isalnum( c );
		} );
}

bool is_directive( const std::string& s )
{
	return s.size() > 1 &&
		s.front() == '.' &&
		std::all_of( s.begin() + 1,s.end(),
			[]( char c ) { return std::isalnum( c ); }
	);
}