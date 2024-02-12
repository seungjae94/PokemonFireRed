#pragma once
#include <EngineBase/EngineMath.h>
#include <EngineBase/Transform.h>
#include <EngineCore/Actor.h>

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
	/// Source와 Dest의 LeftTop을 일치 시키고 싶을 때 Source가 어떤 위치를 가져야 하는지를 반환한다.
	/// </summary>
	static FVector GetMatchLeftTop(const FVector& _SrcScale, const FTransform& _DestTrans)
	{
		return GetLeftTop(_DestTrans) + FVector(_SrcScale).Half2D();
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

protected:

private:

};

