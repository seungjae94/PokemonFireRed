#pragma once
#include <EngineCore/Actor.h>

enum class EPivotType
{
	LeftTop,
	LeftBot,
	RightTop,
	RightBot,
	CenterTop,
	CenterBot,
};

class AUIElement : public AActor
{
public:
	// constructor destructor
	AUIElement();
	~AUIElement();

	// delete Function
	AUIElement(const AUIElement& _Other) = delete;
	AUIElement(AUIElement&& _Other) noexcept = delete;
	AUIElement& operator=(const AUIElement& _Other) = delete;
	AUIElement& operator=(AUIElement&& _Other) noexcept = delete;

	virtual void FollowContainer() {}

protected:

private:

};

