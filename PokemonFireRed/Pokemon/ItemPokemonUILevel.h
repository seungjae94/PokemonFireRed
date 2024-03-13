#pragma once
#include "PokemonUILevel.h"

class UBagUILevel;

class EItemPokemonUIState : public EPokemonUIState
{
public:
	static const EItemPokemonUIState TestItemUse;
	static const EItemPokemonUIState HpUpEffect;
	static const EItemPokemonUIState ItemUseResultMessage;
};

class UItemPokemonUILevel : public UPokemonUILevel
{
public:
	// constructor destructor
	UItemPokemonUILevel();
	~UItemPokemonUILevel();

	// delete Function
	UItemPokemonUILevel(const UItemPokemonUILevel& _Other) = delete;
	UItemPokemonUILevel(UItemPokemonUILevel&& _Other) noexcept = delete;
	UItemPokemonUILevel& operator=(const UItemPokemonUILevel& _Other) = delete;
	UItemPokemonUILevel& operator=(UItemPokemonUILevel&& _Other) noexcept = delete;

	const FItem* GetUseItem() const;
protected:
	void LevelStart(ULevel* _PrevLevel) override;
private:
	UBagUILevel* BagUILevel = nullptr;
	const FItem* UseItem = nullptr;
	const float HealTime = 0.5f;
	float Timer = 0.0f;
	int PrevHealHp = 0;
	int NextHealHp = 0;

	void ProcessTestItemUse();
	void ProcessHpUpEffect();
	virtual void ProcessItemUseResultMessage() {};

	// 상태 전이 함수
	void SelectTarget() override;
	void CancelTargetSelection() override;
};

