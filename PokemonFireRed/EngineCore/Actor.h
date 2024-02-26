#pragma once
#include <EngineBase\Transform.h>
#include "TickObject.h"
#include <EngineBase\NameObject.h>
#include "ImageRenderer.h"
#include "Collision.h"
#include "Level.h"

class ULevel;
class UActorComponent;
class UImageRenderer;

// 이름이 있고, 매 틱마다 특정 동작을 하며, 레벨 상에서 Transform을 갖는 오브젝트.
class AActor : public UNameObject, public UTickObject
{
	friend ULevel;		// ULevel에 SetWorld 공개
public:
	// constructor destructor
	AActor();

	// 렌더러를 생성한게 액터이므로 렌더러를 릴리즈하는 책임은 액터에게 있다.
	~AActor();

	// delete Function
	AActor(const AActor& _Other) = delete;
	AActor(AActor&& _Other) noexcept = delete;
	AActor& operator=(const AActor& _Other) = delete;
	AActor& operator=(AActor&& _Other) noexcept = delete;

	// 트랜스폼 변경 함수

	FVector GetActorLocation()
	{
		return Transform.GetPosition();
	}

	void SetActorLocation(FVector _Value)
	{
		Transform.SetPosition(_Value);
	}

	void AddActorLocation(FVector _Value)
	{
		Transform.AddPosition(_Value);
	}

	void SetActorScale(FVector _Value)
	{
		Transform.SetScale(_Value);
	}

	FTransform GetTransform()
	{
		return Transform;
	}

	// World 관련 함수

	ULevel* GetWorld()
	{
		return World;
	}

	template<typename EnumType>
	UCollision* CreateCollision(EnumType _Order = 0)
	{
		return CreateCollision(static_cast<int>(_Order));
	}

	template<typename EnumType>
	UImageRenderer* CreateImageRenderer(EnumType _Order = 0)
	{
		return CreateImageRenderer(static_cast<int>(_Order));
	}

	UCollision* CreateCollision(int _Order = 0);

	// 렌더러를 생성한다.
	// - 렌더러의 Owner를 현재 액터로 설정한다.
	// - 렌더러의 SetOrder를 호출한다. 이 때 렌더러가 레벨이 갖고 있는 렌더러 맵에 자신을 추가한다.
	// - 렌더러의 BeginPlay를 호출한다.
	// - 액터가 갖고 있는 렌더러 리스트에 렌더러를 추가한다.
	UImageRenderer* CreateImageRenderer(int _Order = 0);


	// 자신이 Destroy할 때 갖고 있는 렌더러도 Destroy 해야 한다.
	// - 레벨도 렌더러 맵을 갖고 있기 때문에 렌더러도 메모리에서 지우기 전에 레벨의 렌더러 맵에서 지워줘야 한다.
	void Destroy(float _DestroyTime = 0.0f) override;

	void DestroyUpdate(float _DeltaTime) override;

	void SetActive(bool _Active, float _ActiveTime = 0.0f) override;
	void ActiveUpdate(float _DeltaTime) override;

	// Actor가 살아있을 때, 렌더러와 콜리전이 Destory 된다면 액터가 직접 삭제한다.
	void CheckReleaseChild();

	void AllRenderersActiveOff();
	void AllRenderersActiveOn();

protected:
	void Tick(float _DeltaTime) override;
	void ChildTick(float _DeltaTime);
private:
	std::list<UImageRenderer*> Renderers;
	std::list<UCollision*> Collisions;

	ULevel* World = nullptr;
	FTransform Transform = FTransform();

	// 레벨이 액터를 생성할 때 호출되어 액터의 World를 해당 레벨로 설정해주는 함수
	// - 레벨에서만 호출할 함수이므로 private으로 설정한다.
	// - 레벨이 호출할 수 있어야 하므로 ULevel에 friend를 걸어준다.
	void SetWorld(ULevel* _World)
	{
		World = _World;
	}
};

