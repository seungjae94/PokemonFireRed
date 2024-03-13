#pragma once
#include "PokemonLevel.h"
#include "FadeLevelChanger.h"
#include "PokemonCanvas.h"
#include "Battler.h"

class UBattleLevel;
class UBagUILevel;

// 상속이 가능한 Enum 구현
class EPokemonUIState
{
public:
	static const EPokemonUIState None;
	static const EPokemonUIState TargetSelectionWait;
	static const EPokemonUIState ActionSelectionWait;

	void operator=(const EPokemonUIState& _Other);
	bool operator==(const EPokemonUIState& _Other);
	bool operator<(const EPokemonUIState& _Other);
	bool operator>(const EPokemonUIState& _Other);
protected:
	EPokemonUIState() : Index(MaxIndex++) {}
	int Index;
	static int MaxIndex;
};

// 타겟 선택, 캔버스 초기화, 리소스 로드를 담당
class UPokemonUILevel : public UPokemonLevel
{
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
	void LevelStart(ULevel* _PrevLevel) override;
	
	APokemonCanvas* Canvas = nullptr;
	EPokemonUIState State = EPokemonUIState::None;
	int TargetCursor = 0;
	
	// 상태 틱 함수
	void ProcessTargetSelectionWait();
	void ProcessActionSelectionWait();

	// 상태 전이 함수
	virtual void SelectTarget() {};
	virtual void CancelTargetSelection() {};
	virtual void SelectAction() {};

	// 유틸 함수
	bool IsTargetCursorOnFirst() const;
	bool IsTargetCursorOnEntry() const;
	bool IsTargetCursorOnCancel() const;
private:
	static bool IsCommonResourcesLoaded;
};

