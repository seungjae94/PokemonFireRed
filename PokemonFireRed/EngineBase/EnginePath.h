#pragma once

#include <string>
#include <filesystem>

// ����� ����, �̵��� ����ϴ� Ŭ����
class UEnginePath
{
public:
	// constructor destructor
	UEnginePath(); // ���� ��θ� �Է����� ������ ���� ��η� �����Ѵ�.
	UEnginePath(std::filesystem::path _Path);
	~UEnginePath();

	// delete Function
	UEnginePath(const UEnginePath& _Other) = delete;
	UEnginePath(UEnginePath&& _Other) noexcept = delete;
	UEnginePath& operator=(const UEnginePath& _Other) = delete;
	UEnginePath& operator=(UEnginePath&& _Other) noexcept = delete;

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

