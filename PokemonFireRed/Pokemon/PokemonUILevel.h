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
		Pokemon,		// �޴�â�� ���� ���ϸ� UI ������ ���� ���
		Bag,			// ���� �������� ������ ��� ����� ���� ���� ���ϸ� UI ������ ���� ���
		BattleShift,	// ��Ʋ �������� Pokemon �׼��� �����ؼ� ���ϸ� UI ������ ���� ���
		BattleItem		// ��Ʋ �������� Item �׼��� �����ؼ� ���� ������ ������ �� �ٽ� ������ ��� ����� ���� ���� ���ϸ� UI ������ ���� ���
	};

	enum class EState
	{
		TargetSelectionWait,				// ���ϸ� �Ǵ� ��� ��ư ������ ��ٸ��� ����
		ActionSelectionWait,				// ���ϸ��� �����ϰ� �׼� ������ ��ٸ��� ����
		BattleShiftActionSelectionWait,		// (BattleShift ���) ���ϸ��� �����ϰ� �׼� ������ ��ٸ��� ����
		BattleShiftFailMessageShow,
		SwitchSelectionWait,				// ������ �ٲ� ���ϸ��� �����ϱ⸦ ��ٸ��� ����
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

	// ���� ������
	const float SwitchMoveOutTime = 1 / 4.0f;
	const float SwitchMoveWaitTime = 1 / 5.0f;
	const float SwitchMoveInTime = 1 / 4.0f;
	int TargetCursor = 0;
	int SwitchFromCursor = 0;
	float Timer = 0.0f;

	// ���� ������ - ����ġ
	AImageElement* SwitchFromBox = nullptr;
	AImageElement* SwitchToBox = nullptr;
	FVector SwitchFromInPos;
	FVector SwitchFromOutPos;
	FVector SwitchToInPos;
	FVector SwitchToOutPos;

	// ���� ������ - ��Ʋ ����Ʈ
	UBattler* PlayerBattler = nullptr;
	
	// ���� ������ - ����
	const FItem* UseItem = nullptr;


	// ����
	EState State = EState::TargetSelectionWait;
	EMode Mode = EMode::Pokemon;

	// ���� ƽ �Լ�
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

	// ���� ���� �Լ�
	void SelectTarget();
	void SelectAction();
	void SelectBattleAction();
	void SelectSwitch();

	// ��ƿ �Լ�
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

