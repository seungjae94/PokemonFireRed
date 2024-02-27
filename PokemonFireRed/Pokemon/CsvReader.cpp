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

	// "I |want a "|"Events""" ��� ���ڿ��� �����غ���.
	// ù ��° | ��ġ�� �ִٸ� DoubleQuoteCount = 1
	// �� ��° | ��ġ�� �ִٸ� DoubleQuoteCount = 2
	int DoubleQuoteCount = 0;
	bool Ascending = false;

	std::string Buffer = "";

	for (char Ch : _Line)
	{
		if (DoubleQuoteCount == 0 && Ch == '\"')
		{
			// "�� �����ϴ� ���� �б� �����Ѵ�. 
			// �� " �Ǵ� ,�� ���Ե� ���� �б� �����Ѵ�. 
			DoubleQuoteCount++;
			Ascending = true;
		}
		else if (DoubleQuoteCount == 0 && Ch == ',')
		{
			// " �Ǵ� ,�� ���Ե��� ���� ���� �� �о���.
			// ����� �� ���� Result�� Buffer�� �߰��Ѵ�.
			Result.push_back(Buffer);
		}
		else if (DoubleQuoteCount == 0)
		{
			// " �Ǵ� ,�� ���Ե��� ���� ���� �д� ���̴�.
			// ����� �� ���� Buffer�� Ch�� ���Ѵ�.
			Buffer += Ch;
		}
		else if (DoubleQuoteCount == 1 && Ch == '\"')
		{
			// " �Ǵ� ,�� ���Ե� ���� �д� ���߿� "�� �߰��ߴ�. ������ "�� ���� ���µ� �ƴϴ�.
			// Buffer�� "�� ���Ѵ�.
			Buffer += Ch;
			DoubleQuoteCount++;
		}
		else if (DoubleQuoteCount == 2 && Ch == '\"')
		{
			// " �Ǵ� ,�� ���Ե� ���� �д� ���߿� "�� �߰��ߴ�. ������ "�� ���� ���´�.
			DoubleQuoteCount++;
		}
		else if (DoubleQuoteCount == 3 && Ch == '\"')
		{
			Buffer += Ch;
			DoubleQuoteCount--;
		}

		else
		{
			// " �Ǵ� ,�� ���Ե� ���� �д� �����̴�. 
			// ,�� �߰��ϴ��� �Ȱ��� ���ۿ� �߰��Ѵ�.
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

