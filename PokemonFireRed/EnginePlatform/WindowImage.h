#pragma once

#include <EngineBase/PathObject.h>
#include <string>

// 이미지 정보를 저장하는 객체
// - Name, Path를 가지기 때문에 UPathObject 상속
class UWindowImage : public UPathObject 
{
public:
	// constructor destructor
	UWindowImage();
	~UWindowImage();

	// delete Function
	UWindowImage(const UWindowImage& _Other) = delete;
	UWindowImage(UWindowImage&& _Other) noexcept = delete;
	UWindowImage& operator=(const UWindowImage& _Other) = delete;
	UWindowImage& operator=(UWindowImage&& _Other) noexcept = delete;

protected:

private:

};

