#pragma once
#include "PokemonUILevel.h"

class EBattleShiftPokemonUIState : public EPokemonUIState
{
public:
	// ���ϸ��� ������ �ְų� �̹� ���� �ִٴ� �޽����� ���� �ִ� ����
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

	// ���� ������
	float Timer = 0.0f;
	UBattler* PlayerBattler = nullptr;

	void ProcessBattleShiftFailMessageShow();

	// ���� ���� �Լ�
	void SelectTarget() override;
	void CancelTargetSelection() override;
	void SelectAction() override;
};

