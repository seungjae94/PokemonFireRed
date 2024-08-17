#pragma once
#include <functional>
#include <map>
#include "EventManager.h"
#include "EventEnums.h"

class ASurprise;
class AEventTarget;
class AEventTrigger;
class UEventCondition;
class UEventStream;
enum class EEventType;
enum class EEventTriggerAction;

// 매 프레임마다 이벤트 액터 대신 이벤트를 처리해주는 객체
// 현재 레벨과 무관하게 틱이 돌아간다.
class UEventProcessor
{
	friend UEventManager;
private:
	enum class EMoveState
	{
		None,
		Move,
	};
public:
	// delete Function
	UEventProcessor(const UEventProcessor& _Other) = delete;
	UEventProcessor(UEventProcessor&& _Other) noexcept = delete;
	UEventProcessor& operator=(const UEventProcessor& _Other) = delete;
	UEventProcessor& operator=(UEventProcessor&& _Other) noexcept = delete;

	/// <summary>
	/// 이벤트 실행을 시도한다.
	/// 조건을 만족하는 이벤트 그룹이 없을 경우 false를 반환한다.
	static void Run(AEventTrigger* _Trigger, const UEventStream* _Stream);

	static bool IsRunning()
	{
		return IsRunningValue;
	}

protected:

private:
	inline static AEventTrigger* Trigger = nullptr;
	inline static bool IsRunningValue = false;
	inline static const UEventStream* Stream = nullptr;
	inline static int CurCommandIndex = 0;
	
	static void Tick(float _DeltaTime);
	static void EndRun();

	static std::string ToUpper(std::string_view _Name);

	// 프로세싱 함수
	/// <return>이벤트 명령 종료 여부</return>
	static void ActivatePlayerControl();
	static void DeactivatePlayerControl();
	static bool ProcessSetActive();
	static bool ProcessDestroy();
	static bool ProcessMove(float _DeltaTime);
	static bool ProcessMoveDynamicPath(float _DeltaTime);
	static bool SubprocessMoveStart(const std::vector<std::string>& _TargetNames, const std::vector<std::vector<FTileVector>>& _Paths, float _MoveSpeed);
	static bool SubprocessMove(const std::vector<std::string>& _TargetNames, const std::vector<std::vector<FTileVector>>& _Paths, float _MoveSpeed, bool _CameraFollow, float _DeltaTime);
	static bool ProcessMoveWithoutRestriction();
	static bool ProcessSurprise();
	static void PostProcessMoveWR(AEventTarget* _Target);
	static bool ProcessFadeIn();
	static bool ProcessFadeOut();
	static bool ProcessFadeInBgm();
	static bool ProcessFadeOutBgm();
	static bool ProcessWait();
	static bool ProcessPlayAnimation();
	static bool ProcessPlayBgm();
	static bool ProcessPlaySE();
	static bool ProcessChat();
	static bool ProcessPauseBgm();
	static bool ProcessResumeBgm();
	static bool ProcessShowMapName();
	static bool ProcessChangeArea();
	static bool ProcessChangeLevel();
	static bool ProcessChangePoint();
	static bool ProcessChangePosition();
	static bool ProcessChangeDirection();
	static bool ProcessStarePlayer();
	static bool ProcessHideActor();
	static bool ProcessShowActor();
	static bool ProcessCameraFocus();
	static bool ProcessWildBattle();
	static bool ProcessTrainerBattle();
	static bool ProcessAchieve();
	static bool ProcessUnachieve();
	static bool ProcessGainItem();
	static bool ProcessDeactivatePlayerControl();

	// 프로세싱 변수 - 공통
	inline static bool IsPlayerActivated = true;

	// 프로세싱 변수 - 이동
	inline static EMoveState MoveState = EMoveState::None;
	inline static std::vector<AEventTarget*> MoveTargets;
	inline static std::vector<FTileVector> MovePrevPoints;
	inline static std::vector<FTileVector> MoveNextPoints;
	inline static std::vector<bool> MoveEnds;
	inline static int MovePathIndex = 0;
	inline static float MoveTime = 0.0f;
	inline static float MoveTimer = 0.0f;

	// 프로세싱 변수 - 이동 (경로 동적 설정)
	inline static std::vector<std::vector<FTileVector>> DynamicPaths;

	// 프로세싱 변수 - 이동 (타일 제약 X)
	inline static FVector MoveWRPrevPos;
	inline static FVector MoveWRNextPos;
	inline static int MoveWRPathIndex = -1;
	inline static float MoveWRTime = 0.0f;
	inline static float MoveWRTimer = 0.0f;

	// 프로세싱 변수 - 애니메이션
	inline static bool PlayAnimIsFirstTick = true;

	// 프로세싱 변수 - Surprise
	inline static bool SurpriseFirstTick = true;
	inline static ASurprise* Surprise = nullptr;

private:
	// constructor destructor
	UEventProcessor();
	~UEventProcessor();
};

