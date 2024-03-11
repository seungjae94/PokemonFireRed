#pragma once
#include "MapLevel.h"

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
	//void SetDoorTargetAsPewterCity();

	// ╩Себ
	EState State = EState::CheckHealEvent;
	APokemonMsgBox* MsgBox = nullptr;
	APokemonCenterCanvas* Canvas = nullptr;

	// ╩Себ ф╫
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

};

