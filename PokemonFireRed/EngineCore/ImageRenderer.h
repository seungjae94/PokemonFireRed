#pragma once

#include "SceneComponent.h"

// 설명 :
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

	// 렌더링 함수
	void Render(float _DeltaTime);

protected:
	// UTickObject 함수 
	void BeginPlay() override;

private:

};

