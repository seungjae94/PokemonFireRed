#include "WindowImage.h"

#include <EngineBase/EngineDebug.h>
#include <EngineBase/EngineString.h>

// GDI 관련 라이브러리
#pragma comment(lib, "Msimg32.lib")
#pragma comment(lib, "Gdiplus.lib")
#include <objidl.h>
#include <gdiplus.h>

UWindowImage::UWindowImage() 
{
}

UWindowImage::~UWindowImage() 
{
}

bool UWindowImage::Load(UWindowImage* _Image)
{
	UEnginePath Path = GetEnginePath();

	std::string UpperExt = UEngineString::ToUpper(Path.GetExtension());

	if (".BMP" == UpperExt)
	{
		hBitMap = reinterpret_cast<HBITMAP>(LoadImageA(nullptr, Path.GetFullPath().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
		ImageType = EWIndowImageType::IMG_BMP;
	}
	else if (".PNG" == UpperExt)
	{
		ULONG_PTR gdiplusToken = 0;

		Gdiplus::GdiplusStartupInput gdistartupinput;
		Gdiplus::GdiplusStartup(&gdiplusToken, &gdistartupinput, nullptr);

		// Gdiplus::Image::FromFile은 와이드 문자 경로를 입력으로 받는다.
		std::wstring wPath = UEngineString::AnsiToUnicode(Path.GetFullPath());
		Gdiplus::Image* pImage = Gdiplus::Image::FromFile(wPath.c_str());
		Gdiplus::Bitmap* pBitMap = reinterpret_cast<Gdiplus::Bitmap*>(pImage->Clone());

		// 윈도우 HBITMAP으로 넣어준다.
		Gdiplus::Status stat = pBitMap->GetHBITMAP(Gdiplus::Color(0, 0, 0, 0), &hBitMap);

		if (Gdiplus::Status::Ok != stat)
		{
			MsgBoxAssert("Png 형식 리소스 로드에 실패했습니다.");
		}

		ImageType = EWIndowImageType::IMG_PNG;
	}

	// 윈도우 DC와 호환되는 DC를 생성한다.
	ImageDC = CreateCompatibleDC(_Image->ImageDC);

	if (nullptr == ImageDC)
	{
		MsgBoxAssert("이미지 생성에 실패했습니다");
		return false;
	}

	// ImageDC가 hBitMap을 선택한다.
	// ImageDC가 갖고 있던 1x1 비트맵은 제거한다.
	HBITMAP OldBitMap = reinterpret_cast<HBITMAP>(SelectObject(ImageDC, hBitMap));
	DeleteObject(OldBitMap);

	// 비트맵 정보 갱신
	GetObject(hBitMap, sizeof(BITMAP), &BitMapInfo);

	return true;
}


bool UWindowImage::Create(UWindowImage* _Image, const FVector& _Scale)
{
	HANDLE ImageHandle = CreateCompatibleBitmap(_Image->ImageDC, _Scale.iX(), _Scale.iY());

	if (nullptr == ImageHandle)
	{
		MsgBoxAssert("이미지 생성에 실패했습니다");
		return false;
	}

	hBitMap = reinterpret_cast<HBITMAP>(ImageHandle);

	ImageDC = CreateCompatibleDC(_Image->ImageDC);

	if (nullptr == ImageDC)
	{
		MsgBoxAssert("이미지 생성에 실패했습니다");
		return false;
	}

	HBITMAP OldBitMap = reinterpret_cast<HBITMAP>(SelectObject(ImageDC, hBitMap));
	DeleteObject(OldBitMap);

	GetObject(hBitMap, sizeof(BITMAP), &BitMapInfo);

	return true;
}

void UWindowImage::BitCopy(UWindowImage* _CopyImage, const FTransform& _Trans)
{
	if (nullptr == _CopyImage)
	{
		MsgBoxAssert("nullptr 인 이미지를 복사할 수 없습니다");
	}

	// 대상 이미지 DC (dest)
	HDC hdc = ImageDC;

	// 소스 이미지 DC (src)
	HDC hdcSrc = _CopyImage->ImageDC;

	BitBlt(
		hdc, 					
		_Trans.iLeft(), 			
		_Trans.iTop(), 				 
		_Trans.GetScale().iX(), 		  
		_Trans.GetScale().iY(),		
		hdcSrc,						
		0,							
		0,								
		SRCCOPY						
	);
}

void UWindowImage::TransCopy(UWindowImage* _CopyImage, const FTransform& _Trans, const FTransform& _ImageTrans, Color8Bit _Color)
{
	if (nullptr == _CopyImage)
	{
		MsgBoxAssert("nullptr인 이미지를 복사할 수 없습니다");
	}

	int RenderLeft = _Trans.iLeft();
	int RenderTop = _Trans.iTop();
	int RenderScaleX = _Trans.GetScale().iX();
	int RenderScaleY = _Trans.GetScale().iY();

	int ImageLeft = _ImageTrans.GetPosition().iX();
	int ImageTop = _ImageTrans.GetPosition().iY();
	int ImageScaleX = _ImageTrans.GetScale().iX();
	int ImageScaleY = _ImageTrans.GetScale().iY();

	// 대상 이미지 DC (dest)
	HDC hdc = ImageDC;

	// 소스 이미지 DC (src)
	HDC hdcSrc = _CopyImage->ImageDC;

	// Transparent bit Block transfer
	TransparentBlt(
		hdc, 							 
		RenderLeft, 		  
		RenderTop, 		  
		RenderScaleX,		
		RenderScaleY,		    
		hdcSrc,						
		ImageLeft,   				
		ImageTop,   					 
		ImageScaleX, 							 
		ImageScaleY, 							
		_Color.Color						
	);
}

bool UWindowImage::Create(HDC _MainDC)
{
	ImageDC = _MainDC;

	if (nullptr == ImageDC)
	{
		return false;
	}

	return true;
}
