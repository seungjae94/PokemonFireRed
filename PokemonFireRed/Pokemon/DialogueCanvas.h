#pragma once
#include "Canvas.h"

class UImageRenderer;
class AText;
enum class EFontColor;

enum class EDialogueCanvasState
{
	Hide,
	Show,
	End
};

class ADialogueCanvas : public ACanvas
{
public:
	// constructor destructor
	ADialogueCanvas();
	~ADialogueCanvas();

	// delete Function
	ADialogueCanvas(const ADialogueCanvas& _Other) = delete;
	ADialogueCanvas(ADialogueCanvas&& _Other) noexcept = delete;
	ADialogueCanvas& operator=(const ADialogueCanvas& _Other) = delete;
	ADialogueCanvas& operator=(ADialogueCanvas&& _Other) noexcept = delete;
	
	void SetState(EDialogueCanvasState _State)
	{
		State = _State;
	}

	EDialogueCanvasState GetState() const
	{
		return State;
	}

	void Open(const std::vector<std::wstring>& _Dialogue, EFontColor _Color, int _LineSpace = 14);
protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
private:
	EDialogueCanvasState State = EDialogueCanvasState::Hide;
	bool IsFirstTick = true;
	int Index = 0;
	std::vector<std::wstring> Dialogue;

	AImageElement* MsgBox = nullptr;
	AText* Text = nullptr;
};

