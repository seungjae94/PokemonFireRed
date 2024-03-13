#pragma once
#include "ItemPokemonUILevel.h"

class UBagItemPokemonUILevel : public UItemPokemonUILevel
{
public:
	// constructor destructor
	UBagItemPokemonUILevel();
	~UBagItemPokemonUILevel();

	// delete Function
	UBagItemPokemonUILevel(const UBagItemPokemonUILevel& _Other) = delete;
	UBagItemPokemonUILevel(UBagItemPokemonUILevel&& _Other) noexcept = delete;
	UBagItemPokemonUILevel& operator=(const UBagItemPokemonUILevel& _Other) = delete;
	UBagItemPokemonUILevel& operator=(UBagItemPokemonUILevel&& _Other) noexcept = delete;

protected:

private:
	void LevelStart(ULevel* _PrevLevel) override;

	void ProcessItemUseResultMessage() override;

	// 상태 전이 함수
	void CancelTargetSelection() override;
};

