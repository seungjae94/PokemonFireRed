#pragma once
#include "PokemonLevel.h"
#include "FadeLevelChanger.h"
#include "PokemonCanvas.h"

class UPokemonUILevel : public UPokemonLevel
{
private:
	enum class EState
	{
		TargetSelectionWait,		// 포켓몬 또는 취소 버튼 선택을 기다리는 상태
		ActionSelectionWait,		// 포켓몬을 선택하고 액션 선택을 기다리는 상태
		SwitchSelectionWait,		// 순서를 교체할 포켓몬을 결정하기를 기다리는 상태
		SwitchMoveOut,			
		SwitchMoveWait,
		SwitchMoveIn
	};

	enum class ESwitchMoveState
	{
		Out,
		Wait,
		In
	};

public:
	// constructor destructor
	UPokemonUILevel();
	~UPokemonUILevel();

	// delete Function
	UPokemonUILevel(const UPokemonUILevel& _Other) = delete;
	UPokemonUILevel(UPokemonUILevel&& _Other) noexcept = delete;
	UPokemonUILevel& operator=(const UPokemonUILevel& _Other) = delete;
	UPokemonUILevel& operator=(UPokemonUILevel&& _Other) noexcept = delete;

	UPokemon* GetTargetPokemon()
	{
		return &UPlayerData::GetPokemonInEntry(TargetCursor);
	}

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void LevelStart(ULevel* _PrevLevel) override;
private:
	std::string PrevLevelName;
	APokemonCanvas* Canvas = nullptr;

	// 고유 데이터
	const float SwitchMoveOutTime = 1 / 4.0f;
	const float SwitchMoveWaitTime = 1 / 5.0f;
	const float SwitchMoveInTime = 1 / 4.0f;
	int TargetCursor = 0;
	int SwitchFromCursor = 0;
	float Timer = 0.0f;

	// 고유 데이터 - 스위치
	AImageElement* SwitchFromBox = nullptr;
	AImageElement* SwitchToBox = nullptr;
	FVector SwitchFromInPos;
	FVector SwitchFromOutPos;
	FVector SwitchToInPos;
	FVector SwitchToOutPos;

	// 상태
	EState State = EState::TargetSelectionWait;

	// 상태 틱 함수
	void ProcessTargetSelectionWait();
	void ProcessActionSelectionWait();
	void ProcessSwitchSelectionWait();
	void ProcessSwitchMoveOut();
	void ProcessSwitchMoveWait();
	void ProcessSwitchMoveIn();

	// 상태 전이 함수
	void SelectTarget();
	void SelectAction();
	void SelectSwitch();

	// 유틸 함수
	bool IsTargetCursorOnFirst() const
	{
		return TargetCursor == 0;
	}

	bool IsTargetCursorOnEntry() const
	{
		return TargetCursor > 0 && TargetCursor < UPlayerData::GetPokemonEntrySize();
	}

	bool IsTargetCursorOnCancel() const
	{
		return TargetCursor == UPlayerData::GetPokemonEntrySize();
	}
};

