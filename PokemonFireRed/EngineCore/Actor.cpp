#include "Actor.h"
#include "ImageRenderer.h"

AActor::AActor()
{
}

AActor::~AActor()
{
	for (UImageRenderer* Renderer : Renderers)
	{
		if (nullptr == Renderer)
		{
			MsgBoxAssert("이미지 렌더러가 nullptr인 상황이 존재합니다.");
		}

		delete Renderer;
		Renderer = nullptr;
	}
	Renderers.clear();

	for (UCollision* Collision : Collisions)
	{
		if (nullptr == Collision)
		{
			MsgBoxAssert("콜리전이 nullptr인 상황이 존재합니다.");
		}

		delete Collision;
		Collision = nullptr;
	}
	Collisions.clear();
}

UImageRenderer* AActor::CreateImageRenderer(int _Order)
{
	UImageRenderer* Component = new UImageRenderer();
	UActorComponent* ActorCom = Component;
	ActorCom->SetOwner(this);
	ActorCom->SetOrder(_Order);
	ActorCom->BeginPlay();
	Renderers.push_back(Component);
	return Component;
}

UCollision* AActor::CreateCollision(int _Order)
{
	UCollision* Component = new UCollision();
	UActorComponent* ActorCom = Component;
	ActorCom->SetOwner(this);
	ActorCom->SetOrder(_Order);
	ActorCom->BeginPlay();
	Collisions.push_back(Component);
	return Component;
}

void AActor::Destroy(float _DestroyTime)
{
	UTickObject::Destroy(_DestroyTime);

	for (UImageRenderer* Renderer : Renderers)
	{
		Renderer->Destroy(_DestroyTime);
	}
}

void AActor::DestroyUpdate(float _DeltaTime)
{
	UTickObject::DestroyUpdate(_DeltaTime);

	for (UImageRenderer* Renderer : Renderers)
	{
		Renderer->DestroyUpdate(_DeltaTime);
	}
}

void AActor::ActiveUpdate(float _DeltaTime)
{
	UTickObject::ActiveUpdate(_DeltaTime);

	for (UImageRenderer* Renderer : Renderers)
	{
		Renderer->ActiveUpdate(_DeltaTime);
	}
}

void AActor::Tick(float _DeltaTime)
{
	UTickObject::Tick(_DeltaTime);
	DestroyUpdate(_DeltaTime);
}
