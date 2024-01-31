#include "WindowImage.h"

#include <EngineBase/EngineDebug.h>
#include <EngineBase/EngineString.h>

// GDI ���� ���̺귯��
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

		// Gdiplus::Image::FromFile�� ���̵� ���� ��θ� �Է����� �޴´�.
		std::wstring wPath = UEngineString::AnsiToUnicode(Path.GetFullPath());
		Gdiplus::Image* pImage = Gdiplus::Image::FromFile(wPath.c_str());
		Gdiplus::Bitmap* pBitMap = reinterpret_cast<Gdiplus::Bitmap*>(pImage->Clone());

		// ������ HBITMAP���� �־��ش�.
		Gdiplus::Status stat = pBitMap->GetHBITMAP(Gdiplus::Color(0, 0, 0, 0), &hBitMap);

		if (Gdiplus::Status::Ok != stat)
		{
			MsgBoxAssert("Png ���� ���ҽ� �ε忡 �����߽��ϴ�.");
		}

		ImageType = EWIndowImageType::IMG_PNG;
	}

	// ������ DC�� ȣȯ�Ǵ� DC�� �����Ѵ�.
	ImageDC = CreateCompatibleDC(_Image->ImageDC);

	if (nullptr == ImageDC)
	{
		MsgBoxAssert("�̹��� ������ �����߽��ϴ�");
		return false;
	}

	// ImageDC�� hBitMap�� �����Ѵ�.
	// ImageDC�� ���� �ִ� 1x1 ��Ʈ���� �����Ѵ�.
	HBITMAP OldBitMap = reinterpret_cast<HBITMAP>(SelectObject(ImageDC, hBitMap));
	DeleteObject(OldBitMap);

	// ��Ʈ�� ���� ����
	GetObject(hBitMap, sizeof(BITMAP), &BitMapInfo);

	return true;
}


bool UWindowImage::Create(UWindowImage* _Image, const FVector& _Scale)
{
	HANDLE ImageHandle = CreateCompatibleBitmap(_Image->ImageDC, _Scale.iX(), _Scale.iY());

	if (nullptr == ImageHandle)
	{
		MsgBoxAssert("�̹��� ������ �����߽��ϴ�");
		return false;
	}

	hBitMap = reinterpret_cast<HBITMAP>(ImageHandle);

	ImageDC = CreateCompatibleDC(_Image->ImageDC);

	if (nullptr == ImageDC)
	{
		MsgBoxAssert("�̹��� ������ �����߽��ϴ�");
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
		MsgBoxAssert("nullptr �� �̹����� ������ �� �����ϴ�");
	}

	// ��� �̹��� DC (dest)
	HDC hdc = ImageDC;

	// �ҽ� �̹��� DC (src)
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
		MsgBoxAssert("nullptr�� �̹����� ������ �� �����ϴ�");
	}

	int RenderLeft = _Trans.iLeft();
	int RenderTop = _Trans.iTop();
	int RenderScaleX = _Trans.GetScale().iX();
	int RenderScaleY = _Trans.GetScale().iY();

	int ImageLeft = _ImageTrans.GetPosition().iX();
	int ImageTop = _ImageTrans.GetPosition().iY();
	int ImageScaleX = _ImageTrans.GetScale().iX();
	int ImageScaleY = _ImageTrans.GetScale().iY();

	// ��� �̹��� DC (dest)
	HDC hdc = ImageDC;

	// �ҽ� �̹��� DC (src)
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
