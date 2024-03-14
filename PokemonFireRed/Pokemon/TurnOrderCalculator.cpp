#include "TurnOrderCalculator.h"
#include "BattleLevel.h"
#include "BattleEnemyActionGenerator.h"

UTurnOrderCalculator::UTurnOrderCalculator()
{
}

UTurnOrderCalculator::~UTurnOrderCalculator()
{
}

bool UTurnOrderCalculator::IsPlayerFirst(const UBattler* _Player, const UBattler* _Enemy)
{
    int PlayerPriority = ActionToPriority(_Player);
    int EnemyPriority = ActionToPriority(_Enemy);

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

    int ModifiedPlayerSpeed = CalcModifiedSpeed(_Player);
    int ModifiedEnemySpeed = CalcModifiedSpeed(_Enemy);

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

bool UTurnOrderCalculator::IsPlayerFirstEOT(const UBattler* _Player, const UBattler* _Enemy)
{
    int ModifiedPlayerSpeed = CalcModifiedSpeed(_Player);
    int ModifiedEnemySpeed = CalcModifiedSpeed(_Enemy);

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

int UTurnOrderCalculator::ActionToPriority(const UBattler* _Battler)
{
    switch (_Battler->CurAction())
    {
    case EBattleAction::None:
        break;
    case EBattleAction::Fight:
        break;
    case EBattleAction::Escape:
    case EBattleAction::Shift:
    case EBattleAction::UseItem:
    case EBattleAction::Ball:
        return 6;
    default:
        break;
    }

    return MoveIdToPriority(_Battler->CurMoveId());
}

int UTurnOrderCalculator::MoveIdToPriority(EPokemonMove _MoveId)
{
    if (_MoveId == EPokemonMove::QuickAttack)
    {
        return 1;
    }

    return 0;
}

int UTurnOrderCalculator::CalcModifiedSpeed(const UBattler* _Battler)
{
    const UPokemon* Pokemon = _Battler->CurPokemonReadonly();

    float ModifiedSpeed = static_cast<float>(Pokemon->GetSpeed());
    ModifiedSpeed *= _Battler->StatStage.GetSpeedMultiplier();
   
    if (Pokemon->GetStatusId() == EPokemonStatus::Paralysis)
    {
        ModifiedSpeed *= 1.0f / 4;
    }

    return UPokemonMath::Floor(ModifiedSpeed);
}
