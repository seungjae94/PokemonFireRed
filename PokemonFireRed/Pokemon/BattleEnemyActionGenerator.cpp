#include "BattleEnemyActionGenerator.h"
#include "BattleLevel.h"

UBattleEnemyActionGenerator::UBattleEnemyActionGenerator()
{
}

UBattleEnemyActionGenerator::~UBattleEnemyActionGenerator()
{
}

void UBattleEnemyActionGenerator::Generate(UBattler* _Enemy)
{
	if (true == _Enemy->IsWildPokemon())
	{
		const UPokemon* EnemyPokemon = _Enemy->CurPokemonReadonly();
		int RandomMoveIndex = UPokemonMath::RandomInt(0, EnemyPokemon->GetMoveCount() - 1);
		_Enemy->SetAction(EBattleAction::Fight);
		_Enemy->SetMoveIndex(RandomMoveIndex);
		return;
	}

	// TODO: 트레이너 스위치, 아이템 기능은 차차 추가
	const UPokemon* EnemyPokemon = _Enemy->CurPokemonReadonly();
	int RandomMoveIndex = UPokemonMath::RandomInt(0, EnemyPokemon->GetMoveCount() - 1);
	_Enemy->SetAction(EBattleAction::Fight);
	_Enemy->SetMoveIndex(RandomMoveIndex);
	return;
}
