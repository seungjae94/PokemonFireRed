#include "WindowImage.h"
#include <EngineBase\EngineString.h>
#include <Windows.h>
#include <EngineBase\EngineDebug.h>
#include <EngineBase\EngineDirectory.h>
#include <EngineBase\EngineFile.h>

// GDI ���� ���̺귯��
#pragma comment(lib, "Msimg32.lib")
#include <objidl.h>
#include <gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")

UWindowImage::UWindowImage()
{
}

UWindowImage::~UWindowImage()
{
	switch (LoadType)
	{
	case EImageLoadType::IMG_Folder:
	{
		for (size_t i = 0; i < Infos.size(); i++)
		{
			DeleteObject(Infos[i].hBitMap);
			DeleteDC(Infos[i].ImageDC);
		}
		break;
	}
	case EImageLoadType::IMG_Cutting:
		DeleteObject(hBitMap);
		DeleteDC(ImageDC);
		break;
	default:
		break;
	}
}

FVector UWindowImage::GetScale()
{
	return FVector(BitMapInfo.bmWidth, BitMapInfo.bmHeight);
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

bool UWindowImage::Load(UWindowImage* _Image)
{
	LoadType = EImageLoadType::IMG_Cutting;

	UEnginePath Path = GetEnginePath();

	std::string UpperExt = UEngineString::ToUpper(Path.GetExtension());

	if (".BMP" == UpperExt)
	{
		HANDLE ImageHandle = LoadImageA(nullptr, Path.GetFullPath().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		hBitMap = reinterpret_cast<HBITMAP>(ImageHandle);
		ImageType = EWIndowImageType::IMG_BMP;
	}
	else if (".PNG" == UpperExt)
	{
		ULONG_PTR gdiplusToken = 0;

		Gdiplus::GdiplusStartupInput gdistartupinput;
		Gdiplus::GdiplusStartup(&gdiplusToken, &gdistartupinput, nullptr);

		// Gdiplus::Image::FromFile�� ���̵� ���� ��θ� �Է����� �޴´�.
		std::wstring wPath = UEngineString::AnsiToUniCode(Path.GetFullPath());
		Gdiplus::Image* pImage = Gdiplus::Image::FromFile(wPath.c_str());
		Gdiplus::Bitmap* pBitMap = reinterpret_cast<Gdiplus::Bitmap*>(pImage->Clone());

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

	// BitMapInfo�� ��Ʈ���� �ʺ�, ���� ���� ������ ���� �� �ִ� BITMAP ����ü�̴�.
	// ���� ��Ʈ�� ��ü�� Ŀ�� ������Ʈ�� �츮�� ������ �� ����.
	// �׷��� hBitMap�� ����Ű�� Ŀ�� ������Ʈ�� ������ BitMapInfo�� ����ִ� �Լ��� GetObject�� ȣ���ؾ�����
	// BitMapInfo�� ���� ��Ʈ�� ������ ����� �� �ִ�.
	GetObject(hBitMap, sizeof(BITMAP), &BitMapInfo);

	UImageInfo Info;
	Info.hBitMap = hBitMap;
	Info.ImageDC = ImageDC;
	Info.CuttingTrans.SetPosition({ 0,0 });
	Info.CuttingTrans.SetScale(GetScale());
	Info.ImageType = ImageType;
	Infos.push_back(Info);

	return true;
}


bool UWindowImage::LoadFolder(UWindowImage* _Image)
{
	LoadType = EImageLoadType::IMG_Folder;

	UEnginePath EnginePath = GetEnginePath();

	if (false == EnginePath.IsDirectory())
	{
		MsgBoxAssert("���丮�� �ƴ� ��η� �����ε带 �Ϸ��� �߽��ϴ�");
	}

	UEngineDirectory Dir = EnginePath;

	std::list<UEngineFile> NewList = Dir.AllFile({ ".png", ".bmp" }, false);
	Infos.reserve(NewList.size());

	for (UEngineFile& File : NewList)
	{
		UEnginePath Path = File;

		std::string UpperExt = UEngineString::ToUpper(Path.GetExtension());

		if (".BMP" == UpperExt)
		{
			HANDLE ImageHandle = LoadImageA(nullptr, Path.GetFullPath().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			hBitMap = reinterpret_cast<HBITMAP>(ImageHandle);
			ImageType = EWIndowImageType::IMG_BMP;
		}
		else if (".PNG" == UpperExt)
		{
			ULONG_PTR gdiplusToken = 0;
			Gdiplus::GdiplusStartupInput gdistartupinput;
			Gdiplus::GdiplusStartup(&gdiplusToken, &gdistartupinput, nullptr);
			std::wstring wPath = UEngineString::AnsiToUniCode(Path.GetFullPath());
			Gdiplus::Image* pImage = Gdiplus::Image::FromFile(wPath.c_str());
			Gdiplus::Bitmap* pBitMap = reinterpret_cast<Gdiplus::Bitmap*>(pImage->Clone());
			Gdiplus::Status stat = pBitMap->GetHBITMAP(Gdiplus::Color(0, 0, 0, 0), &hBitMap);
			if (Gdiplus::Status::Ok != stat)
			{
				MsgBoxAssert("Png ���� ���ҽ� �ε忡 �����߽��ϴ�.");
			}

			ImageType = EWIndowImageType::IMG_PNG;
		}
		ImageDC = CreateCompatibleDC(_Image->ImageDC);

		if (nullptr == ImageDC)
		{
			MsgBoxAssert("�̹��� ������ �����߽��ϴ�");
			return false;
		}

		HBITMAP OldBitMap = reinterpret_cast<HBITMAP>(SelectObject(ImageDC, hBitMap));
		DeleteObject(OldBitMap);
		GetObject(hBitMap, sizeof(BITMAP), &BitMapInfo);

		UImageInfo Info;
		Info.hBitMap = hBitMap;
		Info.ImageDC = ImageDC;
		Info.CuttingTrans.SetPosition({ 0,0 });
		Info.CuttingTrans.SetScale(GetScale());
		Infos.push_back(Info);
	}


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

void UWindowImage::TransCopy(UWindowImage* _CopyImage, const FTransform& _Trans, int _Index, Color8Bit _Color)
{
	if (nullptr == _CopyImage)
	{
		MsgBoxAssert("nullptr �� �̹����� ������ �� �����ϴ�");
	}

	if (_Index >= _CopyImage->Infos.size())
	{
		MsgBoxAssert(GetName() + "�̹��� ������ �ε����� �����Ͽ� ����߽��ϴ�");
	}

	FTransform& ImageTrans = _CopyImage->Infos[_Index].CuttingTrans;

	// �������� (RenderLeft, RenderTop)���� (RenderLeft + RenderScaleX, RenderTop + RenderScaleY)������ ������ �׸��� �׸���.
	int RenderLeft = _Trans.iLeft();
	int RenderTop = _Trans.iTop();
	int RenderScaleX = _Trans.GetScale().iX();
	int RenderScaleY = _Trans.GetScale().iY();

	// �̹��� ���ҽ��� (ImageLeft, ImageTop)���� (ImageLeft + ImageScaleX, ImageRight + ImageScaleY)������ ������ �߶󳽴�.
	int ImageLeft = ImageTrans.GetPosition().iX();
	int ImageTop = ImageTrans.GetPosition().iY();
	int ImageScaleX = ImageTrans.GetScale().iX();
	int ImageScaleY = ImageTrans.GetScale().iY();

	// ��� �̹��� DC (dest)
	HDC hdc = ImageDC;
	// �ҽ� �̹��� DC (src)
	HDC hdcSrc = _CopyImage->Infos[_Index].ImageDC;

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

void UWindowImage::AlphaCopy(UWindowImage* _CopyImage, const FTransform& _Trans, int _Index, Color8Bit _Color)
{
	if (nullptr == _CopyImage)
	{
		MsgBoxAssert("nullptr �� �̹����� ������ �� �����ϴ�");
	}

	if (_Index >= _CopyImage->Infos.size())
	{
		MsgBoxAssert(GetName() + "�̹��� ������ �ε����� �����Ͽ� ����߽��ϴ�");
	}


	FTransform& ImageTrans = _CopyImage->Infos[_Index].CuttingTrans;

	int RenderLeft = _Trans.iLeft();
	int RenderTop = _Trans.iTop();
	int RenderScaleX = _Trans.GetScale().iX();
	int RenderScaleY = _Trans.GetScale().iY();

	int ImageLeft = ImageTrans.GetPosition().iX();
	int ImageTop = ImageTrans.GetPosition().iY();
	int ImageScaleX = ImageTrans.GetScale().iX();
	int ImageScaleY = ImageTrans.GetScale().iY();

	HDC hdc = ImageDC;

	HDC hdcSrc = _CopyImage->Infos[_Index].ImageDC;

	BLENDFUNCTION Function;
	Function.BlendOp = AC_SRC_OVER;
	Function.BlendFlags = 0;
	Function.SourceConstantAlpha = _Color.A;
	Function.AlphaFormat = AC_SRC_ALPHA;

	AlphaBlend(
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
		Function
	);
}

void UWindowImage::TextCopy(const std::string& _Text, const std::string& _Font, float _Size, const FTransform& _Trans, Color8Bit _Color)
{
	Gdiplus::Graphics graphics(ImageDC);
	std::wstring WFont = UEngineString::AnsiToUniCode(_Font);
	Gdiplus::Font fnt(WFont.c_str(), _Size, 0, Gdiplus::UnitPixel);
	Gdiplus::SolidBrush hB(Gdiplus::Color(_Color.R, _Color.G, _Color.B));
	FVector Pos = _Trans.GetPosition();
	Gdiplus::RectF  rectF(_Trans.GetPosition().X, _Trans.GetPosition().Y, 0, 0);

	Gdiplus::StringFormat stringFormat;
	stringFormat.SetAlignment(Gdiplus::StringAlignmentCenter);
	stringFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter);
	std::wstring WText = UEngineString::AnsiToUniCode(_Text);
	graphics.DrawString(WText.c_str(), -1, &fnt, rectF, &stringFormat, &hB); 
}

void UWindowImage::Cutting(int _X, int _Y)
{
	Infos.clear();

	FVector CuttingScale = { GetScale().X / _X,  GetScale().Y / _Y };
	FVector CuttingPos = { 0, 0 };

	for (int i = 0; i < _Y; i++)
	{
		for (int i = 0; i < _X; i++)
		{
			UImageInfo Info;
			Info.ImageDC = ImageDC;
			Info.CuttingTrans.SetPosition(CuttingPos);
			Info.CuttingTrans.SetScale(CuttingScale);
			Infos.push_back(Info);

			CuttingPos.X += CuttingScale.X;
		}

		CuttingPos.X = 0.0f;
		CuttingPos.Y += CuttingScale.Y;
	}
}

void UWindowImage::DrawRectangle(const FTransform& _Trans)
{
	Rectangle(ImageDC, _Trans.iLeft(), _Trans.iTop(), _Trans.iRight(), _Trans.iBottom());
}

void UWindowImage::DrawEllipse(const FTransform& _Trans)
{
	Ellipse(ImageDC, _Trans.iLeft(), _Trans.iTop(), _Trans.iRight(), _Trans.iBottom());
}

Color8Bit UWindowImage::GetColor(int _X, int _Y, Color8Bit _DefaultColor)
{
	// �̹��� �ٱ��� �ȼ� ���� �䱸�ϴ� ��� ����Ʈ �÷��� ��ȯ�Ѵ�.
	if (0 > _X)
	{
		return _DefaultColor;
	}

	if (0 > _Y)
	{
		return _DefaultColor;
	}

	if (GetScale().iX() <= _X)
	{
		return _DefaultColor;
	}

	if (GetScale().iY() <= _Y)
	{
		return _DefaultColor;
	}

	Color8Bit Color;
	Color.Color = ::GetPixel(ImageDC, _X, _Y);
	return Color;
}
