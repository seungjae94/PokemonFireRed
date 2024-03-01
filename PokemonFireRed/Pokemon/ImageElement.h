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

	void CreateAnimation(std::string_view _AnimName, int _Start, int _End, float _Interval, bool _Loop);
	void CreateAnimation(std::string_view _AnimName, const std::vector<int>& _Indexs, std::vector<float> _Times, bool _Loop);
	void ChangeAnimation(std::string_view _AnimName, bool _IsForce = false);


protected:

private:
	UImageRenderer* Renderer = nullptr;
	std::string ImageName;
};

