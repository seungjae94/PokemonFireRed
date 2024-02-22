#pragma once
#include <EngineCore/Actor.h>
#include "PokemonText.h"
#include "PlayerData.h"

enum class EPokemonUIState
{
	TargetSelectionWait,		// ���ϸ� �Ǵ� ��� ��ư ������ ��ٸ��� ����
	ActionSelectionWait,		// ���ϸ��� �����ϰ� �׼� ������ ��ٸ��� ����
	Switch,						// ���ϸ� ������ ��ü�ϰ� �ִ� ����
};

class UPokemonUILevelManager : public AActor
{
public:
	// constructor destructor
	UPokemonUILevelManager();
	~UPokemonUILevelManager();

	// delete Function
	UPokemonUILevelManager(const UPokemonUILevelManager& _Other) = delete;
	UPokemonUILevelManager(UPokemonUILevelManager&& _Other) noexcept = delete;
	UPokemonUILevelManager& operator=(const UPokemonUILevelManager& _Other) = delete;
	UPokemonUILevelManager& operator=(UPokemonUILevelManager&& _Other) noexcept = delete;

	void SetPrevMapLevelName(std::string_view _Name)
	{
		PrevMapLevelName = _Name;
	}

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
private:
	int TargetCursor = 0;
	int MemoryEntryCursor = 1;
	UImageRenderer* BackgroundRenderer = nullptr;

	UImageRenderer* FirstRenderer = nullptr;
	APokemonText* FirstNameText = nullptr;
	APokemonText* FirstLevelText = nullptr;
	APokemonText* FirstHpText = nullptr;
	APokemonText* FirstCurHpText = nullptr;

	std::vector<UImageRenderer*> EntryRenderers;
	std::vector<APokemonText*> EntryNameTexts;
	std::vector<APokemonText*> EntryLevelTexts;
	std::vector<APokemonText*> EntryHpTexts;
	std::vector<APokemonText*> EntryCurHpTexts;

	UImageRenderer* CancelRenderer = nullptr;

	// ���� ó��
	EPokemonUIState State = EPokemonUIState::TargetSelectionWait;

	void TargetSelectionWaitTick(float _DeltaTime);
	std::string PrevMapLevelName;
	void MoveTargetCursor(int _Cursor);

	void ActionSelectionWaitTick(float _DeltaTime);
	void SwitchTick(float _DeltaTime);

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

