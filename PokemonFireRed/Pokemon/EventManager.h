#pragma once
#include <map>
#include <vector>
#include <functional>
#include "PokemonMath.h"
#include "EventCondition.h"

class UPokemonLevel;
class AEventTarget;
class UEventTargetInitialSetting;
class AEventTrigger;
class UEventProcessor;
class APlayer;
class UEventManagerReleaser;
class UEventCondition;
class AMenuWindow;
class ADialogueWindow;
class ABlackScreen;
class AUIElement;
enum class EFontColor;

using Event = std::function<bool()>;
#define ToEvent(Func) [this](){return Func();}

// ��� �̺�Ʈ ó���� ����ϴ� Ŭ����
// - �� �������� ���͸� �����Ϸ��� �ݵ�� EventManager Ŭ������ ���ؼ� �����ؾ� �Ѵ�.
class UEventManager
{
	friend AEventTrigger; // ���� �ð� ���� �����ϴ� ��ƿ �Լ��� �̺�Ʈ Ʈ���ſ��Ը� ����
	friend UPokemonLevel;
	friend UEventManagerReleaser;
public:
	// delete Function
	UEventManager(const UEventManager& _Other) = delete;
	UEventManager(UEventManager&& _Other) noexcept = delete;
	UEventManager& operator=(const UEventManager& _Other) = delete;
	UEventManager& operator=(UEventManager&& _Other) noexcept = delete;

	// �̺�Ʈ ���
	static void RegisterEvent(AEventTrigger* _Trigger, const UEventCondition& _Condition, Event _Event);

	// �̺�Ʈ Ʈ����
	static bool TriggerEvent(AEventTrigger* _Trigger, EEventTriggerAction _Action = EEventTriggerAction::Direct);

	// ��� �̺�Ʈ �Լ�
	static bool ChangeLevel(std::string_view _LevelName);

	static bool ChangeMap(std::string_view _NextMapName, const FTileVector& _Point);

	static bool ChangePoint(std::string_view _MapName, std::string_view _TargetName, const FTileVector& _Point);

	static bool ChangeDirection(std::string_view _MapName, std::string_view _TargetName, const FTileVector& _Direction);

	static bool StealPlayerControl();

	static bool GiveBackPlayerControl();

	/// <summary>
	/// ���͸� ������ ��θ� ���� �̵���Ų��.
	/// </summary>
	/// <param name="_Path">�̵� ���</param>
	/// <param name="_MoveSpeed">�̵� �ӵ�</param>
	/// <returns>EventEnd �̺�Ʈ ���� ����</returns>
	static bool MoveActor(std::string_view _MapName, std::string_view _TargetName, std::vector<FTileVector> _Path, float _MoveSpeed = 3.6f);

	static bool Chat(const std::vector<std::wstring>& _Dialogue, EFontColor _Color, int _LineSpace = 14, bool _IsSequential = false);

	static bool EndEvent(AEventTrigger* _Trigger, bool _GiveBackPlayerControl = true);

	static bool FadeOut(float _Time);

	static bool FadeIn(float _Time);
	static bool FadeIn(std::string_view _LevelName, float _Time);

	static bool Wait(float _Time);

	// ã�� ���� �Լ�
	static APlayer* GetCurPlayer();
	static AMenuWindow* GetCurMenuWindow();
	static ADialogueWindow* GetCurDialogueWindow();
	static ABlackScreen* GetCurBlackScreen();
	static ABlackScreen* GetBlackScreen(std::string_view _LevelName);

protected:
	// constructor destructor
	UEventManager();
	~UEventManager();
private:
	static bool CameraFollowing;

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
	
	static void AddTarget(AEventTarget* _Target, const UEventTargetInitialSetting& _Setting);
	static void AddTrigger(AEventTrigger* _Trigger, const UEventTargetInitialSetting& _Setting);
	static void AddPlayer(APlayer* _Player, const FTileVector& _Point);
	static void AddUIElement(AUIElement* _UIElement, std::string_view _Name);

	// DeltaTime ���
	static float DeltaTime;

	// �̺�Ʈ ���μ��� ������
	static void Release();

	// �̺�Ʈ ����
	static void CheckPlayerEvent();
};