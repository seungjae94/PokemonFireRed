#pragma once
#include "MapLevel.h"
#include "EndingCanvas.h"

class UInteriorPewterGymLevel : public UMapLevel
{
private:
	enum class EState
	{
		None,
		Ending,
	};
public:
	// constructor destructor
	UInteriorPewterGymLevel();
	~UInteriorPewterGymLevel();

	// delete Function
	UInteriorPewterGymLevel(const UInteriorPewterGymLevel& _Other) = delete;
	UInteriorPewterGymLevel(UInteriorPewterGymLevel&& _Other) noexcept = delete;
	UInteriorPewterGymLevel& operator=(const UInteriorPewterGymLevel& _Other) = delete;
	UInteriorPewterGymLevel& operator=(UInteriorPewterGymLevel&& _Other) noexcept = delete;

protected:

private:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void MakePewterCityDoor();
	void MakeCamper();
	void MakeGymLeader();

	static bool FightWithCamperChecker();
	static bool FightWithBrockChecker();
	static std::vector<FTileVector> CamperPathGenerator();

	// 엔딩 이벤트 구현 - 상태
	EState State = EState::None;
	AEndingCanvas* Canvas = nullptr;

	void ProcessEnding();
};

