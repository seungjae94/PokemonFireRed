#pragma once
#include <vector>
#include "Pokemon.h"
#include "BattleEnums.h"
#include "StatStage.h"

class UPlayerData;
class AEventTrigger;

enum class EItemSelectState
{
	None,
	Canceled,
	ItemUsed,
	BallSelected,
};

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

	void Clear();

	// ��Ʋ�� ����
	void InitPlayer();
	void InitWildPokemon();
	void InitTrainer();
	bool IsPlayer() const;
	bool IsWildPokemon() const;
	bool IsTrainer() const;

	// Ʈ���̳� ����
	std::wstring GetTrainerNameW() const;
	std::string GetTrainerImageName() const;
	std::wstring FrontPlayerWinMessage() const;
	void PopFrontPlayerWinMessage();
	int GetPlayerWinMessageSize() const;
	AEventTrigger* GetAfterBattleTrigger();
	bool IsGymLeader() const;
	void SetGymLeader(bool _GymLeader);

	// ���ϸ� ����
	void InitCurPokemon();
	UPokemon* CurPokemon();
	const UPokemon* CurPokemonReadonly() const;
	int CurPokemonIndex() const;
	void ShiftPokemon();
	int GetLevel(int _Index);
	int CurMovePP();
	void DecCurMovePP();

	// �׼�
	EBattleAction CurAction() const;
	void SetAction(EBattleAction _Action);

	// �׼� ������
	bool GetRunResult() const;
	void SetRunResult(bool _RunResult);
	EPokemonMove CurMoveId() const;
	const FPokemonMove* CurMove() const;
	void SetMoveIndex(int _MoveIndex);
	int GetShiftPokemonIndex() const;
	void SetShiftPokemonIndex(int _ShiftPokemonIndex);
	EItemSelectState GetItemSelectState() const;
	void SetItemSelectState(EItemSelectState _State);
	void SelectItem(const FItem* _Item);
	bool GetCatchResult() const;
	void SetCatchResult(bool _Result);

	// �� �׼�
	void EnemyAutoShift();

	// ���� ����
	void ResetTemporalValues();

	const FPokemonStatus* CurStatus() const;

	EPokemonStatus CurStatusId() const
	{
		return CurPokemonReadonly()->GetStatusId();
	}

	const FPokemonStatus* CurTempStatus() const
	{
		return UGameDB::FindStatus(TempStatusId);
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

	bool IsFaint(int _Index) const;

protected:

private:
	// ��Ʋ�� ����
	std::vector<UPokemon*> Entry;
	int FightingPokemonIndex = 0;
	int CurMoveIndex = 0;
	bool IsPlayerValue = false;
	bool IsWildPokemonValue = false;

	// Ʈ���̳� ����
	std::string TrainerName;
	std::string TrainerImageName;
	std::list<std::wstring> PlayerWinMessage;
	AEventTrigger* AfterBattleTrigger = nullptr;
	bool GymLeader = false;

	// �׼� ���� ���� ����
	EBattleAction Action = EBattleAction::None;
	bool RunResult = false;
	int ShiftPokemonIndex = -1;
	const FItem* SelectedItem = nullptr;
	EItemSelectState ItemSelectState = EItemSelectState::None;
	bool CatchResult = false;

	// ���� ���� ����
	std::list<UPokemon*> Participants;
	EPokemonStatus TempStatusId = EPokemonStatus::Normal;
	int BindCount = 0;

	void InitEnemyPokemon();
};

