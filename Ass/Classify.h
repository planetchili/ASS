#pragma once

#include "Parsing.h"
#include "Trim.h"

enum class ParamType
{
	Register,
	RegisterIndirect,
	Name,
	NameAddress,
	LiteralInteger,
	Unknown
};

inline ParamType str_to_param_type( const std::string& pts )
{
	if( pts == "reg" )
	{
		return ParamType::Register;
	}
	else if( pts == "regind" )
	{
		return ParamType::RegisterIndirect;
	}
	else if( pts == "name" )
	{
		return ParamType::Name;
	}
	else if( pts == "nameadd" )
	{
		return ParamType::NameAddress;
	}
	else if( pts == "int" )
	{
		return ParamType::LiteralInteger;
	}
	else
	{
		return ParamType::Unknown;
	}
}

inline ParamType classify_param( const std::string& p )
{
	if( is_register_name( p ) )
	{
		return ParamType::Register;
	}

	if( is_name( p ) )
	{
		return ParamType::Name;
	}

	{
		const auto lit = int_literal_type( p );
		if( lit != IntLiteralType::Not )
		{
			return ParamType::LiteralInteger;
		}
	}

	// need mutable for the try_strip functions
	auto pc = p;
	{
		const bool isAddress = try_strip_address( pc );
		if( isAddress )
		{
			if( is_name( pc ) )
			{
				return ParamType::NameAddress;
			}
			else
			{
				return ParamType::Unknown;
			}
		}
	}

	{
		const bool isIndirect = try_strip_indirect( pc );
		if( isIndirect )
		{
			if( is_register_name( pc ) )
			{
				return ParamType::RegisterIndirect;
			}
			else
			{
				return ParamType::Unknown;
			}
		}
	}

	return ParamType::Unknown;
}