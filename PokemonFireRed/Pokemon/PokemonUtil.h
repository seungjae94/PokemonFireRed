#pragma once
#include <EngineBase/EngineMath.h>
#include <EngineBase/Transform.h>
#include <EngineCore/Actor.h>
#include "Global.h"
#include "Page.h"

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

	static void AlignImage(
		UImageRenderer* _Renderer, 
		EPivotType _PivotType
	);

	static void PlaceImageOnScreen(
		UImageRenderer* _Renderer, 
		EPivotType _PivotType = EPivotType::LeftTop,
		int _ScreenPixelX = 0, int _ScreenPixelY = 0
	);

	static void CreatePokemonMiniMoveAnimations(UImageRenderer* _Renderer);
	static void CreatePokemonMiniStopAnimations(UImageRenderer* _Renderer);
	static void CreatePokemonFrontAnimations(UImageRenderer* _Renderer);
	static void CreatePokemonBackAnimations(UImageRenderer* _Renderer);

	static std::vector<std::string> StringSplit(const std::string& _Str, char _Delimeter = '\n');
	static std::vector<std::wstring> StringSplit(std::wstring_view _Str, wchar_t _Delimeter = L'\n');

protected:

private:

};

