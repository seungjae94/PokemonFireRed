#include "Actor.h"

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
			return;
		}

		delete Renderer;
		Renderer = nullptr;
	}

	Renderers.clear();
}

UImageRenderer* AActor::CreateImageRenderer(int _Order)
{
	UImageRenderer* NewRenderer = new UImageRenderer();
	UActorComponent* ActorCom = NewRenderer;
	ActorCom->SetOwner(this);
	ActorCom->SetOrder(_Order);
	ActorCom->BeginPlay();
	Renderers.push_back(NewRenderer);
	return NewRenderer;
}

void AActor::Destroy()
{
	UTickObject::Destroy();

	for (UImageRenderer* Renderer : Renderers)
	{
		Renderer->Destroy();
	}
}
