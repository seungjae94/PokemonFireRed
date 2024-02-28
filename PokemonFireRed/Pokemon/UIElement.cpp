#include "UIElement.h"
#include "PokemonUtil.h"

AUIElement::AUIElement()
{
}

AUIElement::~AUIElement()
{
}

void AUIElement::SetRelativePos(FVector _PivotRelativePos)
{
	RelativePos = _PivotRelativePos;
	FollowContainer();
}

void AUIElement::AddRelativePos(FVector _PivotRelativePos)
{
	SetRelativePos(RelativePos + _PivotRelativePos);
}

void AUIElement::FollowContainer()
{
	FTransform ContainerTrans = Container->GetTransform();
	float ContainerWidth = ContainerTrans.GetScale().X;
	float ContainerHeight = ContainerTrans.GetScale().Y;
	switch (PivotType)
	{
	case EPivotType::LeftTop:
		Pivot = ContainerTrans.LeftTop();
		break;
	case EPivotType::LeftBot:
		Pivot = ContainerTrans.LeftTop() + FVector(0.0f, ContainerHeight - Global::FloatPixelSize);
		break;
	case EPivotType::RightTop:
		Pivot = ContainerTrans.LeftTop() + FVector(ContainerWidth - Global::FloatPixelSize, 0.0f);
		break;
	case EPivotType::RightBot:
		Pivot = ContainerTrans.LeftTop() + FVector(ContainerWidth - Global::FloatPixelSize, ContainerHeight - Global::FloatPixelSize);
		break;
	case EPivotType::CenterTop:
	{
		int PixelContainerWidth = std::lround(ContainerWidth / 3.0f);
		Pivot = ContainerTrans.LeftTop() + UPokemonUtil::PixelVector(PixelContainerWidth / 2, 0);
	}
	break;
	case EPivotType::CenterBot:
	{
		int PixelContainerWidth = std::lround(ContainerWidth / 3.0f);
		Pivot = ContainerTrans.LeftTop() + UPokemonUtil::PixelVector(PixelContainerWidth / 2, 0) + FVector(0.0f, ContainerHeight - Global::FloatPixelSize);
	}
	break;
	default:
		break;
	}

	SetActorLocation(Pivot + RelativePos);
}
