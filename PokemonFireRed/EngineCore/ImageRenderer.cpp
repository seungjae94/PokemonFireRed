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
	
	Renderers[GetOrder()].remove(this);

	UTickObject::SetOrder(_Order);

	Renderers[GetOrder()].push_back(this);
}

int UAnimationInfo::Update(float _DeltaTime)
{
	if (false == Loop && true == IsEnd)
	{
		return Indexs[CurFrame];
	}

	IsEnd = false;
	CurTime -= _DeltaTime;

	if (0.0f >= CurTime)
	{
		CurTime = Times[CurFrame];
		++CurFrame;

		if (1 == Indexs.size())
		{
			IsEnd = true;
		}

		if (false == Loop && Indexs.size() <= CurFrame)
		{
			IsEnd = true;
		}
	}

	if (Indexs.size() <= CurFrame)
	{
		if (1 < Indexs.size())
		{
			IsEnd = true;
		}
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
	if (false == Text.empty())
	{
		TextRender(_DeltaTime);
	}
	else {
		ImageRender(_DeltaTime);
	}

}

void UImageRenderer::BeginPlay()
{
	USceneComponent::BeginPlay();
}

void UImageRenderer::Tick(float _DeltaTime)
{
	USceneComponent::Tick(_DeltaTime);

	if (nullptr != CurAnimation)
	{
		Image = CurAnimation->Image;
		InfoIndex = CurAnimation->Update(_DeltaTime);
	}
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
	bool _Loop /*= true*/
)
{
	std::vector<int> Indexs;
	int Size = _End - _Start;

	for (int i = _Start; i <= _End; i++)
	{
		Indexs.push_back(i);
	}

	CreateAnimation(_AnimationName, _ImageName, Indexs, _Inter, _Loop);

}

void UImageRenderer::CreateAnimation(
	std::string_view _AnimationName,
	std::string_view _ImageName,
	std::vector<int> _Indexs,
	float _Inter,
	bool _Loop
)
{

	std::vector<float> Inters;

	int Size = static_cast<int>(_Indexs.size());
	Inters.reserve(Size);
	for (int i = 0; i <= Size; i++)
	{
		Inters.push_back(_Inter);
	}

	CreateAnimation(_AnimationName, _ImageName, _Indexs, Inters, _Loop);
}

void UImageRenderer::CreateAnimation(
	std::string_view _AnimationName,
	std::string_view _ImageName,
	std::vector<int> _Indexs,
	std::vector<float> _Inters,
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
	Info.Name = UpperAniName;
	Info.Image = FindImage;
	Info.CurFrame = 0;
	Info.CurTime = 0.0f;
	Info.Loop = _Loop;
	Info.Times = _Inters;
	Info.Indexs = _Indexs;
}

void UImageRenderer::ChangeAnimation(std::string_view _AnimationName, bool _IsForce /*= false*/, int _StartIndex/* = 0*/, float _Time /*= -1.0f*/)
{
	std::string UpperAniName = UEngineString::ToUpper(_AnimationName);

	if (false == AnimationInfos.contains(UpperAniName))
	{
		MsgBoxAssert(std::string(UpperAniName) + "라는 이름의 애니메이션이 존재하지 않습니다.");
		return;
	}

	// _IsForce가 false일 경우 동일한 애니메이션은 다시 재생하지 않는다.
	if (false == _IsForce && nullptr != CurAnimation && CurAnimation->Name == UpperAniName)
	{
		return;
	}

	UAnimationInfo& Info = AnimationInfos[UpperAniName];
	CurAnimation = &Info;
	CurAnimation->CurFrame = _StartIndex;
	CurAnimation->CurTime = CurAnimation->Times[_StartIndex];
	if (0.0f < _Time)
	{
		CurAnimation->CurTime = _Time;
	}
	CurAnimation->IsEnd = false;
}

void UImageRenderer::AnimationReset()
{
	CurAnimation = nullptr;
}

FTransform UImageRenderer::GetRenderTransForm()
{
	FTransform RendererTrans = GetActorBaseTransform();

	if (true == CameraEffect)
	{
		AActor* Actor = GetOwner();
		ULevel* World = Actor->GetWorld();
		FVector CameraPos = World->GetCameraPos();
		CameraPos *= CameraRatio;
		RendererTrans.AddPosition(-CameraPos);
	}

	return RendererTrans;
}

void UImageRenderer::TextRender(float _DeltaTime)
{
	FTransform RendererTrans = GetRenderTransForm();

	switch (TextEffect)
	{
	case 1:
		GEngine->MainWindow.GetBackBufferImage()->TextCopy(Text, Font, Size, RendererTrans, TextColor, TextColor2);
		break;
	case 2:
		GEngine->MainWindow.GetBackBufferImage()->TextCopyBold(Text, Font, Size, RendererTrans, TextColor);
		break;
	default:
		GEngine->MainWindow.GetBackBufferImage()->TextCopy(Text, Font, Size, RendererTrans, TextColor);
		break;
	}
}

void UImageRenderer::ImageRender(float _DeltaTime)
{

	if (nullptr == Image)
	{
		MsgBoxAssert("이미지가 존재하지 않는 랜더러 입니다");
	}

	FTransform RendererTrans = GetRenderTransForm();

	EWIndowImageType ImageType = Image->GetImageType();

	const UImageInfo& Info = Image->ImageInfo(InfoIndex);

	switch (SortType)
	{
	case EImageSortType::Left:
	{
		const UImageInfo& Info = Image->ImageInfo(InfoIndex);
		RendererTrans.AddPosition({ RendererTrans.GetScale().hX() , 0.0f });
		break;
	}
	default:
		break;
	}

	if (true == AutoImageScaleValue)
	{
		RendererTrans.SetScale(Info.CuttingTrans.GetScale() * AutoImageScaleRatio);
	}

	switch (ImageType)
	{
	case EWIndowImageType::IMG_BMP:
		GEngine->MainWindow.GetBackBufferImage()->TransCopy(Image, RendererTrans, InfoIndex, TransColor);
		break;
	case EWIndowImageType::IMG_PNG:
		if (0.0f == Angle)
		{
			GEngine->MainWindow.GetBackBufferImage()->AlphaCopy(Image, RendererTrans, InfoIndex, TransColor);
		}
		else
		{
			GEngine->MainWindow.GetBackBufferImage()->PlgCopy(Image, RendererTrans, InfoIndex, Angle * UEngineMath::DToR);
		}
		break;
	default:
		MsgBoxAssert("투명처리가 불가능한 이미지 입니다.");
		break;
	}
}