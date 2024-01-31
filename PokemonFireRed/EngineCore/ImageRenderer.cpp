#include "ImageRenderer.h"

#include "Level.h"
#include "Actor.h"

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
	FTransform ThisTrans = GetTransform();
	FTransform OwnerTrans = GetOwner()->GetTransform();

	ThisTrans.AddPosition(OwnerTrans.GetPosition());

	// TODO: ThisTrans�� ���� �ִ� �̹����� �׸��� ����
}

void UImageRenderer::BeginPlay()
{
	// UTickObject::BeginPlay�� �ƹ��� ���۵� ���� �ʰ� ������ �θ��� �Լ��� �׻� �������ִ� ������ ������ �Ѵ�.
	// ���� ������ ����ż� �θ� �Լ��� ������ �߰����� �𸣱� �����̴�.
	USceneComponent::BeginPlay();
}

