#pragma once
#include "PokemonLevel.h"
#include "BagCanvas.h"

class UBagUILevel : public UPokemonLevel
{
private:
	enum class EState
	{
		None,
		TargetSelect,
		ActionSelect,
		End
	};
public:
	// constructor destructor
	UBagUILevel();
	~UBagUILevel();

	// delete Function
	UBagUILevel(const UBagUILevel& _Other) = delete;
	UBagUILevel(UBagUILevel&& _Other) noexcept = delete;
	UBagUILevel& operator=(const UBagUILevel& _Other) = delete;
	UBagUILevel& operator=(UBagUILevel&& _Other) noexcept = delete;

protected:

private:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void LevelStart(ULevel* _PrevLevel) override;
	void LevelEnd(ULevel* _NextLevel) override;

	int Page = 0;
	std::string PrevLevelName;
	ABagCanvas* Canvas = nullptr;
};

