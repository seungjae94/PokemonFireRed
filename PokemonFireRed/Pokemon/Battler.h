#pragma once
#include <vector>
#include "Pokemon.h"
#include "BattleEnums.h"
#include "StatStage.h"

class UPlayerData;

class UBattler
{
public:
	// constructor destructor
	UBattler();
	~UBattler();

	// delete Function
	UBattler(const UBattler& _Other) = delete;
	UBattler(UBattler&& _Other) noexcept = delete;
	UBattler& operator=(const UBattler& _Other) = delete;
	UBattler& operator=(UBattler&& _Other) noexcept = delete;

	UStatStage StatStage;

	void Clear()
	{
		StatStage.Reset();
		Entry.clear();
		Action = EBattleAction::None;
		FightingPokemonIndex = 0;
		CurMoveIndex = 0;
		IsPlayerValue = false;
		IsWildPokemonValue = false;
	}

	// ��Ʋ�� ����
	void SetPlayer();
	void SetWildPokemon();

	bool IsPlayer() const
	{
		return IsPlayerValue;
	}

	bool IsWildPokemon() const
	{
		return IsWildPokemonValue;
	}

	bool IsTrainer() const
	{
		return false == IsPlayer() && false == IsWildPokemon();
	}

	// ���ϸ� ����
	void InitCurPokemon()
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

	UPokemon* CurPokemon()
	{
		return Entry[FightingPokemonIndex];
	}

	const UPokemon* CurPokemonReadonly() const
	{
		return Entry[FightingPokemonIndex];
	}

	int CurPokemonIndex() const
	{
		return FightingPokemonIndex;
	}

	void ShiftPokemon()
	{
		StatStage.Reset();
		FightingPokemonIndex = ShiftPokemonIndex;
		CurMoveIndex = 0;
		ShiftPokemonIndex = -1;
	}

	// �׼�
	EBattleAction CurAction() const
	{
		return Action;
	}

	void SetAction(EBattleAction _Action)
	{
		Action = _Action;
	}

	// �׼� ������
	bool GetRunResult() const
	{
		return RunResult;
	}

	void SetRunResult(bool _RunResult)
	{
		RunResult = _RunResult;
	}

	EPokemonMove CurMoveId() const
	{
		return CurPokemonReadonly()->GetMoveId(CurMoveIndex);
	}

	const FPokemonMove* CurMove() const
	{
		EPokemonMove MoveId = CurMoveId();
		return UPokemonDB::FindMove(MoveId);
	}

	void SetMoveIndex(int _MoveIndex)
	{
		CurMoveIndex = _MoveIndex;
	}

	int GetShiftPokemonIndex() const
	{
		return ShiftPokemonIndex;
	}

	void SetShiftPokemonIndex(int _ShiftPokemonIndex)
	{
		ShiftPokemonIndex = _ShiftPokemonIndex;
	}

	// ���� ����
	void ResetTemporalStat()
	{
		StatStage.Reset();
		Participants.clear();
	}

	const FPokemonStatus* CurStatus() const
	{
		const UPokemon* Pokemon = CurPokemonReadonly();
		EPokemonStatus StatusId = Pokemon->GetStatusId();
		return UPokemonDB::FindStatus(StatusId);
	}

	EPokemonStatus CurStatusId() const
	{
		return CurPokemonReadonly()->GetStatusId();
	}

	const FPokemonStatus* CurTempStatus() const
	{
		return UPokemonDB::FindStatus(TempStatusId);
	}

	EPokemonStatus CurTempStatusId() const
	{
		return TempStatusId;
	}

	void SetTempStatus(EPokemonStatus _TempStatus)
	{
		TempStatusId = _TempStatus;
	}

	std::list<UPokemon*>& GetParticipants()
	{
		return Participants;
	}

	const std::list<UPokemon*>& GetParticipantsReadonly() const
	{
		return Participants;
	}

	// ���� ���� ������
	void DecBindCount()
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

	// ��Ʈ��
	int GetEntrySize() const
	{
		return static_cast<int>(Entry.size());
	}

	bool AllFaint() const
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

protected:

private:
	// ��Ʋ�� ����
	std::vector<UPokemon*> Entry;
	int FightingPokemonIndex = 0;
	int CurMoveIndex = 0;
	bool IsPlayerValue = false;
	bool IsWildPokemonValue = false;

	// �׼� ���� ���� ����
	EBattleAction Action = EBattleAction::None;
	bool RunResult = false;
	int ShiftPokemonIndex = -1;

	// ���� ���� ����
	std::list<UPokemon*> Participants;
	EPokemonStatus TempStatusId = EPokemonStatus::Normal;
	int BindCount = 0;
};

