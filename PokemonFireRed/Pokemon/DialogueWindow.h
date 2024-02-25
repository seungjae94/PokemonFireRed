#pragma once
#include "Page.h"

class UImageRenderer;
class AText;
enum class EFontColor;

enum class EDialogueWindowState
{
	Hide,
	Show,
	End
};

class ADialogueWindow : public APage
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

	void SetDialogue(const std::vector<std::wstring>& _Dialogue, EFontColor _Color, int _LineSpace = 14, bool _IsSequential = false);
protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
private:
	EDialogueWindowState State = EDialogueWindowState::Hide;
	bool IsFirstTick = true;
	int Index = 0;
	std::vector<std::wstring> Dialogue;

	UImageRenderer* DialogueWindowRenderer = nullptr;
	AText* Text = nullptr;
};

