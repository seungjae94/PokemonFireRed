#include "Battler.h"
#include "PlayerData.h"
#include "EventManager.h"
#include "Trainer.h"

UBattler::UBattler()
{
}

UBattler::~UBattler()
{
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
	TrainerWinMessage = Trainer->GetTrainerWinMessage();
	PlayerWinMessage = Trainer->GetPlayerWinMessage();
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
		MsgBoxAssert("UBattler::IsFaint �Լ����� ��Ʈ�� ������� ū �ε����� �����Ϸ��� �߽��ϴ�.");
		return false;
	}

	return Entry[_Index]->IsFaint();
}

void UBattler::InitEnemyPokemon()
{
	std::vector<UPokemon>* RealEntry = UEventManager::LoadEnemyEntry();

	if (nullptr == RealEntry)
	{
		MsgBoxAssert("�� ���ϸ��� ���� ���·� ��Ʋ�� �����Ϸ��� �մϴ�.");
		return;
	}

	Entry.clear();
	for (int i = 0; i < RealEntry->size(); ++i)
	{
		Entry.push_back(&(RealEntry->at(i)));
	}
}
