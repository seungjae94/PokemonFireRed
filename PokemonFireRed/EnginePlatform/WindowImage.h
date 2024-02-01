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
	friend UEngineWindow; // ������ �̹���, ����� �̹��� ����
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

	// �� ���ó: ����� �̹��� ����
	// - ���ҽ� �̹����� Load�� �����Ѵ�.
	bool Create(UWindowImage* _Image, const FVector& _Scale);

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
	/// <param name="_CopyTrans">
	/// (�̹��� �������� ���� ��ġ, �̹��� �������� ũ��)�� ��Ÿ���� Ʈ������.
	/// </param>
	/// <param name="_ImageCuttingTrans">
	/// �̹������� �߶� ������ �ǹ��ϴ� Ʈ������.
	/// </param>
	/// <param name="_Color">
	/// �������� ����� ����
	/// </param>
	void TransCopy(UWindowImage* _CopyImage, const FTransform& _CopyTrans, const FTransform& _ImageCuttingTrans, Color8Bit _Color = Color8Bit::Black);

	FVector GetScale();

protected:

private:
	HBITMAP hBitMap = nullptr;
	HDC ImageDC = nullptr;
	BITMAP BitMapInfo = BITMAP();

	EWIndowImageType ImageType = EWIndowImageType::IMG_NONE;

	// �� ���ó: ������ �̹��� ����
	// - ���ҽ� �̹����� Load�� �����Ѵ�.
	bool Create(HDC _MainDC);
};

