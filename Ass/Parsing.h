#pragma once

#include <string>
#include <optional>
#include <algorithm>
#include "Trim.h"


inline void strip_comment( std::string& line )
{
	const auto pos = line.find_first_of( ';' );
	if( pos != std::string::npos )
	{
		line.erase( pos );
	}
}

inline std::optional<std::string> extract_token_white( std::string& s )
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

enum class IntLiteralType
{
	Not,
	Dec,
	Bin,
	Hex
};

inline IntLiteralType int_literal_type( const std::string& s )
{
	if( s.size() == 0 )
	{
		return IntLiteralType::Not;
	}

	if( !std::isdigit( s.front() ) )
	{
		return IntLiteralType::Not;
	}

	if( s.size() >= 3 )
	{
		if( s[1] == 'x' )
		{
			return std::all_of( std::next( s.begin(),2 ),s.end(),[]( char c ) {
				return std::isdigit( c ) || 
					(std::tolower( c ) >= 'a' && std::tolower( c ) <= 'f');
			} ) ? IntLiteralType::Hex : IntLiteralType::Not;
		}
		else if( s[1] == 'b' )
		{
			return std::all_of( std::next( s.begin(),2 ),s.end(),[]( char c ) {
				return c == '0' || c == '1';
			} ) ? IntLiteralType::Bin : IntLiteralType::Not;
		}
	}

	return std::all_of( std::next( s.begin() ),s.end(),[]( char c ) {
		return std::isdigit( c );
	} ) ? IntLiteralType::Dec : IntLiteralType::Not;
}

inline bool is_label( const std::string& s )
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

inline bool is_name( const std::string& s )
{
	return s.size() > 0 &&
		!std::isdigit( s.front() ) &&
		std::all_of( s.begin(),s.end(),[]( char c )
		{
			return std::isalnum( c ) || c == '_';
		} ) &&
		std::any_of( s.begin(),s.end(),[]( char c )
		{
			return std::isalnum( c );
		} );
}

inline bool is_directive( const std::string& s )
{
	return s.size() > 1 &&
		s.front() == '.' &&
		std::all_of( s.begin() + 1,s.end(),
			[]( char c ) { return std::isalnum( c ); }
	);
}