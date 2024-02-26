#pragma once
#include <EngineBase/EngineString.h>
#include <cwctype>
#include <string>

class UPokemonString
{
public:
	// constructor destructor
	UPokemonString();
	~UPokemonString();

	// delete Function
	UPokemonString(const UPokemonString& _Other) = delete;
	UPokemonString(UPokemonString&& _Other) noexcept = delete;
	UPokemonString& operator=(const UPokemonString& _Other) = delete;
	UPokemonString& operator=(UPokemonString&& _Other) noexcept = delete;

	static std::string PadLeft(std::string_view _Text, int _Width, char _PadChar)
	{
		std::string Result = _Text.data();
		Result.insert(0, _Width - _Text.size(), _PadChar);
		return Result;
	}

	static std::wstring PadLeft(std::wstring_view _Text, int _Width, wchar_t _PadChar)
	{
		std::wstring Result = _Text.data();
		Result.insert(0, _Width - _Text.size(), _PadChar);
		return Result;
	}

	static std::wstring ToUpperW(std::wstring_view _View)
	{
		std::wstring Text = _View.data();

		for (wchar_t& _Ch : Text)
		{
			_Ch = std::towupper(_Ch);
		}

		return Text;
	}

	static std::wstring ToUpperW(std::string_view _Text)
	{
		return ToUpperW(UEngineString::AnsiToUniCode(_Text));
	}

protected:

private:
};

