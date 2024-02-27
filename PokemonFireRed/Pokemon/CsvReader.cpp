#include "CsvReader.h"
#include "PokemonString.h"

UCsvReader::UCsvReader(std::string_view _FullPath)
{
	FileStream = std::ifstream(_FullPath.data());
}

UCsvReader::~UCsvReader()
{
	FileStream.close();
}

std::vector<std::string> UCsvReader::ReadLine()
{
	std::vector<std::string> Line;

	if (true == FileStream.eof())
	{
		return Line;
	}

	std::string Buffer;
	std::getline(FileStream, Buffer, '\n');

	return ComplexSplit(Buffer);
}

std::vector<std::vector<std::string>> UCsvReader::ReadLines()
{
	ReadLine();

	std::vector<std::vector<std::string>> Lines;

	while (true)
	{
		std::vector<std::string>&& Line = ReadLine();

		if (0 == Line.size())
		{
			break;
		}

		Lines.push_back(Line);
	}

	return Lines;
}

std::vector<std::string> UCsvReader::ComplexSplit(std::string_view _Line)
{
	std::vector<std::string> Result;

	// "I |want a "|"Events""" 라는 문자열을 생각해보자.
	// 첫 번째 | 위치에 있다면 DoubleQuoteCount = 1
	// 두 번째 | 위치에 있다면 DoubleQuoteCount = 2
	int DoubleQuoteCount = 0;
	bool Ascending = false;

	std::string Buffer = "";

	for (char Ch : _Line)
	{
		if (DoubleQuoteCount == 0 && Ch == '\"')
		{
			// "로 시작하는 셀을 읽기 시작한다. 
			// 즉 " 또는 ,가 포함된 셀을 읽기 시작한다. 
			DoubleQuoteCount++;
			Ascending = true;
		}
		else if (DoubleQuoteCount == 0 && Ch == ',')
		{
			// " 또는 ,가 포함되지 않은 셀을 다 읽었다.
			// 고민할 것 없이 Result에 Buffer를 추가한다.
			Result.push_back(Buffer);
		}
		else if (DoubleQuoteCount == 0)
		{
			// " 또는 ,가 포함되지 않은 셀을 읽는 중이다.
			// 고민할 것 없이 Buffer에 Ch를 더한다.
			Buffer += Ch;
		}
		else if (DoubleQuoteCount == 1 && Ch == '\"')
		{
			// " 또는 ,가 포함된 셀을 읽는 도중에 "를 발견했다. 직전에 "를 읽은 상태도 아니다.
			// Buffer에 "를 더한다.
			Buffer += Ch;
			DoubleQuoteCount++;
		}
		else if (DoubleQuoteCount == 2 && Ch == '\"')
		{
			// " 또는 ,가 포함된 셀을 읽는 도중에 "를 발견했다. 직전에 "를 읽은 상태다.
			DoubleQuoteCount++;
		}
		else if (DoubleQuoteCount == 3 && Ch == '\"')
		{
			Buffer += Ch;
			DoubleQuoteCount--;
		}

		else
		{
			// " 또는 ,가 포함된 셀을 읽는 도중이다. 
			// ,를 발견하더라도 똑같이 버퍼에 추가한다.
			Buffer += Ch;
			PrevCh = Ch;
		}
	}

	if (Buffer.size() > 0)
	{
		Result.push_back(Buffer);
	}

	return Result;
}

