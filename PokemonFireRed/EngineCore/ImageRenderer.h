#pragma once

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

protected:
	// UTickObject �Լ� 
	void BeginPlay() override;

private:

};

