#include "EnginePath.h"

#include "EngineDebug.h"

UEnginePath::UEnginePath()
	: Path(std::filesystem::current_path())
{
}

UEnginePath::UEnginePath(std::filesystem::path _Path)
	: Path(_Path)
{
}

UEnginePath::~UEnginePath() 
{
}

bool UEnginePath::IsFile()
{
	return !std::filesystem::is_directory(Path);
}

bool UEnginePath::IsDirectory()
{
	return std::filesystem::is_directory(Path);
}

bool UEnginePath::IsExists()
{
	return std::filesystem::exists(Path);
}

std::string UEnginePath::GetFullPath()
{
	return Path.string();
}

std::string UEnginePath::GetExtension()
{
	std::filesystem::path Ext = Path.extension();
	return Ext.string();
}

std::string UEnginePath::GetFileName()
{
	std::filesystem::path Name = Path.filename();
	return Name.string();
}

void UEnginePath::MoveParent()
{
	Path = Path.parent_path();
}

void UEnginePath::Move(std::string_view _Path)
{
	std::filesystem::path NextPath = Path;
	NextPath.append(_Path);					// '/'까지 붙혀서 이동한 경로를 만들어 준다.

	bool Check = std::filesystem::exists(NextPath);
	if (false == Check)
	{
		MsgBoxAssert(NextPath.string() + "라는 경로는 존재하지 않습니다");
		return;
	}

	Path = NextPath;
}