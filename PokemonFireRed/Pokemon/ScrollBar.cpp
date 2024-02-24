#include "ScrollBar.h"
#include "PokemonUtil.h"

AScrollBar::AScrollBar() 
{
}

AScrollBar::~AScrollBar() 
{
}

void AScrollBar::DrawHp(int _Value)
{
	float Prop = _Value / static_cast<float>(MaxValue);
	float FWidth = Prop * (48 * Global::FloatPixelSize);

	std::string ImageName;
	if (Prop > 0.5f)
	{
		ImageName = RN::HpBarGreen;
	}
	else if (Prop > 0.2f)
	{ 
		ImageName = RN::HpBarYellow;
	}
	else
	{
		ImageName = RN::HpBarRed;
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

	std::string ImageName = RN::ExpBar;

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

