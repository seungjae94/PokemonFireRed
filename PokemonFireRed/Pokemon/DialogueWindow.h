#pragma once
#include <EngineCore/Actor.h>

class UImageRenderer;
class APokemonText;
enum class EFontColor;

class ADialogueWindow : public AActor
{
public:
	// constructor destructor
	ADialogueWindow();
	~ADialogueWindow();

	// delete Function
	ADialogueWindow(const ADialogueWindow& _Other) = delete;
	ADialogueWindow(ADialogueWindow&& _Other) noexcept = delete;
	ADialogueWindow& operator=(const ADialogueWindow& _Other) = delete;
	ADialogueWindow& operator=(ADialogueWindow&& _Other) noexcept = delete;
	
	void AllRenderersActiveOn();
	void AllRenderersActiveOff();

	void SetDialogues(const std::vector<std::wstring>& _Dialogues, EFontColor _Color);

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
private:
	UImageRenderer* DialogueWindowRenderer = nullptr;
	APokemonText* Text = nullptr;
};

