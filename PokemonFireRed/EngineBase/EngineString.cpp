#include "EngineString.h"

std::string UEngineString::ToUpper(std::string_view _View)
{
	std::string Str = _View.data();

	for (char& _Ch : Str)
	{
		_Ch = std::toupper(_Ch);
	}

	return Str;
}

UEngineString::UEngineString()
{
}

UEngineString::~UEngineString()
{
}
