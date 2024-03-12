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

	// 배틀러 정보
	void InitPlayer();
	void InitWildPokemon();
	void InitTrainer();
	bool IsPlayer() const;
	bool IsWildPokemon() const;
	bool IsTrainer() const;

	// 트레이너 정보
	std::wstring GetTrainerNameW() const;
	std::string GetTrainerImageName() const;
	std::wstring FrontPlayerWinMessage() const;
	void PopFrontPlayerWinMessage();
	int GetPlayerWinMessageSize() const;
	AEventTrigger* GetAfterBattleTrigger();

	// 포켓몬 정보
	void InitCurPokemon();
	UPokemon* CurPokemon();
	const UPokemon* CurPokemonReadonly() const;
	int CurPokemonIndex() const;
	void ShiftPokemon();
	int GetLevel(int _Index);
	int CurMovePP();
	void DecCurMovePP();

	// 액션
	EBattleAction CurAction() const;
	void SetAction(EBattleAction _Action);

	// 액션 디테일
	bool GetRunResult() const;
	void SetRunResult(bool _RunResult);
	EPokemonMove CurMoveId() const;
	const FPokemonMove* CurMove() const;
	void SetMoveIndex(int _MoveIndex);
	int GetShiftPokemonIndex() const;
	void SetShiftPokemonIndex(int _ShiftPokemonIndex);
	EItemSelectState GetItemSelectState() const;
	void SetItemSelectState(EItemSelectState _State);

	// 적 액션
	void EnemyAutoShift();

	// 스탯 변경
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

	// 스탯 변경 디테일
	void DecBindCount();

	// 엔트리
	int GetEntrySize() const
	{
		return static_cast<int>(Entry.size());
	}

	bool AllFaint() const;

	bool IsFaint(int _Index) const;

protected:

private:
	// 배틀러 정보
	std::vector<UPokemon*> Entry;
	int FightingPokemonIndex = 0;
	int CurMoveIndex = 0;
	bool IsPlayerValue = false;
	bool IsWildPokemonValue = false;

	// 트레이너 정보
	std::string TrainerName;
	std::string TrainerImageName;
	std::list<std::wstring> PlayerWinMessage;
	AEventTrigger* AfterBattleTrigger = nullptr;

	// 액션 선택 관련 변수
	EBattleAction Action = EBattleAction::None;
	bool RunResult = false;
	int ShiftPokemonIndex = -1;
	EItemSelectState ItemSelectState = EItemSelectState::None;

	// 스탯 관련 변수
	std::list<UPokemon*> Participants;
	EPokemonStatus TempStatusId = EPokemonStatus::Normal;
	int BindCount = 0;

	void InitEnemyPokemon();
};

