#include "PokemonUtil.h"
#include <sstream>

UPokemonUtil::UPokemonUtil()
{
}

UPokemonUtil::~UPokemonUtil()
{
}

std::vector<std::string> UPokemonUtil::StringSplit(const std::string& _Str, char _Delimeter)
{
	std::vector<std::string> Result;
	std::stringstream Stream(_Str);
	std::string Buffer;

	while (std::getline(Stream, Buffer, _Delimeter))
	{
		Result.push_back(Buffer);
	}

	return Result;
}

std::vector<std::wstring> UPokemonUtil::StringSplit(const std::wstring& _Str, wchar_t _Delimeter)
{
	std::vector<std::wstring> Result;
	std::wstringstream Stream(_Str);
	std::wstring Buffer;

	while (std::getline(Stream, Buffer, _Delimeter))
	{
		Result.push_back(Buffer);
	}

	return Result;
}
