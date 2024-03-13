#pragma once
#include "ItemPokemonUILevel.h"

class UBattleItemPokemonUILevel : public UItemPokemonUILevel
{
public:
	// constructor destructor
	UBattleItemPokemonUILevel();
	~UBattleItemPokemonUILevel();

	// delete Function
	UBattleItemPokemonUILevel(const UBattleItemPokemonUILevel& _Other) = delete;
	UBattleItemPokemonUILevel(UBattleItemPokemonUILevel&& _Other) noexcept = delete;
	UBattleItemPokemonUILevel& operator=(const UBattleItemPokemonUILevel& _Other) = delete;
	UBattleItemPokemonUILevel& operator=(UBattleItemPokemonUILevel&& _Other) noexcept = delete;

protected:

private:
	void LevelStart(ULevel* _PrevLevel) override;

	void ProcessItemUseResultMessage() override;
};

