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

// �̹��� ������ �����ϴ� ��ü
// - Name, Path�� ������ ������ UPathObject ���
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
	/// Path�� ��ġ�� ���� �̹����� hBitMap �ʵ忡 �ε��Ѵ�. �� hBitMap �ʵ带 ����ϴ� ImageDC�� �����Ѵ�.
	/// </summary>
	/// <param name="_Image">
	/// </param>
	/// <returns>
	/// �̹��� �ε� ���� ����
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

