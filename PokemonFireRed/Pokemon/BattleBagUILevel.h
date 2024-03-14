#pragma once
#include "BagUILevel.h"

class UBattleLevel;
class UBattler;

class UBattleBagUILevel : public UBagUILevel
{
public:
	// constructor destructor
	UBattleBagUILevel();
	~UBattleBagUILevel();

	// delete Function
	UBattleBagUILevel(const UBattleBagUILevel& _Other) = delete;
	UBattleBagUILevel(UBattleBagUILevel&& _Other) noexcept = delete;
	UBattleBagUILevel& operator=(const UBattleBagUILevel& _Other) = delete;
	UBattleBagUILevel& operator=(UBattleBagUILevel&& _Other) noexcept = delete;

	UBattler* GetPlayerBattler();
	UBattleLevel* GetBattleLevel();

protected:

private:
	UBattleLevel* BattleLevel = nullptr;
	UBattler* PlayerBattler = nullptr;

	void LevelStart(ULevel* _PrevLevel) override;

	void SelectTarget() override;
	void CancelTargetSelection() override;
	void SelectAction() override;
};

