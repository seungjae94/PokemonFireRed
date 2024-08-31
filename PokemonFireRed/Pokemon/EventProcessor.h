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

// �� �����Ӹ��� �̺�Ʈ ���� ��� �̺�Ʈ�� ó�����ִ� ��ü
// ���� ������ �����ϰ� ƽ�� ���ư���.
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
	/// �̺�Ʈ ������ �õ��Ѵ�.
	/// ������ �����ϴ� �̺�Ʈ �׷��� ���� ��� false�� ��ȯ�Ѵ�.
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

	// ���μ��� �Լ�
	/// <return>�̺�Ʈ ��� ���� ����</return>
	static void ActivatePlayerControl();

	// ���μ��� ����
	inline static bool bIsPlayerActivated = true;
	inline static bool bIsPlayAnimFirstTick = true;

private:
	// constructor destructor
	UEventProcessor();
	~UEventProcessor();
};

