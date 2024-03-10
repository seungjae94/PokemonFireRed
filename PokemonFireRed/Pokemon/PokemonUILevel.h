#pragma once
#include "PokemonLevel.h"
#include "FadeLevelChanger.h"
#include "PokemonCanvas.h"
#include "Battler.h"

class UBattleLevel;

class UPokemonUILevel : public UPokemonLevel
{
private:
	enum class EMode
	{
		Pokemon,		// 메뉴창을 통해 포켓몬 UI 레벨로 들어온 경우
		Bag,			// 가방 레벨에서 아이템 사용 대상을 고르기 위해 포켓몬 UI 레벨로 들어온 경우
		BattleShift,	// 배틀 레벨에서 Pokemon 액션을 선택해서 포켓몬 UI 레벨로 들어온 경우
		BattleItem		// 배틀 레벨에서 Item 액션을 선택해서 가방 레벨로 진입한 뒤 다시 아이템 사용 대상을 고르기 위해 포켓몬 UI 레벨로 들어온 경우
	};

	enum class EState
	{
		TargetSelectionWait,				// 포켓몬 또는 취소 버튼 선택을 기다리는 상태
		ActionSelectionWait,				// 포켓몬을 선택하고 액션 선택을 기다리는 상태
		BattleShiftActionSelectionWait,		// (BattleShift 모드) 포켓몬을 선택하고 액션 선택을 기다리는 상태
		BattleShiftFailMessageShow,
		SwitchSelectionWait,				// 순서를 바꿀 포켓몬을 결정하기를 기다리는 상태
		SwitchMoveOut,			
		SwitchMoveWait,
		SwitchMoveIn,
		BagTestItemUseEffect,
		BagItemUseEffect,
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

	// 고유 데이터 - 배틀 시프트
	UBattler* PlayerBattler = nullptr;
	
	// 고유 데이터 - 가방
	const FItem* UseItem = nullptr;


	// 상태
	EState State = EState::TargetSelectionWait;
	EMode Mode = EMode::Pokemon;

	// 상태 틱 함수
	void ProcessTargetSelectionWait();
	void ProcessActionSelectionWait();
	void ProcessBattleShiftActionSelectionWait();
	void ProcessBattleShiftFailMessageShow();
	void ProcessSwitchSelectionWait();
	void ProcessSwitchMoveOut();
	void ProcessSwitchMoveWait();
	void ProcessSwitchMoveIn();
	void ProcessBagTestItemUseEffect();
	void ProcessBagItemUseEffect();

	// 상태 전이 함수
	void SelectTarget();
	void SelectAction();
	void SelectBattleAction();
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

	void CancelTargetSelection();
};

