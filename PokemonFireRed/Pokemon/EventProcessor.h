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

// �� �����Ӹ��� �̺�Ʈ ���� ��� �̺�Ʈ�� ó�����ִ� ��ü
// ���� ������ �����ϰ� ƽ�� ���ư���.
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
	/// �̺�Ʈ ������ �õ��Ѵ�.
	/// ������ �����ϴ� �̺�Ʈ �׷��� ���� ��� false�� ��ȯ�Ѵ�.
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

	// ���� �Լ�
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

	// ���μ��� �Լ�
	/// <return>�̺�Ʈ ��� ���� ����</return>
	void ActivatePlayerControl();
	void DeactivatePlayerControl();
	bool ProcessMove();
	void PostProcessMove(AEventTarget* _Target);
	bool ProcessMoveWithoutRestriction();
	void PostProcessMoveWR(AEventTarget* _Target);
	bool ProcessFadeIn();
	bool ProcessFadeOut();
	bool ProcessWait();
	bool ProcessPlayAnimation();
	bool ProcessChat();
	bool ProcessChangeLevel();
	bool ProcessChangePoint();
	bool ProcessChangeDirection();
	bool ProcessStarePlayer();
	bool ProcessHideActor();
	bool ProcessShowActor();
	bool ProcessCameraFocus();

	// ���μ��� ���� - ����
	bool IsPlayerActivated = true;

	// ���μ��� ���� - �̵�
	FTileVector MovePrevPoint;
	FTileVector MoveNextPoint;
	int MovePathIndex = -1;		// -1�� ù ��° ƽ���� �ǹ��Ѵ�.
	float MoveTime = 0.0f;
	float MoveTimer = 0.0f;

	// ���μ��� ���� - �̵� (Ÿ�� ���� X)
	FVector MoveWRPrevPos;
	FVector MoveWRNextPos;
	int MoveWRPathIndex = -1;
	float MoveWRTime = 0.0f;
	float MoveWRTimer = 0.0f;

	// ���μ��� ���� - �ִϸ��̼�
	bool PlayAnimIsFirstTick = true;
};

