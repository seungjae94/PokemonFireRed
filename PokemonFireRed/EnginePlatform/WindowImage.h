#pragma once

#include <EngineBase/PathObject.h>
#include <string>

// �̹��� ������ �����ϴ� ��ü
// - Name, Path�� ������ ������ UPathObject ���
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

