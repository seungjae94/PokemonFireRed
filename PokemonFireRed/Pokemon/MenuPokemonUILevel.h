#pragma once
#include "PokemonUILevel.h"

class EMenuPokemonUIState : public EPokemonUIState
{
public:
	// 액션 선택을 기다리는 상태
	static const EMenuPokemonUIState ActionSelectionWait;

	// 순서를 바꿀 포켓몬을 결정하기를 기다리는 상태
	static const EMenuPokemonUIState SwitchSelectionWait;

	// 엔트리 박스가 바깥으로 빠지고 있는 상태
	static const EMenuPokemonUIState SwitchMoveOut;

	// 엔트리 박스가 바깥으로 빠져서 안보이는 상태로 정지해 있는 상태
	static const EMenuPokemonUIState SwitchMoveWait;

	// 엔트리 박스가 안으로 들어오고 있는 상태
	static const EMenuPokemonUIState SwitchMoveIn;
};

class UMenuPokemonUILevel : public UPokemonUILevel
{
public:
	// constructor destructor
	UMenuPokemonUILevel();
	~UMenuPokemonUILevel();

	// delete Function
	UMenuPokemonUILevel(const UMenuPokemonUILevel& _Other) = delete;
	UMenuPokemonUILevel(UMenuPokemonUILevel&& _Other) noexcept = delete;
	UMenuPokemonUILevel& operator=(const UMenuPokemonUILevel& _Other) = delete;
	UMenuPokemonUILevel& operator=(UMenuPokemonUILevel&& _Other) noexcept = delete;

protected:

private:
	void Tick(float _DeltaTime) override;
	void LevelStart(ULevel* _PrevLevel) override;

	// 고유 데이터
	std::string MapLevelName;
	float Timer = 0.0f;
	const float SwitchMoveOutTime = 1 / 4.0f;
	const float SwitchMoveWaitTime = 1 / 5.0f;
	const float SwitchMoveInTime = 1 / 4.0f;
	int SwitchFromCursor = 0;

	// 고유 데이터 - 스위치
	AImageElement* SwitchFromBox = nullptr;
	AImageElement* SwitchToBox = nullptr;
	FVector SwitchFromInPos;
	FVector SwitchFromOutPos;
	FVector SwitchToInPos;
	FVector SwitchToOutPos;

	// 상태 틱 함수
	void ProcessSwitchSelectionWait();
	void ProcessSwitchMoveOut();
	void ProcessSwitchMoveWait();
	void ProcessSwitchMoveIn();

	// 상태 전이 함수
	void SelectTarget() override;
	void CancelTargetSelection() override;
	void SelectAction() override;
	void SelectSwitch();
};

