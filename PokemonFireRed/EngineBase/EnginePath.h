#pragma once
#include <string>
#include <filesystem>

// 경로의 편집, 이동을 담당하는 클래스
class UEnginePath
{
public:
	// constructor destructor
	UEnginePath(); // 따로 경로를 입력하지 않으면 실행 경로로 세팅한다.
	UEnginePath(std::filesystem::path _Path);
	~UEnginePath();

	bool IsFile();
	bool IsDirectory();
	bool IsExists();

	std::string GetFullPath();
	std::string GetFileName();
	std::string GetExtension();

	void MoveParent();
	void Move(std::string_view _Path);

protected:
	std::filesystem::path Path;
private:

};

