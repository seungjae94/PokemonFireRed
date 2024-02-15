#pragma once
#include <functional>
#include <map>
#include "EventManager.h"
#include "EventCondition.h"

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

	// 프로세싱 함수
	/// <return>이벤트 명령 종료 여부</return>
	void ActivatePlayerControl();
	void DeactivatePlayerControl();
	bool ProcessMoveActor();
	void PostProcessMoveActor(AEventTarget* _Target);
	bool ProcessFadeIn();
	bool ProcessFadeOut();
	bool ProcessWait();
	bool ProcessChat();
	bool ProcessChangeLevel();
	bool ProcessChangePoint();
	bool ProcessChangeDirection();
	bool ProcessStarePlayer();

	// 프로세싱 변수 - 이동
	FTileVector MoveActorPrevPoint;
	FTileVector MoveActorNextPoint;
	int MoveActorPathIndex = -1;		// -1은 첫 번째 틱임을 의미한다.
	float MoveActorMoveTime = 0.0f;
	float MoveActorTimer = 0.0f;
};

