#pragma once
#include "BattleStateMachine.h"
#include "Battler.h"
#include "ExpCalculator.h"
#include "BattleExpGainStateMachine.h"

class ABattleCanvas;
class APokemonMsgBox;

class ABattleFaintStateMachine : public ABattleStateMachine
{
private:
	enum class ESubstate
	{
		None,
		TestFaint,
		HidePokemon,
		ShowFaintMessage1,
		ShowFaintMessage2,
		TestExpGain,
		ExpGain,
		End
	};
public:
	// constructor destructor
	ABattleFaintStateMachine();
	~ABattleFaintStateMachine();

	// delete Function
	ABattleFaintStateMachine(const ABattleFaintStateMachine& _Other) = delete;
	ABattleFaintStateMachine(ABattleFaintStateMachine&& _Other) noexcept = delete;
	ABattleFaintStateMachine& operator=(const ABattleFaintStateMachine& _Other) = delete;
	ABattleFaintStateMachine& operator=(ABattleFaintStateMachine&& _Other) noexcept = delete;

	bool IsEnd() const
	{
		return State == ESubstate::End;
	}

	void SetEGSM(ABattleExpGainStateMachine* _EGSM)
	{
		BattleExpGainSM = _EGSM;
	}

	void Start(UBattler* _Attacker, UBattler* _Defender, bool& _AttackerFaintChecked, bool& _DefenderFaintChecked);

protected:
private:
	void Tick(float _DeltaTime) override;

	void Start() override {};
	UBattler* Attacker = nullptr;
	UBattler* Defender = nullptr;

	// ���� ������
	ESubstate State = ESubstate::None;
	UBattler* Fainter = nullptr;
	const UPokemon* PlayerCurPokemon = nullptr;
	std::list<UBattler*> Fainters;
	const float FaintTime = 0.5f;
	float Timer = 0.0f;

	// ���� ������ - ����ġ ȹ�� ������
	FSimExpGainResult SimExpGainResult;
	std::map<UPokemon*, int> ExpGainByFaint;

	// ���� ƽ �Լ�
	void ProcessTestFaint();
	void ProcessHidePokemon();
	void ProcessShowFaintMessage1();
	void ProcessShowFaintMessage2();
	void ProcessTestExpGain();
	void ProcessExpGain();

	// SM - �� ���ϸ��� ����ġ ȹ�游�� ���
	ABattleExpGainStateMachine* BattleExpGainSM = nullptr;
};

