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
    int _PlayerMoveIndex, int _EnemyMoveIndex,
    const UStatStage* _PlayerStatStage, const UStatStage* _EnemyStatStage
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

    int ModifiedPlayerSpeed = CalcModifiedSpeed(_PlayerPokemon, _PlayerStatStage);
    int ModifiedEnemySpeed = CalcModifiedSpeed(_PlayerPokemon, _PlayerStatStage);

    if (ModifiedPlayerSpeed > ModifiedEnemySpeed)
    {
        return true;
    }
    else if (ModifiedPlayerSpeed < ModifiedEnemySpeed)
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

int UTurnOrderCalculator::CalcModifiedSpeed(const UPokemon* _Pokemon, const UStatStage* _StatStage)
{
    float ModifiedSpeed = static_cast<float>(_Pokemon->GetSpeed());
    ModifiedSpeed *= _StatStage->GetSpeedMultiplier();
    // if _Pokemon is in "Paralysis" Status, then ModifiedSpeed *= 1.0f/4
    return UPokemonMath::Floor(ModifiedSpeed);
}
