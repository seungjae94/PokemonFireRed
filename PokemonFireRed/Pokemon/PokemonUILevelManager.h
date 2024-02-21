#pragma once
#include <EngineCore/Actor.h>
#include "PokemonText.h"

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

	// ���� ó��
	EPokemonUIState State = EPokemonUIState::TargetSelectionWait;

	void TargetSelectionWaitTick(float _DeltaTime);
	std::string PrevMapLevelName;

	void ActionSelectionWaitTick(float _DeltaTime);
	void SwitchTick(float _DeltaTime);
};

