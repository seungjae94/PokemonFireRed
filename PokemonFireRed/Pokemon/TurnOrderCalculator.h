#pragma once
#include "Pokemon.h"
#include "BattleEnums.h"
#include "StatStage.h"
#include "Battler.h"

/*
* ������� �켱���� (�������� ���� ����)
* 6. Switch/Escape (�÷��̾ ����)
* 1. QuickAttack
* 0. Other Moves
*/
class UTurnOrderCalculator
{
public:
	// constructor destructor
	UTurnOrderCalculator();
	~UTurnOrderCalculator();

	// delete Function
	UTurnOrderCalculator(const UTurnOrderCalculator& _Other) = delete;
	UTurnOrderCalculator(UTurnOrderCalculator&& _Other) noexcept = delete;
	UTurnOrderCalculator& operator=(const UTurnOrderCalculator& _Other) = delete;
	UTurnOrderCalculator& operator=(UTurnOrderCalculator&& _Other) noexcept = delete;

	static bool IsPlayerFirst(const UBattler* _Player, const UBattler* _Enemy);

	static bool IsPlayerFirstEOT(const UBattler* _Player, const UBattler* _Enemy);

protected:

private:
	static int ActionToPriority(const UBattler* _Battler);
	static int MoveIdToPriority(EPokemonMove _MoveId);
	static int CalcModifiedSpeed(const UBattler* _Battler);
};

