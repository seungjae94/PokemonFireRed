#include "EngineDirectory.h"
#include "EngineFile.h"
#include "EngineString.h"

UEngineDirectory::UEngineDirectory()
{
}

UEngineDirectory::UEngineDirectory(const UEnginePath& _Path)
	: UEnginePath(_Path.GetFullPath())
{

}

UEngineDirectory::~UEngineDirectory()
{
}

void UEngineDirectory::AllFileRecursive(
	const std::string_view _Path,
	std::list<UEngineFile>& _Result,
	std::vector<std::string> _Ext,
	bool _Recursive)
{
	std::filesystem::directory_iterator DirIter = std::filesystem::directory_iterator(_Path);
	for (const std::filesystem::directory_entry& Entry : DirIter)
	{
		std::filesystem::path Path = Entry.path();
		std::filesystem::path Ext = Entry.path().extension();
		std::string UpperExt = UEngineString::ToUpper(Ext.string());

		// 디렉터리인 경우 재귀 탐색
		if (true == Entry.is_directory())
		{
			if (true == _Recursive)
			{
				AllFileRecursive(Path.string(), _Result, _Ext, _Recursive);
			}
			continue;
		}

		// 확장자를 제공하지 않은 경우 모든 파일을 찾는다.
		if (0 == _Ext.size())
		{
			_Result.push_back(UEngineFile(Path.string()));
			continue;
		}

		// UpperExt가 _Ext에 포함되어 있는지 체크한다.
		bool Check = false;
		for (size_t i = 0; i < _Ext.size(); i++)
		{
			if (_Ext[i] == UpperExt)
			{
				Check = true;
			}
		}

		if (true == Check)
		{
			_Result.push_back(UEngineFile(Path.string()));
		}

	}
}

std::list<UEngineFile> UEngineDirectory::AllFile(
	std::vector<std::string> _Ext ,
	bool _Rescursive
)
{
	std::list<UEngineFile> Result;

	for (size_t i = 0; i < _Ext.size(); i++)
	{
		_Ext[i] = UEngineString::ToUpper(_Ext[i]);
	}

	AllFileRecursive(Path.string(), Result, _Ext, _Rescursive);
	return Result;
}