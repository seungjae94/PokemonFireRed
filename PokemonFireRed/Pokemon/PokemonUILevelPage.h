#pragma once
#include "Page.h"
#include "PlayerData.h"

enum class EPokemonUIState
{
	TargetSelectionWait,		// ���ϸ� �Ǵ� ��� ��ư ������ ��ٸ��� ����
	ActionSelectionWait,		// ���ϸ��� �����ϰ� �׼� ������ ��ٸ��� ����
	SwitchSelectionWait,		// ������ ��ü�� ���ϸ��� �����ϱ⸦ ��ٸ��� ����
	Switch,						// ���ϸ� ������ ��ü�ϰ� �ִ� ����
};

class UPokemonUILevel;

class UPokemonUILevelPage : public APage
{
public:
	// constructor destructor
	UPokemonUILevelPage();
	~UPokemonUILevelPage();

	// delete Function
	UPokemonUILevelPage(const UPokemonUILevelPage& _Other) = delete;
	UPokemonUILevelPage(UPokemonUILevelPage&& _Other) noexcept = delete;
	UPokemonUILevelPage& operator=(const UPokemonUILevelPage& _Other) = delete;
	UPokemonUILevelPage& operator=(UPokemonUILevelPage&& _Other) noexcept = delete;

	void SetPrevMapLevelName(std::string_view _Name)
	{
		PrevMapLevelName = _Name;
	}

	// Ŀ�� ���� �ʱ�ȭ
	void Reset()
	{
		TargetCursor = 0;
		MemoryEntryCursor = 1;
		ActionCursor->SetCursor(0);
		RefreshAllTargets();
	}

	UPokemon* GetTargetPokemon()
	{
		if (true == IsCancel(TargetCursor))
		{
			return nullptr;
		}

		return &UPlayerData::GetPokemonInEntry(TargetCursor);
	}

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
private:
	enum class ETargetState
	{
		Empty,
		Unfocused,
		Focused,
		From,
		To
	};

	enum class ESwitchMoveState
	{
		Out,
		Wait,
		In
	};

	UImageRenderer* BackgroundRenderer = nullptr;
	UImageRenderer* TargetSelectionMsgBoxRenderer = nullptr;
	UImageRenderer* ActionSelectionMsgBoxRenderer = nullptr;
	UImageRenderer* SwitchSelectionMsgBoxRenderer = nullptr;
	UImageRenderer* ActionBoxRenderer = nullptr;
	ACursor* ActionCursor = nullptr;

	UImageRenderer* FirstRenderer = nullptr;
	APokemonElement* FirstPokemonIcon = nullptr;
	AText* FirstNameText = nullptr;
	AText* FirstLevelText = nullptr;
	AText* FirstHpText = nullptr;
	AText* FirstCurHpText = nullptr;
	AScrollBar* FirstHpBar = nullptr;
	AImageElement* FirstGender = nullptr;

	std::vector<UImageRenderer*> EntryRenderers;
	std::vector<APokemonElement*> EntryPokemonIcons;
	std::vector<AText*> EntryNameTexts;
	std::vector<AText*> EntryLevelTexts;
	std::vector<AText*> EntryHpTexts;
	std::vector<AText*> EntryCurHpTexts;
	std::vector<AScrollBar*> EntryHpBars;
	std::vector<AImageElement*> EntryGenders;

	UImageRenderer* CancelRenderer = nullptr;

	// ���� ó��
	EPokemonUIState State = EPokemonUIState::TargetSelectionWait;

	void TargetSelectionWaitTick(float _DeltaTime);
	int TargetCursor = 0;
	int MemoryEntryCursor = 1;
	std::string PrevMapLevelName;
	void TargetSelect();

	void ActionSelectionWaitTick(float _DeltaTime);
	void ActionSelect();

	void SwitchSelectionWaitStart();
	void SwitchSelectionWaitTick(float _DeltaTime);
	int SwitchFromCursor = 0; // TargetCursor�� SwitchToCursor ������ ����. 
	void SwitchSelect();

	void SwitchStart();
	void SwitchTick(float _DeltaTime);
	void SwitchMoveOutTick(float _DeltaTime);
	void SwitchMoveWaitTick(float _DeltaTime);
	void SwitchMoveInTick(float _DeltaTime);
	ESwitchMoveState SwitchMoveState = ESwitchMoveState::Out;
	float SwitchMoveOutTime = 1 / 4.0f;
	float SwitchMoveWaitTime = 1 / 5.0f;
	float SwitchMoveInTime = 1 / 4.0f;
	float SwitchMoveTimer = 0.0f;
	FVector SwitchFromPrevPos;
	FVector SwitchFromOutPos;
	UImageRenderer* SwitchMoveSwitchFromRenderer = nullptr;
	FVector TargetPrevPos;
	FVector TargetOutPos;
	UImageRenderer* SwitchMoveTargetRenderer = nullptr;

	// Refresh �Լ�
	void RefreshFirst();
	void RefreshEntry(int _Index);
	void RefreshCancel();
	void RefreshAllTargets();

	// ���� �Լ�
	void MoveTargetCursor(int _Cursor);

	bool IsFirst(int _Cursor)
	{
		return _Cursor == 0;
	}

	bool IsEntry(int _Cursor)
	{
		return _Cursor > 0 && _Cursor < UPlayerData::GetPokemonEntrySize();
	}

	bool IsCancel(int _Cursor)
	{
		return _Cursor == UPlayerData::GetPokemonEntrySize();
	}
};

