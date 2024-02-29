#pragma once
#include "UIElement.h"

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

	void SetImage(std::string_view _ImageName);

	FTransform GetUITransform() override;

protected:

private:
	UImageRenderer* Renderer = nullptr;
};

