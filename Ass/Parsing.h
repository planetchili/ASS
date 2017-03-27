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
		const auto i = std::find_if( s.begin(),s.end(),[]( char c ) { return std::isspace( c ) || c == ','; } );
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

inline bool try_consume_comma( std::string& s )
{
	const auto i = std::find_if( s.begin(),s.end(),[]( char c ) { return !std::isspace( c ); } );
	if( i != s.end() )
	{
		if( *i == ',' )
		{
			s.erase( s.begin(),std::next( i ) );
			return true;
		}
	}
	return false;
}

inline bool is_register_name( const std::string& s )
{
	return s == "a" || s == "b";
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

inline int parse_int_literal( const std::string& s,IntLiteralType type )
{
	switch( type )
	{
	case IntLiteralType::Dec:
		return std::stoi( s );
		break;
	case IntLiteralType::Bin:
		return std::stoi( s.substr( 2,std::string::npos ),0,2 );
		break;
	case IntLiteralType::Hex:
		return std::stoi( s,0,16 );
		break;
	default:
		assert( "bad int literal parse type!" && false );
		return -1;
	}
}

inline int parse_int_literal( const std::string& s )
{
	return parse_int_literal( s,int_literal_type( s ) );
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

inline bool is_indirect( const std::string& s )
{
	return s.size() > 1 &&
		s.front() == '[' && s.back() == ']';
}

inline bool try_strip_indirect( std::string& s )
{
	if( is_indirect( s ) )
	{
		s = std::string( std::next( s.begin() ),std::prev( s.end() ) );
		return true;
	}
	return false;
}

inline bool is_address( const std::string& s )
{
	return s.size() > 1 &&
		s.front() == '&' &&
		is_name( s.substr( 1,std::string::npos ) );
}

inline bool try_strip_address( std::string& s )
{
	if( is_address( s ) )
	{
		s = std::string( std::next( s.begin() ),s.end() );
		return true;
	}
	return false;
}

inline bool is_directive( const std::string& s )
{
	return s.size() > 1 &&
		s.front() == '.' &&
		std::all_of( s.begin() + 1,s.end(),
			[]( char c ) { return std::isalnum( c ); }
	);
}