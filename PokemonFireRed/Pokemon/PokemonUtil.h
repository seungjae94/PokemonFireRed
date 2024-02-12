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
	/// Source�� Dest�� LeftTop�� ��ġ ��Ű�� ���� �� Source�� � ��ġ�� ������ �ϴ����� ��ȯ�Ѵ�.
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

