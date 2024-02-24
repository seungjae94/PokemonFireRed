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
	float TotalWidth = 48 * Global::FloatPixelSize;		// 빈 영역을 포함한 전체 영역의 너비
	float FillWidth = Prop * TotalWidth;				// 빨강, 노랑, 초록 바가 채워져 있는 영역의 너비
	int TotalWidthInt = std::lround(TotalWidth);
	int FillWidthInt = std::lround(std::floor(FillWidth));

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

	if (Renderers.size() > 0)
	{
		for (UImageRenderer* Renderer : Renderers)
		{
			Renderer->Destroy();
		}
		Renderers.clear();
	}

	for (int i = 0; i < FillWidthInt; ++i)
	{
		UImageRenderer* Renderer = CreateImageRenderer(ERenderingOrder::Text);
		Renderer->SetImage(ImageName);

		FVector RenderScale = Global::ScrollBarRenderScale;
		FVector Pos = RenderScale.Half2D() + FVector::Right * static_cast<float>(i);
		Renderer->SetTransform({Pos, RenderScale});
		Renderers.push_back(Renderer);
	}
	for (int i = FillWidthInt; i < TotalWidthInt; ++i)
	{
		UImageRenderer* Renderer = CreateImageRenderer(ERenderingOrder::Text);
		Renderer->SetImage(RN::HpBarBlack);

		FVector RenderScale = Global::ScrollBarRenderScale;
		FVector Pos = RenderScale.Half2D() + FVector::Right * static_cast<float>(i);
		Renderer->SetTransform({ Pos, RenderScale });
		Renderers.push_back(Renderer);
	}
}

void AScrollBar::DrawExp(int _Value)
{
	float Prop = _Value / static_cast<float>(MaxValue);
	float TotalWidth = 64 * Global::FloatPixelSize;
	float FillWidth = Prop * TotalWidth;
	int TotalWidthInt = std::lround(TotalWidth);
	int FillWidthInt = std::lround(std::floor(FillWidth));

	std::string ImageName = RN::ExpBar;

	if (Renderers.size() > 0)
	{
		for (UImageRenderer* Renderer : Renderers)
		{
			Renderer->Destroy();
		}
		Renderers.clear();
	}

	for (int i = 0; i < FillWidthInt; ++i)
	{
		UImageRenderer* Renderer = CreateImageRenderer(ERenderingOrder::Text);
		Renderer->SetImage(ImageName);

		FVector RenderScale = Global::ScrollBarRenderScale;
		Renderer->SetTransform({ FVector::Right * static_cast<float>(i), RenderScale });
		Renderers.push_back(Renderer);
	}
	for (int i = FillWidthInt; i < TotalWidthInt; ++i)
	{
		UImageRenderer* Renderer = CreateImageRenderer(ERenderingOrder::Text);

		if (i % 2 == 0)
		{
			Renderer->SetImage(RN::ExpBarEmpty0);
		}
		else
		{
			Renderer->SetImage(RN::ExpBarEmpty1);
		}

		FVector RenderScale = Global::ScrollBarRenderScale;
		Renderer->SetTransform({ FVector::Right * static_cast<float>(i), RenderScale });
		Renderers.push_back(Renderer);
	}
}

