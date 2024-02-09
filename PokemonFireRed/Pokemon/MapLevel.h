#pragma once
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EngineCore/EngineResourcesManager.h>
#include "PokemonLevel.h"
#include "Player.h"
#include "Map.h"

class AEventActor;

class UMapLevel : public UPokemonLevel
{
	friend AEventActor;
public:
	// constructor destructor
	UMapLevel();
	~UMapLevel();

	// delete Function
	UMapLevel(const UMapLevel& _Other) = delete;
	UMapLevel(UMapLevel&& _Other) noexcept = delete;
	UMapLevel& operator=(const UMapLevel& _Other) = delete;
	UMapLevel& operator=(UMapLevel&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	template <typename EventActorType>
	EventActorType* SpawnEventActor(const FTileVector& _Point)
	{
		EventActorType* NewEventActor = SpawnActor<EventActorType>();
		FVector Pos = _Point.ToFVector();
		NewEventActor->SetActorLocation(Pos);
		NewEventActor->SetMapLevel(this);

		if (AllEventActor.contains(_Point))
		{
			MsgBoxAssert("이미 이벤트 액터가 존재하는 위치 " + _Point.ToString() + "에 이벤트 액터를 생성하려고 했습니다.");
			return nullptr;
		}

		AllEventActor[_Point] = NewEventActor;
		return NewEventActor;
	}

	bool IsEventActor(const FTileVector& _Point)
	{
		return AllEventActor.contains(_Point);
	}

	AEventActor* FindEventActor(const FTileVector& _Point)
	{
		if (!IsEventActor(_Point))
		{
			MsgBoxAssert(_Point.ToString() + "에는 이벤트 액터가 존재하지 않습니다.");
		}

		return AllEventActor[_Point];
	}

protected:
	// 하위 클래스에서 너무 많이 사용할 것 같아서 protected로 설정
	AMap* Map = nullptr;
	APlayer* Player = nullptr;
	UEngineDirectory CurDir;

	std::map<FTileVector, AEventActor*> AllEventActor;

	void LevelStart(ULevel* _PrevLevel) override;
};

