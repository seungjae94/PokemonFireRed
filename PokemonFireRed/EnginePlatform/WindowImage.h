#pragma once

#include <Windows.h>
#include <EngineBase/EngineMath.h>
#include <EngineBase/Transform.h>
#include <EngineBase/PathObject.h>
#include <string>

enum class EWIndowImageType
{
	IMG_NONE,
	IMG_BMP,
	IMG_PNG
};

class UEngineWindow;

// 이미지 정보를 저장하는 객체
// - Name, Path를 가지기 때문에 UPathObject 상속
class UWindowImage : public UPathObject 
{
	friend UEngineWindow; // ImageDC
public:
	// constructor destructor
	UWindowImage();
	~UWindowImage();

	// delete Function
	UWindowImage(const UWindowImage& _Other) = delete;
	UWindowImage(UWindowImage&& _Other) noexcept = delete;
	UWindowImage& operator=(const UWindowImage& _Other) = delete;
	UWindowImage& operator=(UWindowImage&& _Other) noexcept = delete;

	/// <summary>
	/// Path에 위치한 실제 이미지를 hBitMap 필드에 로드한다. 또 hBitMap 필드를 사용하는 ImageDC를 생성한다.
	/// </summary>
	/// <param name="_Image">
	/// </param>
	/// <returns>
	/// 이미지 로드 성공 여부
	/// </returns>
	bool Load(UWindowImage* _Image);

	// 
	bool Create(UWindowImage* _Image, const FVector& _Scale);

	void BitCopy(UWindowImage* _CopyImage, const FTransform& _Trans);

	void TransCopy(UWindowImage* _CopyImage, const FTransform& _Trans, const FTransform& _ImageTrans, Color8Bit _Color);

protected:

private:
	HBITMAP hBitMap = nullptr;
	HDC ImageDC = nullptr;
	BITMAP BitMapInfo = BITMAP();

	EWIndowImageType ImageType = EWIndowImageType::IMG_NONE;

	bool Create(HDC _MainDC);
};

