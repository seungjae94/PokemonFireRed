#include "EngineDirectory.h"

#include "EngineString.h"
#include "EngineFile.h"

UEngineDirectory::UEngineDirectory() 
{
}

UEngineDirectory::~UEngineDirectory() 
{
}

std::list<UEngineFile> UEngineDirectory::AllFile(std::vector<std::string> _Ext, bool _Recursive)
{
	std::list<UEngineFile> Result;

	for (size_t i = 0; i < _Ext.size(); i++)
	{
		_Ext[i] = UEngineString::ToUpper(_Ext[i]);
	}

	AllFileRecursive(Path.string(), Result, _Ext, _Recursive);

	return Result;
}

void UEngineDirectory::AllFileRecursive(const std::string_view _Path, std::list<UEngineFile>& _Result, std::vector<std::string> _Ext, bool _Recursive)
{
	std::filesystem::directory_iterator DirIter = std::filesystem::directory_iterator(_Path);

	for (const std::filesystem::directory_entry& Entry : DirIter)
	{
		std::filesystem::path Path = Entry.path();
		std::filesystem::path Ext = Entry.path().extension();
		std::string UpperExt = UEngineString::ToUpper(Ext.string());

		// ���͸��� ��� ��� Ž��
		if (Entry.is_directory())
		{
			if (_Recursive)
			{
				AllFileRecursive(Path.string(), _Result, _Ext, _Recursive);
			}
			continue;
		}

		// Ȯ���ڸ� �������� ���� ��� ��� ������ ã�´�.
		if (_Ext.size() == 0)
		{
			_Result.push_back(UEngineFile(Path.string()));
			continue;
		}

		// UpperExt�� _Ext�� ���ԵǾ� �ִ��� üũ�Ѵ�.
		for (size_t i = 0; i < _Ext.size(); i++)
		{
			if (UpperExt == _Ext[i])
			{
				_Result.push_back(UEngineFile(Path.string()));
				break;
			}
		}
	}
}

