#pragma once
#include <EngineCore/Actor.h>
#include "PokemonText.h"

enum class EPokemonUIState
{
	TargetSelectionWait,		// 포켓몬 또는 취소 버튼 선택을 기다리는 상태
	ActionSelectionWait,		// 포켓몬을 선택하고 액션 선택을 기다리는 상태
	Switch,						// 포켓몬 순서를 교체하고 있는 상태
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
	int Cursor = 0;
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

	// 상태 처리
	EPokemonUIState State = EPokemonUIState::TargetSelectionWait;

	void TargetSelectionWaitTick(float _DeltaTime);
	std::string PrevMapLevelName;

	void ActionSelectionWaitTick(float _DeltaTime);
	void SwitchTick(float _DeltaTime);
};

