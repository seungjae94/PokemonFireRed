#pragma once

#include "EnginePath.h"

class UEngineFile;

class UEngineDirectory : public UEnginePath
{
public:
	// constructor destructor
	UEngineDirectory();
	~UEngineDirectory();
	
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

protected:
	
private:
	// ��������� ��� ���� ��� ������ ã�� �Լ� 
	void AllFileRecursive(const std::string_view _Path, std::list<UEngineFile>& _Result, std::vector<std::string> _Ext = std::vector<std::string>(), bool _Recursive = false);
};

