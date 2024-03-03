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

	// TODO: Ʈ���̳� ����ġ, ������ ����� ���� �߰�
	const UPokemon* EnemyPokemon = _Enemy->CurPokemonReadonly();
	int RandomMoveIndex = UPokemonMath::RandomInt(0, EnemyPokemon->GetMoveCount() - 1);
	_Enemy->SetAction(EBattleAction::Fight);
	_Enemy->SetMoveIndex(RandomMoveIndex);
	return;
}
