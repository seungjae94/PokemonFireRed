#include "PokemonString.h"

UPokemonString::UPokemonString()
{
}

UPokemonString::~UPokemonString()
{
}

std::vector<std::string> UPokemonString::Split(const std::string& _Str, char _Delimeter)
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

std::vector<std::wstring> UPokemonString::Split(std::wstring_view _Str, wchar_t _Delimeter)
{
	std::vector<std::wstring> Result;
	std::wstringstream Stream(_Str.data());
	std::wstring Buffer;

	while (std::getline(Stream, Buffer, _Delimeter))
	{
		Result.push_back(Buffer);
	}

	return Result;
}