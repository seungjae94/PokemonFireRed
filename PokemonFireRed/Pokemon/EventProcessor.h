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

// �� �����Ӹ��� �̺�Ʈ ���� ��� �̺�Ʈ�� ó�����ִ� ��ü
// ���� ������ �����ϰ� ƽ�� ���ư���.
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

	// ���μ��� ���� - ����
	bool IsPlayerActivated = true;

	// ���μ��� ���� - �̵�
	EMoveState MoveState = EMoveState::None;
	std::vector<AEventTarget*> MoveTargets;
	std::vector<FTileVector> MovePrevPoints;
	std::vector<FTileVector> MoveNextPoints;
	std::vector<bool> MoveEnds;
	int MovePathIndex = 0;
	float MoveTime = 0.0f;
	float MoveTimer = 0.0f;

	// ���μ��� ���� - �̵� (��� ���� ����)
	std::vector<std::vector<FTileVector>> DynamicPaths;

	// ���μ��� ���� - �̵� (Ÿ�� ���� X)
	FVector MoveWRPrevPos;
	FVector MoveWRNextPos;
	int MoveWRPathIndex = -1;
	float MoveWRTime = 0.0f;
	float MoveWRTimer = 0.0f;

	// ���μ��� ���� - �ִϸ��̼�
	bool PlayAnimIsFirstTick = true;

	// ���μ��� ���� - Surprise
	bool SurpriseFirstTick = true;
	ASurprise* Surprise = nullptr;
};

