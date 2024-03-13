#pragma once
#include "PokemonUILevel.h"

class EBattleShiftPokemonUIState : public EPokemonUIState
{
public:
	// 포켓몬이 기절해 있거나 이미 나와 있다는 메시지를 띄우고 있는 상태
	static const EBattleShiftPokemonUIState BattleShiftFailMessageShow;
};

class UBattleShiftPokemonUILevel : public UPokemonUILevel
{
public:
	// constructor destructor
	UBattleShiftPokemonUILevel();
	~UBattleShiftPokemonUILevel();

	// delete Function
	UBattleShiftPokemonUILevel(const UBattleShiftPokemonUILevel& _Other) = delete;
	UBattleShiftPokemonUILevel(UBattleShiftPokemonUILevel&& _Other) noexcept = delete;
	UBattleShiftPokemonUILevel& operator=(const UBattleShiftPokemonUILevel& _Other) = delete;
	UBattleShiftPokemonUILevel& operator=(UBattleShiftPokemonUILevel&& _Other) noexcept = delete;

	void SetPlayerBattler(UBattler* _Battler)
	{
		PlayerBattler = _Battler;
	}

protected:

private:
	void Tick(float _DeltaTime) override;
	void LevelStart(ULevel* _PrevLevel) override;

	// 고유 데이터
	float Timer = 0.0f;
	UBattler* PlayerBattler = nullptr;

	void ProcessBattleShiftFailMessageShow();

	// 상태 전이 함수
	void SelectTarget() override;
	void CancelTargetSelection() override;
	void SelectAction() override;
};

