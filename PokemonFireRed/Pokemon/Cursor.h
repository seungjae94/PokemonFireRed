#pragma once
#include "UIElement.h"
#include "PokemonMath.h"

class ACursor : public AUIElement
{
	friend class APage;
public:
	// constructor destructor
	ACursor();
	~ACursor();

	// delete Function
	ACursor(const ACursor& _Other) = delete;
	ACursor(ACursor&& _Other) noexcept = delete;
	ACursor& operator=(const ACursor& _Other) = delete;
	ACursor& operator=(ACursor&& _Other) noexcept = delete;

	int GetCursor() const
	{
		return Cursor;
	}

	void SetCursor(int _Cursor)
	{
		if (nullptr == Renderer)
		{
			Renderer = CreateImageRenderer(ERenderingOrder::Text);
			Renderer->CameraEffectOff();
			Renderer->SetImage(RN::BlackCursor);
			FVector RenderScale = UPokemonUtil::GetRenderScale(Renderer);
			Renderer->SetTransform({ RenderScale.Half2D(), RenderScale });
		}

		Cursor = _Cursor;
	}

	void SetOptionCount(int _OptionCount)
	{
		OptionCount = _OptionCount;
	}

	void IncCursor()
	{
		Cursor = UPokemonMath::Mod(Cursor + 1, OptionCount);
	}

	void DecCursor()
	{
		Cursor = UPokemonMath::Mod(Cursor - 1, OptionCount);
	}

	void FollowContainer() override;

protected:

private:
	UImageRenderer* Renderer = nullptr;
	int Cursor = 0;
	int OptionCount = 0;
	int PixelGap = 16;
};

