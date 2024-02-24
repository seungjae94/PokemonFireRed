#pragma once
#include "Page.h"
#include "PokemonText.h"
#include "ScrollBar.h"
#include "PlayerData.h"

enum class EPokemonUIState
{
	TargetSelectionWait,		// ���ϸ� �Ǵ� ��� ��ư ������ ��ٸ��� ����
	ActionSelectionWait,		// ���ϸ��� �����ϰ� �׼� ������ ��ٸ��� ����
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

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
private:
	enum class ETargetImageState
	{
		Empty,
		Unfocused,
		Focused,
		From,
		To
	};

	UImageRenderer* BackgroundRenderer = nullptr;
	UImageRenderer* LongMsgBoxRenderer = nullptr;
	UImageRenderer* ShortMsgBoxRenderer = nullptr;
	UImageRenderer* ActionBoxRenderer = nullptr;
	ACursor* ActionCursor = nullptr;

	UImageRenderer* FirstRenderer = nullptr;
	UImageRenderer* FirstMiniPokemonRenderer = nullptr;
	APokemonText* FirstNameText = nullptr;
	APokemonText* FirstLevelText = nullptr;
	APokemonText* FirstHpText = nullptr;
	APokemonText* FirstCurHpText = nullptr;
	AScrollBar* FirstHpBar = nullptr;

	std::vector<UImageRenderer*> EntryRenderers;
	std::vector<UImageRenderer*> EntryMiniPokemonRenderers;
	std::vector<APokemonText*> EntryNameTexts;
	std::vector<APokemonText*> EntryLevelTexts;
	std::vector<APokemonText*> EntryHpTexts;
	std::vector<APokemonText*> EntryCurHpTexts;
	std::vector<AScrollBar*> EntryHpBars;

	UImageRenderer* CancelRenderer = nullptr;

	// ���� ó��
	EPokemonUIState State = EPokemonUIState::TargetSelectionWait;

	int TargetCursor = 0;
	int MemoryEntryCursor = 1;
	std::string PrevMapLevelName;
	void TargetSelectionWaitTick(float _DeltaTime);
	void MoveTargetCursor(int _Cursor);
	void TargetSelect();

	void DrawFirst(ETargetImageState _State);
	void DrawEntry(ETargetImageState _State, int _Index);
	void DrawCancel(ETargetImageState _State);

	void ActionSelectionWaitTick(float _DeltaTime);

	void SwitchTick(float _DeltaTime);

	// ���� �Լ�
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

