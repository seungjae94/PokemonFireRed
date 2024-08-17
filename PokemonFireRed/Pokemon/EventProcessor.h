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

protected:

private:
	inline static AEventTrigger* Trigger = nullptr;
	inline static bool IsRunningValue = false;
	inline static const UEventStream* Stream = nullptr;
	inline static int CurCommandIndex = 0;
	
	static void Tick(float _DeltaTime);
	static void EndRun();

	static std::string ToUpper(std::string_view _Name);

	// ���μ��� �Լ�
	/// <return>�̺�Ʈ ��� ���� ����</return>
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

	// ���μ��� ���� - ����
	inline static bool IsPlayerActivated = true;

	// ���μ��� ���� - �̵�
	inline static EMoveState MoveState = EMoveState::None;
	inline static std::vector<AEventTarget*> MoveTargets;
	inline static std::vector<FTileVector> MovePrevPoints;
	inline static std::vector<FTileVector> MoveNextPoints;
	inline static std::vector<bool> MoveEnds;
	inline static int MovePathIndex = 0;
	inline static float MoveTime = 0.0f;
	inline static float MoveTimer = 0.0f;

	// ���μ��� ���� - �̵� (��� ���� ����)
	inline static std::vector<std::vector<FTileVector>> DynamicPaths;

	// ���μ��� ���� - �̵� (Ÿ�� ���� X)
	inline static FVector MoveWRPrevPos;
	inline static FVector MoveWRNextPos;
	inline static int MoveWRPathIndex = -1;
	inline static float MoveWRTime = 0.0f;
	inline static float MoveWRTimer = 0.0f;

	// ���μ��� ���� - �ִϸ��̼�
	inline static bool PlayAnimIsFirstTick = true;

	// ���μ��� ���� - Surprise
	inline static bool SurpriseFirstTick = true;
	inline static ASurprise* Surprise = nullptr;

private:
	// constructor destructor
	UEventProcessor();
	~UEventProcessor();
};

