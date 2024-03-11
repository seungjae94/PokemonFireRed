#pragma once
#include "MapLevel.h"
#include "PokemonCenterBalls.h"

class AInteriorDoor;
class APokemonMsgBox;
class APokemonCenterCanvas;

class UInteriorPokemonCenterLevel : public UMapLevel
{
private:
	enum class EState
	{
		CheckHealEvent,
		WelcomeMessage1,
		WelcomeMessage2,
		HealSelectMessage,
		HealSelect,
		NoHealMessage,
		YesHealMessage,
		BallAppear,
		BallAnim,
		HealEndMessage1,
		HealEndMessage2,
		HealEndMessage3,
		HealEndMessage4,
		HealEndMessage5,
	};
public:
	// constructor destructor
	UInteriorPokemonCenterLevel();
	~UInteriorPokemonCenterLevel();

	// delete Function
	UInteriorPokemonCenterLevel(const UInteriorPokemonCenterLevel& _Other) = delete;
	UInteriorPokemonCenterLevel(UInteriorPokemonCenterLevel&& _Other) noexcept = delete;
	UInteriorPokemonCenterLevel& operator=(const UInteriorPokemonCenterLevel& _Other) = delete;
	UInteriorPokemonCenterLevel& operator=(UInteriorPokemonCenterLevel&& _Other) noexcept = delete;

protected:

private:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void LevelStart(ULevel* _PrevLevel) override;

	AInteriorDoor* Door = nullptr;
	void MakeDoor();
	void MakeNurse();

	void SetDoorTargetAsViridianCity();
	void SetDoorTargetAsPewterCity();

	// 상태
	EState State = EState::CheckHealEvent;
	APokemonMsgBox* MsgBox = nullptr;
	APokemonCenterCanvas* Canvas = nullptr;

	// 상태 틱
	void ProcessCheckHealEvent();
	void ProcessWelcomeMessage1();
	void ProcessWelcomeMessage2();
	void ProcessHealSelectMessage();
	void ProcessHealSelect();
	void ProcessNoHealMessage();
	void ProcessYesHealMessage();
	void ProcessBallAppear();
	void ProcessBallAnim();
	void ProcessHealEndMessage1();
	void ProcessHealEndMessage2();
	void ProcessHealEndMessage3();
	void ProcessHealEndMessage4();
	void ProcessHealEndMessage5();

	// 상태 데이터
	float Timer = 0.0f;
	const float BallAppearInterval = 0.15f;
	const float BallBlinkTime = 1.8f;
	int MaxBallCount = 0;
	int CurBallCount = 0;
	AEventTarget* Nurse = nullptr;
	APokemonCenterBalls* Balls = nullptr;
};

