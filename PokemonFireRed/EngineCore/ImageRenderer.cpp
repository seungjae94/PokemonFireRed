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

int UAnimationInfo::Update(float _DeltaTime)
{
	CurTime -= _DeltaTime;

	if (0.0f >= CurTime)
	{
		CurTime = Times[CurFrame];
		++CurFrame;
	}

	if (Indexs.size() <= CurFrame)
	{
		if (true == Loop)
		{
			CurFrame = 0;
		}
		else
		{
			--CurFrame;
		}
	}

	int Index = Indexs[CurFrame];

	return Index;
}


void UImageRenderer::Render(float _DeltaTime)
{
	if (nullptr == Image)
	{
		MsgBoxAssert("이미지가 존재하지 않는 랜더러 입니다");
	}

	if (nullptr != CurAnimation)
	{
		Image = CurAnimation->Image;
		InfoIndex = CurAnimation->Update(_DeltaTime);
	}

	FTransform RendererTrans = GetTransform();

	FTransform ActorTrans = GetOwner()->GetTransform();

	RendererTrans.AddPosition(ActorTrans.GetPosition());

	std::string Out = std::to_string(InfoIndex);

	Out += "\n";
	OutputDebugStringA(Out.c_str());

	GEngine->MainWindow.GetBackBufferImage()->TransCopy(Image, RendererTrans, InfoIndex);

}

void UImageRenderer::BeginPlay()
{
	// UTickObject::BeginPlay가 아무런 동작도 하지 않고 있지만 부모의 함수는 항상 실행해주는 습관을 가져야 한다.
	// 언제 엔진이 변경돼서 부모 함수에 동작이 추가될지 모르기 때문이다.		
	USceneComponent::BeginPlay();
}

void UImageRenderer::SetImage(std::string_view _Name, int _InfoIndex /*= 0*/)
{
	Image = UEngineResourcesManager::GetInst().FindImg(_Name);

	if (nullptr == Image)
	{
		MsgBoxAssert(std::string(_Name) + "이미지가 존재하지 않습니다.");
		return;
	}

	InfoIndex = _InfoIndex;
}

void UImageRenderer::CreateAnimation(
	std::string_view _AnimationName,
	std::string_view _ImageName,
	int _Start,
	int _End,
	float _Inter,
	bool _Loop 
)
{
	UWindowImage* FindImage = UEngineResourcesManager::GetInst().FindImg(_ImageName);

	if (nullptr == FindImage)
	{
		MsgBoxAssert(std::string(_ImageName) + "이미지가 존재하지 않습니다.");
		return;
	}

	std::string UpperAniName = UEngineString::ToUpper(_AnimationName);

	if (true == AnimationInfos.contains(UpperAniName))
	{
		MsgBoxAssert(std::string(UpperAniName) + "라는 이름의 애니메이션이 이미 존재합니다.");
		return;
	}

	UAnimationInfo& Info = AnimationInfos[UpperAniName];
	Info.Image = FindImage;
	Info.CurFrame = 0;
	Info.Start = _Start;
	Info.End = _End;
	Info.CurTime = 0.0f;
	Info.Loop = _Loop;

	int Size = Info.End - Info.Start;
	Info.Times.reserve(Size);
	Info.Indexs.reserve(Size);
	for (int i = _Start; i <= _End; i++)
	{
		Info.Times.push_back(_Inter);
	}

	for (int i = _Start; i <= _End; i++)
	{
		Info.Indexs.push_back(i);
	}
}

void UImageRenderer::ChangeAnimation(std::string_view _AnimationName)
{
	std::string UpperAniName = UEngineString::ToUpper(_AnimationName);

	if (false == AnimationInfos.contains(UpperAniName))
	{
		MsgBoxAssert(std::string(UpperAniName) + "라는 이름의 애니메이션이 존재하지 않습니다.");
		return;
	}

	UAnimationInfo& Info = AnimationInfos[UpperAniName];
	CurAnimation = &Info;
	CurAnimation->CurFrame = 0;
	CurAnimation->CurTime = CurAnimation->Times[0];
}

void UImageRenderer::AnimationReset()
{
	CurAnimation = nullptr;
}