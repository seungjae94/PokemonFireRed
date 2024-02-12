#pragma once
#include <EngineCore/EngineCore.h>

// Ό³Έν :
class UMapEditorCore : public UEngineCore
{
public:
	// constructor destructor
	UMapEditorCore();
	~UMapEditorCore();

	// delete Function
	UMapEditorCore(const UMapEditorCore& _Other) = delete;
	UMapEditorCore(UMapEditorCore&& _Other) noexcept = delete;
	UMapEditorCore& operator=(const UMapEditorCore& _Other) = delete;
	UMapEditorCore& operator=(UMapEditorCore&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
private:

};

