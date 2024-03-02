#include "BattleEnemyActionGenerator.h"
#include "BattleLevel.h"

int UBattleEnemyActionGenerator::MoveIndex = 0;

UBattleEnemyActionGenerator::UBattleEnemyActionGenerator()
{
}

UBattleEnemyActionGenerator::~UBattleEnemyActionGenerator()
{
}

EBattleAction UBattleEnemyActionGenerator::Generate(EEnemyType _EnemyType, const UPokemon* _EnemyPokemon)
{
    // Switch ���, Item ����� ���߿� ������ ����
    MoveIndex = UPokemonMath::RandomInt(0, _EnemyPokemon->GetMoveCount() - 1);
    return EBattleAction::Fight;
}
