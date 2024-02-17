#pragma once
#include <map>
#include <vector>
#include <functional>
#include "PokemonMath.h"
#include "EventCondition.h"

class UPokemonLevel;
class AEventTarget;
class UEventTargetInit;
class AEventTrigger;
class UEventProcessor;
class UEventStream;
class APlayer;
class UEventManagerReleaser;
class UEventCondition;
class AMenuWindow;
class ADialogueWindow;
class ABlackScreen;
class AUIElement;
enum class EFontColor;

// ��� �̺�Ʈ ó���� ����ϴ� Ŭ����
// - �� �������� ���͸� �����Ϸ��� �ݵ�� EventManager Ŭ������ ���ؼ� �����ؾ� �Ѵ�.
class UEventManager
{
	friend UEventProcessor;
	friend UPokemonLevel;
	friend UEventManagerReleaser;
public:
	// delete Function
	UEventManager(const UEventManager& _Other) = delete;
	UEventManager(UEventManager&& _Other) noexcept = delete;
	UEventManager& operator=(const UEventManager& _Other) = delete;
	UEventManager& operator=(UEventManager&& _Other) noexcept = delete;

	// �̺�Ʈ ���
	static void RegisterEvent(AEventTrigger* _Trigger, const UEventCondition& _Condition, UEventStream _Stream);

	// �̺�Ʈ Ʈ����
	static bool TriggerEvent(AEventTrigger* _Trigger, EEventTriggerAction _Action = EEventTriggerAction::Direct);

	// ��� �̺�Ʈ �Լ�
	// - ���̵� ����Ʈ�� ���� �̺�Ʈ ����� 
	//   �̺�Ʈ Ʈ���Ÿ� ������ �ʰ� ��� �����ص� ������ ���� �ʴ´�.

	static void SetLevel(std::string_view _LevelName);

	static void SetPoint(std::string_view _MapName, std::string_view _TargetName, const FTileVector& _Point);

	static void SetDirection(std::string_view _MapName, std::string_view _TargetName, const FTileVector& _Direction);

	// ã�� ���� �Լ�
	static APlayer* GetCurPlayer();
	static AMenuWindow* GetCurMenuWindow();
	static ADialogueWindow* GetCurDialogueWindow();
	static ABlackScreen* GetCurBlackScreen();
	static ABlackScreen* GetBlackScreen(std::string_view _LevelName);
	static AEventTarget* FindTarget(std::string_view _LevelName, std::string_view _TargetName);

	static std::string GetCurLevelName()
	{
		return CurLevelName;
	}

	static float GetDeltaTime()
	{
		return DeltaTime;
	}

	static void PlayerEventProcessingOff()
	{
		PlayerEventProcessing = false;
	}

protected:
	// constructor destructor
	UEventManager();
	~UEventManager();
private:
	static bool CameraFollowing;
	static bool PlayerEventProcessing;

	static std::string CurLevelName;

	// AllPlayers[LevelName]
	// - �÷��̾�� ���� ���� �� �÷��̾� Ÿ������ �ٷ�� �� ���� �ֱ� ������ �߰��� �����Ѵ�.
	static std::map<std::string, APlayer*> AllPlayers;

	// AllMenuWindows[LevelName][ElementName]
	// - �޴�â, ��ȭâ�� Ŀ�� �̵� �� �÷��̾� Ÿ������ �ٷ�� �� ���� �ֱ� ������ �߰��� �����Ѵ�.
	static std::map<std::string, std::map<std::string, AUIElement*>> AllUIElements;

	// AllTargets[LevelName][TargetName]
	// - 'A������ B��� �̸��� ���͸� �̵�������'��� ��û�� ó���Ϸ��� A������ B��� �̸��� ���͸� ã�� �� �־�� �Ѵ�.
	static std::map<std::string, std::map<std::string, AEventTarget*>> AllTargets;

	// AllTriggers[LevelName][Point]
	// - �÷��̾ Ư�� ��ġ�� Ʈ���Ű� �ִ��� Ȯ���� �� ���
	// - �ش� Ʈ���Ÿ� ���۽�ų �� ���
	static std::map<std::string, std::map<FTileVector, AEventTrigger*>> AllTriggers;

	// AllProcessors[Trigger]
	// - �̺�Ʈ �Ŵ����� ���� ����� �����ϰ� ��� ������ �� ������ ��� Ʈ������ �ݹ�(�̺�Ʈ)�� �����ϱ⿡ ���������� �ʴ�.
	// - Ʈ���ſ� �ϴ��Ϸ� �����ؼ� �ݹ��� ����ϰ� ���� ����� �����ϰ� �����ϸ鼭 �������ִ� ��ü�� �ʿ��ϴ�.
	// - �̺�Ʈ ���μ����� �ٷ� �� ������ �ð� �ȴ�.
	static std::map<AEventTrigger*, UEventProcessor*> AllProcessors;
	
	static void Tick(float _DeltaTime);
	
	static void AddTarget(AEventTarget* _Target, const UEventTargetInit& _Setting);
	static void AddTrigger(AEventTrigger* _Trigger, const UEventTargetInit& _Setting);
	static void AddPlayer(APlayer* _Player, const FTileVector& _Point);
	static void AddUIElement(AUIElement* _UIElement, std::string_view _Name);

	// DeltaTime ���
	static float DeltaTime;

	// �̺�Ʈ ���μ��� ������
	static void Release();

	// �̺�Ʈ ����
	static void CheckPlayerEvent();
};