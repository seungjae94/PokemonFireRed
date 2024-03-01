#include "TurnOrderCalculator.h"
#include "BattleLevel.h"
#include "BattleEnemyActionGenerator.h"

EBattlePlayerAction UTurnOrderCalculator::PlayerAction = EBattlePlayerAction::None;
EBattleEnemyAction UTurnOrderCalculator::EnemyAction = EBattleEnemyAction::None;
const UPokemon* UTurnOrderCalculator::PlayerPokemon = nullptr;
const UPokemon* UTurnOrderCalculator::EnemyPokemon = nullptr;
int UTurnOrderCalculator::PlayerMoveIndex = 0;
int UTurnOrderCalculator::EnemyMoveIndex = 0;

UTurnOrderCalculator::UTurnOrderCalculator()
{
}

UTurnOrderCalculator::~UTurnOrderCalculator()
{
}

bool UTurnOrderCalculator::IsPlayerFirst(
    EBattlePlayerAction _PlayerAction, EBattleEnemyAction _EnemyAction,
    const UPokemon* _PlayerPokemon, const UPokemon* _EnemyPokemon,
    int _PlayerMoveIndex, int _EnemyMoveIndex
)
{
    PlayerAction = _PlayerAction;
    EnemyAction = _EnemyAction;
    PlayerPokemon = _PlayerPokemon;
    EnemyPokemon = _EnemyPokemon;
    PlayerMoveIndex = _PlayerMoveIndex;
    EnemyMoveIndex = _EnemyMoveIndex;

    int PlayerPriority = ActionToPlayerPriority();
    int EnemyPriority = ActionToEnemyPriority();

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
    int PlayerSpeed = PlayerPokemon->GetSpeed();
    int EnemySpeed = EnemyPokemon->GetSpeed();

    if (PlayerSpeed > EnemySpeed)
    {
        return true;
    }
    else if (PlayerSpeed < EnemySpeed)
    {
        return false;
    }

    int RandomInt = UPokemonMath::Random(0, 1);
    return RandomInt == 0;
}

int UTurnOrderCalculator::ActionToPlayerPriority()
{
    switch (PlayerAction)
    {
    case EBattlePlayerAction::None:
        break;
    case EBattlePlayerAction::Fight:
        break;
    case EBattlePlayerAction::EscapeFail:
    case EBattlePlayerAction::Shift:
    case EBattlePlayerAction::Item:
        return 6;
    default:
        break;
    }

    EPokemonMove MoveId = PlayerPokemon->GetMoveId(PlayerMoveIndex);
    return MoveIdToPriority(MoveId);
}

int UTurnOrderCalculator::ActionToEnemyPriority()
{
    switch (EnemyAction)
    {
    case EBattleEnemyAction::None:
        break;
    case EBattleEnemyAction::Move:
        break;
    case EBattleEnemyAction::Shift:
    case EBattleEnemyAction::Item:
        return 6;
    default:
        break;
    }

    EPokemonMove MoveId = EnemyPokemon->GetMoveId(EnemyMoveIndex);
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
