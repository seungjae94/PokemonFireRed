#pragma once
#include "UIElement.h"
#include "Pokemon.h"

enum class EImageElementType
{
	Image,
	PokemonMiniMove,
	PokemonMiniStop,
	PokemonFront,
	PokemonBack
};

class AImageElement : public AUIElement
{
public:
	// constructor destructor
	AImageElement();
	~AImageElement();

	// delete Function
	AImageElement(const AImageElement& _Other) = delete;
	AImageElement(AImageElement&& _Other) noexcept = delete;
	AImageElement& operator=(const AImageElement& _Other) = delete;
	AImageElement& operator=(AImageElement&& _Other) noexcept = delete;

	UWindowImage* GetImage()
	{
		return Renderer->GetImage();
	}

	void SetRenderingOrder(ERenderingOrder _Order) override;

	void SetImage(std::string_view _ImageName);
	void SetPokemon(EPokemonId _Id);
	void SetPokemon(const UPokemon* _Pokemon);
	void SetPokemon(const UPokemon& _Pokemon);

	void SetType(EImageElementType _Type)
	{
		Type = _Type;
	}

	void SetScaleFactor(float _Factor)
	{
		Renderer->SetScale(OriginalRenderScale * _Factor);
	}

	void SetAlpha(float _Alpha)
	{
		Renderer->SetAlpha(_Alpha);
	}

	void SetTransColor(Color8Bit _TransColor)
	{
		Renderer->SetTransColor(_TransColor);
	}

	FTransform GetUITransform() override;

	void CreateAnimation(std::string_view _AnimName, int _Start, int _End, float _Interval, bool _Loop);
	void CreateAnimation(std::string_view _AnimName, const std::vector<int>& _Indexs, std::vector<float> _Times, bool _Loop);
	void ChangeAnimation(std::string_view _AnimName, bool _IsForce = false);

	bool IsCurAnimationEnd();

protected:

private:
	UImageRenderer* Renderer = nullptr;
	std::string ImageName;
	bool MadePokemonAnimation = false;

	// 포켓몬 렌더링 기능
	EImageElementType Type = EImageElementType::Image;

	// ScaleFactor 기능
	FVector OriginalRenderScale;
};

