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
	void InitPlayer();
	void InitWildPokemon();
	void InitTrainer(std::string_view _TrainerName, std::string_view _TrainerImageName);

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

	// ��Ʋ�� ����
	std::wstring GetTrainerNameW() const
	{
		return UEngineString::AnsiToUniCode(TrainerName);
	}

	std::string GetTrainerImageName() const
	{
		return TrainerImageName;
	}

	// ���ϸ� ����
	void InitCurPokemon();

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

	void ShiftPokemon();

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
	void DecBindCount();

	// ��Ʈ��
	int GetEntrySize() const
	{
		return static_cast<int>(Entry.size());
	}

	bool AllFaint() const;

protected:

private:
	// ��Ʋ�� ����
	std::vector<UPokemon*> Entry;
	std::string TrainerName;
	std::string TrainerImageName;

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

	void InitEnemyPokemon();
};

