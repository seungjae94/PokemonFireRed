#pragma once
#include <map>
#include <list>
#include <EngineBase/NameObject.h>

// 상위 개념인 레벨이 하위 개념인 액터를 직접 참조하면 순환 참조 문제가 발생할 수 있다.
// 클래스 전방 선언으로 순환 참조 문제를 회피한다.
class AActor;

// 액터들이 활동할 무대
class ULevel : public UNameObject
{
public:
	// constructor destructor
	ULevel();

	// 레벨이 액터를 생성하므로 레벨이 액터를 릴리즈해야 한다.
	~ULevel();

	// delete Function
	ULevel(const ULevel& _Other) = delete;
	ULevel(ULevel&& _Other) noexcept = delete;
	ULevel& operator=(const ULevel& _Other) = delete;
	ULevel& operator=(ULevel&& _Other) noexcept = delete;

	virtual void BeginPlay() 
	{
	}

	virtual void Tick(float _DeltaTime) 
	{
	}

	// 레벨에 액터를 생성해주는 함수
	// - 액터의 월드를 현재 레벨로 설정하고, 액터의 BeginPlay를 호출한다.
	// - 액터를 map에 저장한다.
	template<typename ActorType>
	ActorType* SpawnActor(int _Order = 0)
	{
		ActorType* NewActor = new ActorType();
		ActorInit(NewActor);
		AllActor[_Order].push_back(NewActor);
		return NewActor;
	}

protected:

private:
	std::map<int, std::list<AActor*>> AllActor;

	// 액터의 월드를 현재 레벨로 설정하고, 액터의 BeginPlay를 호출한다.
	void ActorInit(AActor* _Actor);
};

