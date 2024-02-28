#pragma once
#include <EngineBase\PathObject.h>
#include <EngineBase\EngineMath.h>
#include <EngineBase\Transform.h>
#include <EngineBase\EngineDebug.h>
#include <Windows.h>
#include <string>
#include <string_view>
#include <objidl.h>
#include <gdiplus.h>

enum class EImageLoadType
{
	IMG_Folder,
	IMG_Cutting,
};

enum class EWIndowImageType
{
	IMG_NONE,
	IMG_BMP,
	IMG_PNG
};

class UImageInfo
{
public:
	HBITMAP hBitMap;
	HDC ImageDC = nullptr;
	FTransform CuttingTrans;
	UImageInfo* RotationMaskImage = nullptr;
	EWIndowImageType ImageType = EWIndowImageType::IMG_NONE;
};

class UEngineWindow;

// 이미지 정보를 저장하는 객체
// - Name, Path를 가지기 때문에 UPathObject 상속
class UWindowImage : public UPathObject
{
	friend UEngineWindow;	// 윈도우 이미지, 백버퍼 이미지 생성

public:
	UWindowImage();
	~UWindowImage();		// 커널 오브젝트 릴리즈

	// delete Function
	UWindowImage(const UWindowImage& _Other) = delete;
	UWindowImage(UWindowImage&& _Other) noexcept = delete;
	UWindowImage& operator=(const UWindowImage& _Other) = delete;
	UWindowImage& operator=(UWindowImage&& _Other) noexcept = delete;

	/// <summary>
	/// Path에 위치한 리소스 이미지를 비트맵으로 로드한다. 
	/// 또 생성한 비트맵을 사용하는 DC를 만든다.
	/// </summary>
	/// <param name="_Image">
	/// 최종적으로 이미지를 그릴 대상 이미지.
	/// _Image 인자로는 윈도우 이미지가 사실상 강제된다.
	/// </param>
	/// <returns>
	/// 이미지 로드 성공 여부
	/// </returns>
	bool Load(UWindowImage* _Image);

	bool LoadFolder(UWindowImage* _Image);

	FVector GetScale();

	// 이미지 회전에 사용하는 함수
	void PlgCopy(UWindowImage* _CopyImage, const FTransform& _Trans, int _Index, float _RadAngle);

	/// <summary>
	/// 이미지 this에 이미지 _CopyImage를 그려주는 함수.
	/// 주 사용처: 윈도우 이미지에 백버퍼 이미지를 그릴 때 사용.
	/// </summary>
	/// <param name="_CopyImage">
	/// this에 복사할 이미지
	/// </param>
	/// <param name="_Trans">
	/// this(백버퍼)의 트랜스폼
	/// </param>
	void BitCopy(UWindowImage* _CopyImage, const FTransform& _Trans);

	/// <summary>
	/// 이미지 this에 이미지 _CopyImage를 그려주는 함수.
	/// 주 사용처: 백버퍼 이미지에 렌더러가 갖고 있는 이미지를 그릴 때 사용.
	/// </summary>
	/// <param name="_CopyImage">
	/// this에 그릴 이미지
	/// </param>
	/// <param name="_Trans">
	/// (이미지 렌더러의 절대 위치, 이미지 렌더러의 크기)를 나타내는 트랜스폼.
	/// </param>
	/// <param name="_ImageCuttingTrans">
	/// 이미지에서 잘라낼 영역을 의미하는 트랜스폼.
	/// </param>
	/// <param name="_Color">
	/// 투명으로 취급할 색상
	/// </param>
	void TransCopy(UWindowImage* _CopyImage, const FTransform& _Trans, int _Index, Color8Bit _Color = Color8Bit::Black);

	void AlphaCopy(UWindowImage* _CopyImage, const FTransform& _Trans, int _Index, Color8Bit _Color = Color8Bit::Black);

	void TextCopy(const std::string& _Text, const std::string& _Font, float _Size, const FTransform& _Trans, Color8Bit _Color);

	void TextCopy(const std::string& _Text, const std::string& _Font, float _Size, const FTransform& _Trans, Color8Bit _OutLineColor, Color8Bit _FillColor);

	void TextCopyBold(const std::string& _Text, const std::string& _Font, float _Size, const FTransform& _Trans, Color8Bit _Color);

	void TextCopyFormat(const std::string& _Text, const std::string& _Font, const Gdiplus::StringFormat& stringFormat, float _Size, const FTransform& _Trans, Color8Bit _Color);

	// 주 사용처: 백버퍼 이미지 생성
	// - 리소스 이미지는 Load로 생성한다.
	bool Create(UWindowImage* _Image, const FVector& _Scale);

	void Cutting(int _X, int _Y);

	void SetCuttingTransform(const FTransform& _CuttingTrans, int _Index = 0);

	void DrawRectangle(const FTransform& _Trans);
	void DrawEllipse(const FTransform& _Trans);

	Color8Bit GetColor(int _X, int _Y, Color8Bit _DefaultColor);

	EWIndowImageType GetImageType()
	{
		return ImageType;
	}

	void SetRotationMaskImage(int _Index, UWindowImage* _RotationMaskImage, int _MaskIndex)
	{
		UImageInfo& Ref = _RotationMaskImage->Infos[_MaskIndex];
		Infos[_Index].RotationMaskImage = &Ref;
	}

	void SetRotationMaskImageFolder(UWindowImage* _RotationMaskImage)
	{
		if (Infos.size() != _RotationMaskImage->Infos.size())
		{
			MsgBoxAssert("이미지정보의 크기가 다른 이미지 끼리 매칭을 할수가 없습니다.");
			return;
		}

		for (int i = 0; i < static_cast<int>(Infos.size()); i++)
		{
			SetRotationMaskImage(i, _RotationMaskImage, i);
		}
	}

	void TextPrint(std::string_view _Text, FVector _Pos);

	const UImageInfo& ImageInfo(int _Index)
	{
		return Infos[_Index];
	}

protected:

private:
	UWindowImage* RotationMaskImage = nullptr;

	EImageLoadType LoadType = EImageLoadType::IMG_Cutting;

	HBITMAP hBitMap = 0;
	HDC ImageDC = 0;
	BITMAP BitMapInfo = BITMAP(); 
	EWIndowImageType ImageType = EWIndowImageType::IMG_NONE;

	std::vector<UImageInfo> Infos;

	// 주 사용처: 윈도우 이미지 생성
	// - 리소스 이미지는 Load로 생성한다.
	bool Create(HDC _MainDC);
};

