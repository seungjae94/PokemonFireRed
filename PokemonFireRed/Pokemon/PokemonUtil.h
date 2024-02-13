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

	static std::vector<int> FrameCountVectorToIndexVector(std::vector<int> _FrameCounts);

	static std::vector<std::string> StringSplit(const std::string& _Str, char _Delimeter = '\n');
	static std::vector<std::wstring> StringSplit(const std::wstring& _Str, wchar_t _Delimeter = L'\n');

protected:

private:

};

