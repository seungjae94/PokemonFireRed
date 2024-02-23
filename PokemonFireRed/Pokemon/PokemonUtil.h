#pragma once
#include <EngineBase/EngineMath.h>
#include <EngineBase/Transform.h>
#include <EngineCore/Actor.h>
#include "Global.h"

class UPokemonUtil
{
public:
	// constructor destructor
	UPokemonUtil();
	~UPokemonUtil();

	// delete Function
	UPokemonUtil(const UPokemonUtil& _Other) = delete;
	UPokemonUtil(UPokemonUtil&& _Other) noexcept = delete;
	UPokemonUtil& operator=(const UPokemonUtil& _Other) = delete;
	UPokemonUtil& operator=(UPokemonUtil&& _Other) noexcept = delete;

	static FVector GetLeftTop(const FTransform& _Trans)
	{
		return _Trans.GetPosition() - _Trans.GetScale().Half2D();
	}

	/// <summary>
	/// Source�� Dest�� LeftTop�� ��ġ ��Ű�� ���� �� Source�� � ��ġ�� ������ �ϴ����� ��ȯ�Ѵ�.
	/// </summary>
	static FVector GetMatchLeftTop(const FVector& _SrcScale, const FTransform& _DestTrans)
	{
		return GetLeftTop(_DestTrans) + FVector(_SrcScale).Half2D();
	}

	// �̹����� ȭ�� �»�ܿ� �߸��� �ʰ� ��ġ�ϰ� ���� �� �̹����� ��ġ
	static FVector GetLeftTopAlignPos(const FVector& _Scale)
	{
		FVector Scale = _Scale;
		return Scale.Half2D();
	}

	// �̹����� ȭ�� ���ϴܿ� �߸��� �ʰ� ��ġ�ϰ� ���� �� �̹����� ��ġ
	static FVector GetLeftBotAlignPos(const FVector& _Scale)
	{
		return FVector(0, Global::ScreenY) + FVector(_Scale.X / 2, -_Scale.Y / 2);
	}

	// �̹����� ȭ�� ���ܿ� �߸��� �ʰ� ��ġ�ϰ� ���� �� �̹����� ��ġ
	static FVector GetRightTopAlignPos(const FVector& _Scale)
	{
		return FVector(Global::ScreenX, 0) + FVector(-_Scale.X / 2, _Scale.Y / 2);
	}

	// �̹����� ȭ�� ���ϴܿ� �߸��� �ʰ� ��ġ�ϰ� ���� �� �̹����� ��ġ
	static FVector GetRightBotAlignPos(const FVector& _Scale)
	{
		FVector Scale = _Scale;
		return FVector(Global::ScreenX, Global::ScreenY) - Scale.Half2D();
	}

	static void ToggleActive(AActor* _Actor)
	{
		bool ActiveValue = _Actor->IsActive();

		if (true == ActiveValue)
		{
			_Actor->ActiveOff();
		}
		else
		{
			_Actor->ActiveOn();
		}
	}

	static std::vector<int> FrameCountVectorToIndexVector(std::vector<int> _FrameCounts);

	static FVector PixelVector(int _X, int _Y)
	{
		return FVector(_X, _Y) * Global::FloatPixelSize;
	}

	static FVector GetImageScale(UImageRenderer* _Renderer);
	static FVector GetRenderScale(UImageRenderer* _Renderer);
	static void CreateMiniPokemonAnimations(UImageRenderer* _Renderer);

	static std::vector<std::string> StringSplit(const std::string& _Str, char _Delimeter = '\n');
	static std::vector<std::wstring> StringSplit(std::wstring_view _Str, wchar_t _Delimeter = L'\n');

protected:

private:

};

