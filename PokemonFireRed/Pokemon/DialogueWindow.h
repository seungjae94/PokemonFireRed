#pragma once
#include "UIElement.h"

class UImageRenderer;
class APokemonText;
enum class EFontColor;

enum class EDialogueWindowState
{
	Hide,
	Show,
	End
};

class ADialogueWindow : public AUIElement
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
	
	void SetState(EDialogueWindowState _State)
	{
		State = _State;
	}

	EDialogueWindowState GetState() const
	{
		return State;
	}

	void AllRenderersActiveOn();
	void AllRenderersActiveOff();

	void SetDialogue(const std::vector<std::wstring>& _Dialogue, EFontColor _Color, int _LineSpace = 14, bool _IsSequential = false);
protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
private:
	EDialogueWindowState State = EDialogueWindowState::Hide;
	int Index = 0;
	std::vector<std::wstring> Dialogue;

	UImageRenderer* DialogueWindowRenderer = nullptr;
	APokemonText* Text = nullptr;
};

