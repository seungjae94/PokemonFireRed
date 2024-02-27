#include "CsvReader.h"
#include <EngineBase/EngineDebug.h>
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
	//return UPokemonString::Split(Buffer, ',');
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

	// 35(0), "(1)Hello, ""World(2)"" is the common message.", 0.75
	// (0) ��ġ�� Degree = 0
	// (1) ��ġ�� Degree = 1
	// (2) ��ġ�� Degree = 2
	int Degree = 0;

	std::string Buffer = "";

	int i = 0;

	while (i < _Line.size())
	{
		char Ch = _Line[i];
		char NextCh = 0;
		if (i + 1 < _Line.size())
		{
			NextCh = _Line[i + 1];
		}

		if (Degree == 0)
		{
			switch (Ch)
			{
			case '\"':
				// Escaped ���� ���� "
				++Degree;
				++i;
				break;
			case ',':
				// �� ����
				Buffer = ConvertLinespace(Buffer);
				Result.push_back(Buffer);
				Buffer = "";
				++i;
				break;
			default:
				Buffer += Ch;
				++i;
				break;
			}
		}
		else if (Degree == 1)
		{
			switch (Ch)
			{
			case '\"':
				if (NextCh == '\"')
				{
					// �ο� ����
					Buffer += Ch;
					++Degree;
					i += 2;
				}
				else
				{
					// Escaped ���� �ݴ� "
					--Degree;
					++i;
				}
				break;
			case ',':
			default:
				Buffer += Ch;
				++i;
				break;
			}
		}
		else if (Degree == 2)
		{
			switch (Ch)
			{
			case '\"':
				if (NextCh == '\"')
				{
					// �ο� Ż��
					Buffer += Ch;
					--Degree;
					i += 2;
				}
				else
				{
					MsgBoxAssert("�̷л� �Ұ����� ���Դϴ�. ���� ������ ������ �ֽ��ϴ�.");
					++i;
				}
				break;
			case ',':
			default:
				Buffer += Ch;
				++i;
				break;
			}
		}
	}

	if (Buffer.size() > 0)
	{
		Result.push_back(Buffer);
	}

	return Result;
}

std::string UCsvReader::ConvertLinespace(std::string_view _Buffer)
{
	int i = 0;

	std::string Result;

	while (i < _Buffer.size())
	{
		char Ch = _Buffer[i];
		char NextCh = 0;
		if (i + 1 < _Buffer.size())
		{
			NextCh = _Buffer[i + 1];
		}

		if (Ch == '\\' && NextCh == 'n')
		{
			Result += '\n';
			i += 2;
		}
		else
		{
			Result += Ch;
			++i;
		}
	}

	return Result;
}
