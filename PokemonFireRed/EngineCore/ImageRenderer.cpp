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

	// Renderers[Order]���� this�� �����Ѵ�.
	Renderers[GetOrder()].remove(this);
	
	// �θ� ��� Order�� �����Ѵ�.
	UTickObject::SetOrder(_Order);

	// Renderers[Order]�� this�� �߰��Ѵ�.
	Renderers[GetOrder()].push_back(this);
}

void UImageRenderer::Render(float _DeltaTime)
{
	if (nullptr == Image)
	{
		MsgBoxAssert("�������� �̹����� �������� �ʽ��ϴ�.");
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
		MsgBoxAssert(std::string(_Name) + " �̹����� �������� �ʽ��ϴ�.");
		return;
	}
}

void UImageRenderer::BeginPlay()
{
	// UTickObject::BeginPlay�� �ƹ��� ���۵� ���� �ʰ� ������ �θ��� �Լ��� �׻� �������ִ� ������ ������ �Ѵ�.
	// ���� ������ ����ż� �θ� �Լ��� ������ �߰����� �𸣱� �����̴�.
	USceneComponent::BeginPlay();
}

