#pragma once
#include "PokemonUILevel.h"

class EMenuPokemonUIState : public EPokemonUIState
{
public:
	// �׼� ������ ��ٸ��� ����
	static const EMenuPokemonUIState ActionSelectionWait;

	// ������ �ٲ� ���ϸ��� �����ϱ⸦ ��ٸ��� ����
	static const EMenuPokemonUIState SwitchSelectionWait;

	// ��Ʈ�� �ڽ��� �ٱ����� ������ �ִ� ����
	static const EMenuPokemonUIState SwitchMoveOut;

	// ��Ʈ�� �ڽ��� �ٱ����� ������ �Ⱥ��̴� ���·� ������ �ִ� ����
	static const EMenuPokemonUIState SwitchMoveWait;

	// ��Ʈ�� �ڽ��� ������ ������ �ִ� ����
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

	// ���� ������
	std::string MapLevelName;
	float Timer = 0.0f;
	const float SwitchMoveOutTime = 1 / 4.0f;
	const float SwitchMoveWaitTime = 1 / 5.0f;
	const float SwitchMoveInTime = 1 / 4.0f;
	int SwitchFromCursor = 0;

	// ���� ������ - ����ġ
	AImageElement* SwitchFromBox = nullptr;
	AImageElement* SwitchToBox = nullptr;
	FVector SwitchFromInPos;
	FVector SwitchFromOutPos;
	FVector SwitchToInPos;
	FVector SwitchToOutPos;

	// ���� ƽ �Լ�
	void ProcessSwitchSelectionWait();
	void ProcessSwitchMoveOut();
	void ProcessSwitchMoveWait();
	void ProcessSwitchMoveIn();

	// ���� ���� �Լ�
	void SelectTarget() override;
	void CancelTargetSelection() override;
	void SelectAction() override;
	void SelectSwitch();
};

