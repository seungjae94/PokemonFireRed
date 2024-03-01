#pragma once
#include "Pokemon.h"

enum class EBattlePlayerAction;
enum class EBattleEnemyAction;

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
		EBattlePlayerAction _PlayerAction, EBattleEnemyAction _EnemyAction,
		const UPokemon* _PlayerPokemon, const UPokemon* _EnemyPokemon,
		int _PlayerMoveIndex, int _EnemyMoveIndex
	);

protected:

private:
	static EBattlePlayerAction PlayerAction;
	static EBattleEnemyAction EnemyAction;
	static const UPokemon* PlayerPokemon;
	static const UPokemon* EnemyPokemon;
	static int PlayerMoveIndex;
	static int EnemyMoveIndex;

	static int ActionToPlayerPriority();
	static int ActionToEnemyPriority();
	static int MoveIdToPriority(EPokemonMove _MoveId);
};

