#include "ScrollBar.h"
#include "PokemonUtil.h"

AScrollBar::AScrollBar() 
{
}

AScrollBar::~AScrollBar() 
{
}

void AScrollBar::SetRelativePos(FVector _PivotRelativePos)
{
	RelativePos = _PivotRelativePos;
	FollowContainer();
}

void AScrollBar::FollowContainer()
{
	FVector Pivot;
	FTransform ContainerTrans = Container->GetTransform();
	float ContainerWidth = ContainerTrans.GetScale().X;
	float ContainerHeight = ContainerTrans.GetScale().Y;
	switch (PivotType)
	{
	case EPivotType::LeftTop:
		Pivot = ContainerTrans.LeftTop();
		break;
	case EPivotType::LeftBot:
		Pivot = ContainerTrans.LeftTop() + FVector(0.0f, ContainerHeight - 3.0f);
		break;
	case EPivotType::RightTop:
		Pivot = ContainerTrans.LeftTop() + FVector(ContainerWidth - 3.0f, 0.0f);
		break;
	case EPivotType::RightBot:
		Pivot = ContainerTrans.LeftTop() + FVector(ContainerWidth - 3.0f, ContainerHeight - 3.0f);
		break;
	case EPivotType::CenterTop:
		Pivot = Container->GetTransform().GetPosition();
		break;
	default:
		break;
	}

	SetActorLocation(Pivot + RelativePos);
}

void AScrollBar::DrawHp(int _Value)
{
	float Prop = _Value / static_cast<float>(MaxValue);
	float FWidth = Prop * (48 * Global::FloatPixelSize);

	std::string ImageName;
	if (Prop > 0.5f)
	{
		ImageName = Global::HpBarGreenImageName;
	}
	else if (Prop > 0.2f)
	{ 
		ImageName = Global::HpBarYellowImageName;
	}
	else
	{
		ImageName = Global::HpBarRedImageName;
	}

	int Width = std::lround(std::floor(FWidth));

	if (Renderers.size() > 0)
	{
		for (UImageRenderer* Renderer : Renderers)
		{
			Renderer->Destroy();
		}
		Renderers.clear();
	}

	for (int i = 0; i < Width; ++i)
	{
		UImageRenderer* Renderer = CreateImageRenderer(ERenderingOrder::Text);
		Renderer->SetImage(ImageName);

		FVector RenderScale = Global::ScrollBarRenderScale;
		FVector Pos = RenderScale.Half2D() + FVector::Right * static_cast<float>(i);
		Renderer->SetTransform({Pos, RenderScale});
	}
}

void AScrollBar::DrawExp(int _Value)
{
	float Prop = _Value / static_cast<float>(MaxValue);
	float FWidth = Prop * (64 * Global::FloatPixelSize);
	int Width = std::lround(std::floor(FWidth));

	std::string ImageName = Global::ExpBarImageName;

	if (Renderers.size() > 0)
	{
		for (UImageRenderer* Renderer : Renderers)
		{
			Renderer->Destroy();
		}
		Renderers.clear();
	}

	for (int i = 0; i < Width; ++i)
	{
		UImageRenderer* Renderer = CreateImageRenderer(ERenderingOrder::Text);
		Renderer->SetImage(ImageName);

		FVector RenderScale = Global::ScrollBarRenderScale;
		Renderer->SetTransform({ FVector::Right * static_cast<float>(i), RenderScale });
	}
}

