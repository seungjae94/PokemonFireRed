#pragma once
#include <EngineCore/Level.h>
#include "EventManager.h"
#include "EventTrigger.h"
#include "Player.h"
#include "MenuWindow.h"
#include "DialogueWindow.h"
#include "FadeLevelChanger.h"
#include "ScrollBar.h"

// 포켓몬 게임의 모든 레벨이 공통적으로 수행해야 하는 Tick 동작을 정의하는 클래스
// - 최초 개발 의도: 이벤트 매니저의 Tick을 돌리기 위해 생성
class UPokemonLevel : public ULevel
{
public:
	// constructor destructor
	UPokemonLevel();
	~UPokemonLevel();

	// delete Function
	UPokemonLevel(const UPokemonLevel& _Other) = delete;
	UPokemonLevel(UPokemonLevel&& _Other) noexcept = delete;
	UPokemonLevel& operator=(const UPokemonLevel& _Other) = delete;
	UPokemonLevel& operator=(UPokemonLevel&& _Other) noexcept = delete;

	template <typename EventTargetType>
	EventTargetType* SpawnEventTarget(UEventTargetInit _Setting)
	{
		EventTargetType* EventTarget = SpawnActor<EventTargetType>();
		UEventManager::AddTarget(EventTarget, _Setting);
		return EventTarget;
	}

	APlayer* SpawnPlayer(const FTileVector& _Point)
	{
		APlayer* Player = SpawnActor<APlayer>();
		UEventManager::AddPlayer(Player, _Point);
		return Player;
	}

	template <typename UIElementType>
	UIElementType* SpawnUIElement(std::string_view _Name)
	{
		UIElementType* UIElement = SpawnActor<UIElementType>();
		UEventManager::AddUIElement(UIElement, _Name);
		return UIElement;
	}

	template <typename EventTriggerType>
	EventTriggerType* SpawnEventTrigger(UEventTargetInit _Setting)
	{
		EventTriggerType* EventTrigger = SpawnActor<EventTriggerType>();
		UEventManager::AddTarget(EventTrigger, _Setting);
		UEventManager::AddTrigger(EventTrigger, _Setting);
		return EventTrigger;
	}

	template <typename EventTriggerType>
	EventTriggerType* SpawnEventTrigger(std::string_view _Name)
	{
		UEventTargetInit Init;
		Init.SetName(_Name);
		EventTriggerType* EventTrigger = SpawnActor<EventTriggerType>();
		UEventManager::AddTarget(EventTrigger, Init);
		UEventManager::AddTrigger(EventTrigger, Init);
		return EventTrigger;
	}

	APokemonText* SpawnText(UImageRenderer* _Container, EPivotType _PivotType = EPivotType::LeftTop, EAlignType _Alignment = EAlignType::Left)
	{
		APokemonText* Text = SpawnActor<APokemonText>();
		Text->Container = _Container;
		Text->PivotType = _PivotType;
		Text->Alignment = _Alignment;
		return Text;
	}

	AScrollBar* SpawnScrollBar(UImageRenderer* _Container, EPivotType _PivotType = EPivotType::LeftTop)
	{
		AScrollBar* Bar = SpawnActor<AScrollBar>();
		Bar->Container = _Container;
		Bar->PivotType = _PivotType;
		return Bar;
	}

	void ChangeLevelFade(std::string_view _Name, float _FadeInTime = 0.5f, float _FadeOutTime = 0.5f)
	{
		FadeLevelChanger->ChangeLevel(_Name, _FadeInTime, _FadeOutTime);
	}

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
private:
	AFadeLevelChanger* FadeLevelChanger = nullptr;
};

