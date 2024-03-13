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
protected:
	void LevelStart(ULevel* _PrevLevel) override;
	void Tick(float _DeltaTime) override;

	bool ItemUseResult = false;
	const FItem* UseItem = nullptr;
private:
	UBagUILevel* BagUILevel = nullptr;
	const float HealTime = 0.5f;
	float Timer = 0.0f;
	int PrevHealHp = 0;
	int NextHealHp = 0;

	void ProcessTestItemUse();
	void ProcessHpUpEffect();
	virtual void ProcessItemUseResultMessage() {};

	// ���� ���� �Լ�
	void SelectTarget() override;
};

