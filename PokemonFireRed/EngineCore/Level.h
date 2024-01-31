#pragma once
#include <map>
#include <list>
#include <EngineBase/NameObject.h>
#include <EngineCore/EngineCore.h>

// 상위 개념인 레벨이 하위 개념인 액터, 렌더러를 직접 참조하면 순환 참조 문제가 발생할 수 있다.
// 클래스 전방 선언으로 순환 참조 문제를 회피한다.
class AActor;
class UImageRenderer;

// 액터들이 활동할 무대
class ULevel : public UNameObject
{
	friend UEngineCore;
	friend UImageRenderer; // 렌더러에서 Renderers 맵을 수정
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
	std::map<int, std::list<UImageRenderer*>> Renderers;

	// 액터의 월드를 현재 레벨로 설정하고, 액터의 BeginPlay를 호출한다.
	void ActorInit(AActor* _Actor);

	// 오브젝트(액터) 틱 호출
	void LevelTick(float _DeltaTime);

	// 렌더링: 차후 구현
	void LevelRender(float _DeltaTime);

	// Destroy한 오브젝트(액터) 릴리즈
	void LevelRelease(float _DeltaTime);
};

