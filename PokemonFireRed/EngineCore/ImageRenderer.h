#pragma once

#include <EnginePlatform/WindowImage.h>
#include "SceneComponent.h"

// ���� :
class UImageRenderer : public USceneComponent
{
public:
	// constructor destructor
	UImageRenderer();
	~UImageRenderer();

	// delete Function
	UImageRenderer(const UImageRenderer& _Other) = delete;
	UImageRenderer(UImageRenderer&& _Other) noexcept = delete;
	UImageRenderer& operator=(const UImageRenderer& _Other) = delete;
	UImageRenderer& operator=(UImageRenderer&& _Other) noexcept = delete;

	void SetOrder(int _Order) override;

	// ������ �Լ�
	void Render(float _DeltaTime);

	// Setter
	void SetImage(std::string_view _Name);

	void SetTransform(const FTransform& _Value)
	{
		USceneComponent::SetTransform(_Value);
	}

	void SetImageCuttingTransform(const FTransform& _Value)
	{
		ImageCuttingTransform = _Value;
	}

protected:
	// UTickObject �Լ� 
	void BeginPlay() override;

private:
	UWindowImage* Image;
	FTransform ImageCuttingTransform;
};

