#include "ImageRenderer.h"
#include "EngineCore.h"
#include "Actor.h"
#include "Level.h"
#include <EngineCore\EngineResourcesManager.h>

UImageRenderer::UImageRenderer() 
{
}

UImageRenderer::~UImageRenderer() 
{
}

void UImageRenderer::SetOrder(int _Order)
{
	AActor* Owner = GetOwner();
	ULevel* Level = Owner->GetWorld();
	std::map<int, std::list<UImageRenderer*>>& Renderers = Level->Renderers;

	// Renderers[Order]에서 this를 제거한다.
	Renderers[GetOrder()].remove(this);
	
	// 부모 멤버 Order를 변경한다.
	UTickObject::SetOrder(_Order);

	// Renderers[Order]에 this를 추가한다.
	Renderers[GetOrder()].push_back(this);
}

void UImageRenderer::Render(float _DeltaTime)
{
	if (nullptr == Image)
	{
		MsgBoxAssert("렌더러에 이미지가 존재하지 않습니다.");
	}

	FTransform ThisTrans = GetTransform();
	FTransform OwnerTrans = GetOwner()->GetTransform();

	ThisTrans.AddPosition(OwnerTrans.GetPosition());

	GEngine->MainWindow.GetBackBufferImage()->TransCopy(Image, ThisTrans, ImageCuttingTransform);
}

void UImageRenderer::SetImage(std::string_view _Name)
{
	Image = UEngineResourcesManager::GetInst().FindImg(_Name);

	if (nullptr == Image)
	{
		MsgBoxAssert(std::string(_Name) + " 이미지가 존재하지 않습니다.");
		return;
	}
}

void UImageRenderer::BeginPlay()
{
	// UTickObject::BeginPlay가 아무런 동작도 하지 않고 있지만 부모의 함수는 항상 실행해주는 습관을 가져야 한다.
	// 언제 엔진이 변경돼서 부모 함수에 동작이 추가될지 모르기 때문이다.
	USceneComponent::BeginPlay();
}

