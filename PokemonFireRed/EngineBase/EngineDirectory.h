#pragma once
#include "EnginePath.h"
#include <list>

class UEngineFile;

class UEngineDirectory : public UEnginePath
{
public:
	UEngineDirectory();
	UEngineDirectory(const UEnginePath& _Path);
	~UEngineDirectory();

	void MoveToSearchChild(std::string_view _Path);

	/// <summary>
	/// ��� ���� ��� ������ ã�Ƽ� ��ȯ.
	/// </summary>
	/// <param name="_Ext">
	/// Ž���� ����� �� Ȯ����
	/// </param>
	/// <param name="_Recursive">
	/// true�� ������ ��� ��� ���� ���͸��� ������ �ش� ���͸� �ȿ� ���� ������ ã�´�.
	/// </param>
	std::list<UEngineFile> AllFile(std::vector<std::string> _Ext = std::vector<std::string>(), bool _Recursive = false);

	std::list<UEngineDirectory> AllDirectory(bool _Recursive = false);
protected:

private:
	// ��������� ��� ���� ��� ���͸��� ã�� �Լ�
	void AllDirectoryRecursive(const std::string_view _Path, std::list<UEngineDirectory>& _Result, bool _Recursive = false);

	// ��������� ��� ���� ��� ������ ã�� �Լ� 
	void AllFileRecursive(const std::string_view _Path, std::list<UEngineFile>& _Result, std::vector<std::string> _Ext = std::vector<std::string>(), bool _Recursive = false);
};

