#pragma once
#include "Pokemon.h"
#include "BattleEnums.h"

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

	static bool IsPlayerFirst(
		EBattleAction _PlayerAction, EBattleAction _EnemyAction,
		const UPokemon* _PlayerPokemon, const UPokemon* _EnemyPokemon,
		int _PlayerMoveIndex, int _EnemyMoveIndex
	);

protected:

private:
	static int ActionToPriority(EBattleAction _Action, const UPokemon* _Pokemon, int _MoveIndex);
	static int MoveIdToPriority(EPokemonMove _MoveId);
};

