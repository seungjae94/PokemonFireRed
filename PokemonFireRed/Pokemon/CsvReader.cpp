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
	return UPokemonString::Split(Buffer, ',');
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

