#pragma once
#include <map>
#include <vector>
#include <functional>
#include "PokemonMath.h"

class UPokemonLevel;
class AEventTarget;
class UEventTargetInitialSetting;
class AEventTrigger;
class UEventProcessor;
class APlayer;
class UEventManagerReleaser;

using Event = std::function<bool()>;

// ��� �̺�Ʈ ó���� ����ϴ� Ŭ����
// - �� �������� ���͸� �����Ϸ��� �ݵ�� EventManager Ŭ������ ���ؼ� �����ؾ� �Ѵ�.
class UEventManager
{
	friend UPokemonLevel;
	friend UEventManagerReleaser;
public:
	// delete Function
	UEventManager(const UEventManager& _Other) = delete;
	UEventManager(UEventManager&& _Other) noexcept = delete;
	UEventManager& operator=(const UEventManager& _Other) = delete;
	UEventManager& operator=(UEventManager&& _Other) noexcept = delete;

	static void Register(AEventTrigger* _Trigger, Event _Event);

	static bool IsTrigger(std::string_view _LevelName, const FTileVector& _Point);

	static void Trigger(std::string_view _LevelName, const FTileVector& _Point);

	/// <summary>
	/// ���͸� ������ ��θ� ���� �̵���Ų��.
	/// </summary>
	/// <param name="_Path">�̵� ���</param>
	/// <param name="_MoveSpeed">�̵� �ӵ�</param>
	/// <returns>EventEnd �̺�Ʈ ���� ����</returns>
	static bool MoveActor(std::string_view _MapName, std::string_view _TargetName, std::vector<FTileVector> _Path, float _MoveSpeed = 3.6f);

	//static bool Wait(float _DeltaTime, float _WaitTime);

	static bool ChangeMap(std::string_view _CurMapName, std::string_view _NextMapName, const FTileVector& _Point);

	static bool ChangePoint(std::string_view _MapName, std::string_view _TargetName, const FTileVector& _Point);

	static bool ChangeDirection(std::string_view _MapName, std::string_view _TargetName, const FTileVector& _Direction);

	/// <summary>
	/// �̺�Ʈ�� ������ �÷��̾ �ٽ� ĳ���͸� ��Ʈ�� �� �� �ִ� ���·� �����.
	/// </summary>
	/// <param name="_LevelName"></param>
	/// <returns></returns>
	static bool Finish(std::string_view _LevelName);

protected:
	// constructor destructor
	UEventManager();
	~UEventManager();
private:
	// AllPlayers[LevelName]
	// - �÷��̾�� ���� ���� �� �÷��̾� Ÿ������ �ٷ�� �� ���� �ֱ� ������ �߰��� �����Ѵ�.
	static std::map<std::string, APlayer*> AllPlayers;

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

	// DeltaTime ���
	static float DeltaTime;

	static void Release();
};