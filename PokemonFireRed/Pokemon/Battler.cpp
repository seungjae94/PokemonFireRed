#include "Battler.h"
#include "PlayerData.h"
#include "EventManager.h"
#include "EventTrigger.h"
#include "Trainer.h"

UBattler::UBattler()
{
}

UBattler::~UBattler()
{
}

void UBattler::Clear()
{
	StatStage.Reset();
	TempStatusId = EPokemonStatus::Normal;
	Entry.clear();
	Action = EBattleAction::None;
	FightingPokemonIndex = 0;
	CurMoveIndex = 0;
	IsPlayerValue = false;
	IsWildPokemonValue = false;
	PlayerWinMessage.clear();
}

void UBattler::InitPlayer()
{
	IsPlayerValue = true;

	int PlayerEntrySize = static_cast<int>(UPlayerData::GetPokemonEntrySize());
	for (int i = 0; i < PlayerEntrySize; ++i)
	{
		Entry.push_back(&UPlayerData::GetPokemonInEntry(i));
	}
}

void UBattler::InitWildPokemon()
{
	IsPlayerValue = false;
	IsWildPokemonValue = true;
	InitEnemyPokemon();
}

void UBattler::InitTrainer()
{
	ATrainer* Trainer = UEventManager::GetTrainer();
	IsPlayerValue = false;
	IsWildPokemonValue = false;
	TrainerName = Trainer->GetBattlerName();
	TrainerImageName = Trainer->GetBattlerImageName();
	PlayerWinMessage = Trainer->GetPlayerWinMessage();
	AfterBattleTrigger = Trainer->GetAfterBattleTrigger();
	InitEnemyPokemon();
}

void UBattler::InitCurPokemon()
{
	for (int i = 0; i < Entry.size(); ++i)
	{
		if (false == Entry[i]->IsFaint())
		{
			FightingPokemonIndex = i;
			return;
		}
	}
}

void UBattler::ShiftPokemon()
{
	StatStage.Reset();
	FightingPokemonIndex = ShiftPokemonIndex;
	CurMoveIndex = 0;
	ShiftPokemonIndex = -1;
}

int UBattler::GetLevel(int _Index)
{
	if (_Index >= Entry.size())
	{
		MsgBoxAssert("UBattler::GetLevel 함수에서 엔트리 인덱스를 초과해 포켓몬의 레벨을 확인하려고 했습니다.");
		return 0;
	}

	return Entry[_Index]->GetLevel();
}

int UBattler::CurMovePP()
{
	return Entry[FightingPokemonIndex]->GetMovePP(CurMoveIndex);
}

void UBattler::DecCurMovePP()
{
	return Entry[FightingPokemonIndex]->DecMovePP(CurMoveIndex);
}

void UBattler::EnemyAutoShift()
{
	for (int i = 0; i < Entry.size(); ++i)
	{
		if (false == Entry[i]->IsFaint())
		{
			ShiftPokemonIndex = i;
			break;
		}
	}

	ShiftPokemon();
}

void UBattler::ResetTemporalValues()
{
	StatStage.Reset();
	TempStatusId = EPokemonStatus::Normal;
	Participants.clear();
}

const FPokemonStatus* UBattler::CurStatus() const
{
	const UPokemon* Pokemon = CurPokemonReadonly();
	EPokemonStatus StatusId = Pokemon->GetStatusId();
	return UGameDB::FindStatus(StatusId);
}

void UBattler::DecBindCount()
{
	if (BindCount < 1)
	{
		return;
	}

	--BindCount;

	if (BindCount == 0)
	{
		TempStatusId = EPokemonStatus::Normal;
	}
}

bool UBattler::AllFaint() const
{
	for (const UPokemon* Pokemon : Entry)
	{
		if (Pokemon->GetStatusId() != EPokemonStatus::Faint)
		{
			return false;
		}
	}
	return true;
}

bool UBattler::IsFaint(int _Index) const
{
	if (_Index >= Entry.size())
	{
		MsgBoxAssert("UBattler::IsFaint 함수에서 엔트리 사이즈보다 큰 인덱스로 접근하려고 했습니다.");
		return false;
	}

	return Entry[_Index]->IsFaint();
}

void UBattler::InitEnemyPokemon()
{
	std::vector<UPokemon>* RealEntry = UEventManager::LoadEnemyEntry();

	if (nullptr == RealEntry)
	{
		MsgBoxAssert("적 포켓몬이 없는 상태로 배틀을 시작하려고 합니다.");
		return;
	}

	Entry.clear();
	for (int i = 0; i < RealEntry->size(); ++i)
	{
		Entry.push_back(&(RealEntry->at(i)));
	}
}
