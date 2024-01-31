#pragma once

#include "EnginePath.h"

class UEngineFile;

class UEngineDirectory : public UEnginePath
{
public:
	// constructor destructor
	UEngineDirectory();
	~UEngineDirectory();

	// delete Function
	UEngineDirectory(const UEngineDirectory& _Other) = delete;
	UEngineDirectory(UEngineDirectory&& _Other) noexcept = delete;
	UEngineDirectory& operator=(const UEngineDirectory& _Other) = delete;
	UEngineDirectory& operator=(UEngineDirectory&& _Other) noexcept = delete;
	
	/// <summary>
	/// 경로 내의 모든 파일을 찾아서 반환.
	/// </summary>
	/// <param name="_Ext">
	/// 탐색의 대상이 될 확장자
	/// </param>
	/// <param name="_Recursive">
	/// true로 설정할 경우 경로 내에 디렉터리가 있으면 해당 디렉터리 안에 들어가서 파일을 찾는다.
	/// </param>
	std::list<UEngineFile> AllFile(std::vector<std::string> _Ext = std::vector<std::string>(), bool _Recursive = false);

protected:
	
private:
	// 
	void AllFileRecursive(const std::string_view _Path, std::list<UEngineFile>& _Result, std::vector<std::string> _Ext = std::vector<std::string>(), bool _Recursive = false);
};

