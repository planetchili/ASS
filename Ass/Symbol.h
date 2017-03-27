#pragma once

#include <string>

class Symbol
{
public:
	enum class Type
	{
		Label,
		Variable
	};
public:
	Symbol( Symbol&& donor )
		:
		name( std::move( donor.name ) ),
		type( donor.type ),
		address( donor.address ),
		line( donor.line )
	{}
	int GetAddress() const { return address; }
	const std::string& GetName() const { return name; }
	Type GetType() const
	{
		return type;
	}
	int GetLine() const
	{
		return line;
	}
	static Symbol MakeVariable( const std::string& name,int line,int address )
	{
		return Symbol( name,line,address,Type::Variable );
	}
	static Symbol MakeLabel( const std::string& name,int line,int address )
	{
		return Symbol( name,line,address,Type::Label );
	}
	bool operator<( const Symbol& other ) const
	{
		return name < other.name;
	}
	bool operator==( const Symbol& other ) const
	{
		return type == other.type && name == other.name;
	}
private:
	Symbol( const std::string& name,int line,int address,Type type )
		:
		address( address ),
		name( name ),
		type( type ),
		line( line )
	{}
private:
	Type type;
	int address;
	int line;
	std::string name;
};
