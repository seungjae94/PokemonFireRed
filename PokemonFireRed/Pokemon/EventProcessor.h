#pragma once
#include <functional>
#include <map>
#include "EventManager.h"
#include "EventEnums.h"
#include "EventCommand_ProcessingData.h"

class ASurprise;
class AEventTarget;
class AEventTrigger;
class UEventCondition;
class UEventStream;
class UEventCommand;
enum class EEventType;
enum class EEventTriggerAction;

// 매 프레임마다 이벤트 액터 대신 이벤트를 처리해주는 객체
// 현재 레벨과 무관하게 틱이 돌아간다.
class UEventProcessor
{
	friend UEventManager;
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

	static bool IsPlayerActivated()
	{
		return bIsPlayerActivated;
	}

	static bool IsPlayerAnimFirstTick()
	{
		return bIsPlayAnimFirstTick;
	}

	static void SetIsPlayerAnimFirstTick(bool _Value)
	{
		bIsPlayAnimFirstTick = _Value;
	}

	static void DeactivatePlayerControl();

	inline static FMoveBaseData MoveBaseData;
	inline static FMoveDynamicPathData MoveDynamicPathData;
	inline static FMoveWRData MoveWRData;
	inline static FSurpriseData SurpriseData;
	inline static FWaitData WaitData;

protected:

private:
	inline static AEventTrigger* Trigger = nullptr;
	inline static bool IsRunningValue = false;
	inline static const UEventStream* Stream = nullptr;
	inline static int CurCommandIndex = 0;
	
	static void Tick(float _DeltaTime);
	static void EndRun();

	// 프로세싱 함수
	/// <return>이벤트 명령 종료 여부</return>
	static void ActivatePlayerControl();

	// 프로세싱 변수
	inline static bool bIsPlayerActivated = true;
	inline static bool bIsPlayAnimFirstTick = true;

private:
	// constructor destructor
	UEventProcessor();
	~UEventProcessor();
};

