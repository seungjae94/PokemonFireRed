#pragma once
#include <functional>
#include <map>
#include "EventManager.h"
#include "EventCondition.h"

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
	// constructor destructor
	UEventProcessor();
	~UEventProcessor();

	// delete Function
	UEventProcessor(const UEventProcessor& _Other) = delete;
	UEventProcessor(UEventProcessor&& _Other) noexcept = delete;
	UEventProcessor& operator=(const UEventProcessor& _Other) = delete;
	UEventProcessor& operator=(UEventProcessor&& _Other) noexcept = delete;

	void RegisterStream(const UEventCondition& _Condition, UEventStream _Stream);
	void UnregisterStream(const UEventCondition& _Condition);
	
	/// <summary>
	/// 이벤트 실행을 시도한다.
	/// 조건을 만족하는 이벤트 그룹이 없을 경우 false를 반환한다.
	bool TryRun(EEventTriggerAction _TriggerAction);

	bool IsRunning() const
	{
		return IsRunningValue;
	}

protected:

private:
	AEventTrigger* Trigger = nullptr;
	
	bool IsRunningValue = false;

	int CurCommandIndex = 0;
	std::map<EEventType, int> CurIndexOfTypeMap;
	UEventStream* CurStream = nullptr;

	std::map<UEventCondition, UEventStream> AllStreams;

	void Tick(float _DeltaTime);
	void EndRun();

	// 편의 함수
	int GetCurIndexOfType(EEventType _Type)
	{
		if (false == CurIndexOfTypeMap.contains(_Type))
		{
			CurIndexOfTypeMap[_Type] = 0;
		}

		return CurIndexOfTypeMap[_Type];
	}

	void IncCurIndexOfType(EEventType _Type)
	{
		int CurIndexOfType = GetCurIndexOfType(_Type);
		CurIndexOfTypeMap[_Type] = CurIndexOfType + 1;
	}

	std::string ToUpper(std::string_view _Name);

	// 프로세싱 함수
	/// <return>이벤트 명령 종료 여부</return>
	void ActivatePlayerControl();
	void DeactivatePlayerControl();
	bool ProcessSetActive();
	bool ProcessDestroy();
	bool ProcessMove(float _DeltaTime);
	bool ProcessMoveDynamicPath(float _DeltaTime);
	bool SubprocessMoveStart(const std::vector<std::string>& _TargetNames, const std::vector<std::vector<FTileVector>>& _Paths, float _MoveSpeed);
	bool SubprocessMove(const std::vector<std::string>& _TargetNames, const std::vector<std::vector<FTileVector>>& _Paths, float _MoveSpeed, bool _CameraFollow, float _DeltaTime);
	bool ProcessMoveWithoutRestriction();
	bool ProcessSurprise();
	void PostProcessMoveWR(AEventTarget* _Target);
	bool ProcessFadeIn();
	bool ProcessFadeOut();
	bool ProcessFadeInBgm();
	bool ProcessFadeOutBgm();
	bool ProcessWait();
	bool ProcessPlayAnimation();
	bool ProcessPlayBgm();
	bool ProcessPlaySE();
	bool ProcessChat();
	bool ProcessShowMapName();
	bool ProcessChangeLevel();
	bool ProcessChangePoint();
	bool ProcessChangePosition();
	bool ProcessChangeDirection();
	bool ProcessStarePlayer();
	bool ProcessHideActor();
	bool ProcessShowActor();
	bool ProcessCameraFocus();
	bool ProcessWildBattle();
	bool ProcessTrainerBattle();
	bool ProcessAchieve();
	bool ProcessUnachieve();
	bool ProcessGainItem();
	bool ProcessDeactivatePlayerControl();

	// 프로세싱 변수 - 공통
	bool IsPlayerActivated = true;

	// 프로세싱 변수 - 이동
	EMoveState MoveState = EMoveState::None;
	std::vector<AEventTarget*> MoveTargets;
	std::vector<FTileVector> MovePrevPoints;
	std::vector<FTileVector> MoveNextPoints;
	std::vector<bool> MoveEnds;
	int MovePathIndex = 0;
	float MoveTime = 0.0f;
	float MoveTimer = 0.0f;

	// 프로세싱 변수 - 이동 (경로 동적 설정)
	std::vector<std::vector<FTileVector>> DynamicPaths;

	// 프로세싱 변수 - 이동 (타일 제약 X)
	FVector MoveWRPrevPos;
	FVector MoveWRNextPos;
	int MoveWRPathIndex = -1;
	float MoveWRTime = 0.0f;
	float MoveWRTimer = 0.0f;

	// 프로세싱 변수 - 애니메이션
	bool PlayAnimIsFirstTick = true;

	// 프로세싱 변수 - Surprise
	bool SurpriseFirstTick = true;
	ASurprise* Surprise = nullptr;
};

