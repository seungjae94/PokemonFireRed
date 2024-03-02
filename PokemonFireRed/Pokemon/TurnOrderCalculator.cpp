#include "TurnOrderCalculator.h"
#include "BattleLevel.h"
#include "BattleEnemyActionGenerator.h"

UTurnOrderCalculator::UTurnOrderCalculator()
{
}

UTurnOrderCalculator::~UTurnOrderCalculator()
{
}

bool UTurnOrderCalculator::IsPlayerFirst(
    EBattleAction _PlayerAction, EBattleAction _EnemyAction,
    const UPokemon* _PlayerPokemon, const UPokemon* _EnemyPokemon,
    int _PlayerMoveIndex, int _EnemyMoveIndex
)
{
    int PlayerPriority = ActionToPriority(_PlayerAction, _PlayerPokemon, _PlayerMoveIndex);
    int EnemyPriority = ActionToPriority(_EnemyAction, _EnemyPokemon, _EnemyMoveIndex);

    if (PlayerPriority > EnemyPriority)
    {
        return true;
    }
    else if (PlayerPriority < EnemyPriority)
    {
        return false;
    }
    else if (PlayerPriority == 6)
    {
        return true;
    }

    // 이외의 경우 속도를 비교한다.
    // TODO: 
    // 1. Stat stage 구현 후 반영
    // 2. PAR 상태 구현 후 반형
    int PlayerSpeed = _PlayerPokemon->GetSpeed();
    int EnemySpeed = _EnemyPokemon->GetSpeed();

    if (PlayerSpeed > EnemySpeed)
    {
        return true;
    }
    else if (PlayerSpeed < EnemySpeed)
    {
        return false;
    }

    int RandomInt = UPokemonMath::RandomInt(0, 1);
    return RandomInt == 0;
}

int UTurnOrderCalculator::ActionToPriority(EBattleAction _Action, const UPokemon* _Pokemon, int _MoveIndex)
{
    switch (_Action)
    {
    case EBattleAction::None:
        break;
    case EBattleAction::Fight:
        break;
    case EBattleAction::EscapeFail:
    case EBattleAction::Shift:
    case EBattleAction::Item:
        return 6;
    default:
        break;
    }

    EPokemonMove MoveId =_Pokemon->GetMoveId(_MoveIndex);
    return MoveIdToPriority(MoveId);
}

int UTurnOrderCalculator::MoveIdToPriority(EPokemonMove _MoveId)
{
    if (_MoveId == EPokemonMove::QuickAttack)
    {
        return 1;
    }

    return 0;
}
