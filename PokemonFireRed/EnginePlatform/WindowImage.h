#pragma once
#include <EngineBase\PathObject.h>
#include <EngineBase\EngineMath.h>
#include <EngineBase\Transform.h>
#include <Windows.h>

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

class ImageInfo
{
public:
	HBITMAP hBitMap;
	HDC ImageDC = nullptr;
	FTransform CuttingTrans;
	EWIndowImageType ImageType = EWIndowImageType::IMG_NONE;
};

class UEngineWindow;

// �̹��� ������ �����ϴ� ��ü
// - Name, Path�� ������ ������ UPathObject ���
class UWindowImage : public UPathObject
{
	friend UEngineWindow;	// ������ �̹���, ����� �̹��� ����

public:
	UWindowImage();
	~UWindowImage();		// Ŀ�� ������Ʈ ������

	// delete Function
	UWindowImage(const UWindowImage& _Other) = delete;
	UWindowImage(UWindowImage&& _Other) noexcept = delete;
	UWindowImage& operator=(const UWindowImage& _Other) = delete;
	UWindowImage& operator=(UWindowImage&& _Other) noexcept = delete;

	/// <summary>
	/// Path�� ��ġ�� ���ҽ� �̹����� ��Ʈ������ �ε��Ѵ�. 
	/// �� ������ ��Ʈ���� ����ϴ� DC�� �����.
	/// </summary>
	/// <param name="_Image">
	/// ���������� �̹����� �׸� ��� �̹���.
	/// _Image ���ڷδ� ������ �̹����� ��ǻ� �����ȴ�.
	/// </param>
	/// <returns>
	/// �̹��� �ε� ���� ����
	/// </returns>
	bool Load(UWindowImage* _Image);

	bool LoadFolder(UWindowImage* _Image);

	FVector GetScale();

	/// <summary>
	/// �̹��� this�� �̹��� _CopyImage�� �׷��ִ� �Լ�.
	/// �� ���ó: ������ �̹����� ����� �̹����� �׸� �� ���.
	/// </summary>
	/// <param name="_CopyImage">
	/// this�� ������ �̹���
	/// </param>
	/// <param name="_Trans">
	/// this(�����)�� Ʈ������
	/// </param>
	void BitCopy(UWindowImage* _CopyImage, const FTransform& _Trans);

	/// <summary>
	/// �̹��� this�� �̹��� _CopyImage�� �׷��ִ� �Լ�.
	/// �� ���ó: ����� �̹����� �������� ���� �ִ� �̹����� �׸� �� ���.
	/// </summary>
	/// <param name="_CopyImage">
	/// this�� �׸� �̹���
	/// </param>
	/// <param name="_Trans">
	/// (�̹��� �������� ���� ��ġ, �̹��� �������� ũ��)�� ��Ÿ���� Ʈ������.
	/// </param>
	/// <param name="_ImageCuttingTrans">
	/// �̹������� �߶� ������ �ǹ��ϴ� Ʈ������.
	/// </param>
	/// <param name="_Color">
	/// �������� ����� ����
	/// </param>
	void TransCopy(UWindowImage* _CopyImage, const FTransform& _Trans, int _Index, Color8Bit _Color = Color8Bit::Black);

	void AlphaCopy(UWindowImage* _CopyImage, const FTransform& _Trans, int _Index, Color8Bit _Color = Color8Bit::Black);


	// �� ���ó: ����� �̹��� ����
	// - ���ҽ� �̹����� Load�� �����Ѵ�.
	bool Create(UWindowImage* _Image, const FVector& _Scale);

	void Cutting(int _X, int _Y);

	Color8Bit GetColor(int _X, int _Y, Color8Bit _DefaultColor);

	EWIndowImageType GetImageType()
	{
		return ImageType;
	}

protected:

private:
	EImageLoadType LoadType = EImageLoadType::IMG_Cutting;

	HBITMAP hBitMap = 0;
	HDC ImageDC = 0;
	BITMAP BitMapInfo = BITMAP(); 
	EWIndowImageType ImageType = EWIndowImageType::IMG_NONE;

	std::vector<ImageInfo> Infos;

	// �� ���ó: ������ �̹��� ����
	// - ���ҽ� �̹����� Load�� �����Ѵ�.
	bool Create(HDC _MainDC);
};

