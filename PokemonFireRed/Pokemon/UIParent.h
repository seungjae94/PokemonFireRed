#pragma once
#include <EngineCore/Actor.h>

class AUIElement;

class AUIParent : public AActor
{
public:
	// constructor destructor
	AUIParent();
	~AUIParent();

	// delete Function
	AUIParent(const AUIParent& _Other) = delete;
	AUIParent(AUIParent&& _Other) noexcept = delete;
	AUIParent& operator=(const AUIParent& _Other) = delete;
	AUIParent& operator=(AUIParent&& _Other) noexcept = delete;

	void AppendChild(AUIElement* _Element);
	void SetActive(bool _Active, float _ActiveTime = 0.0f) override;

protected:
	std::list<AUIElement*> Children;

	void Tick(float _DeltaTime) override;
private:
};

