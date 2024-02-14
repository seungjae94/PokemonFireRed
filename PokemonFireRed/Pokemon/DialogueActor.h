#pragma once
#include "EventTrigger.h"
#include "PokemonText.h"

// Ό³Έν :
class ADialogueActor : public AEventTrigger
{
public:
	// constructor destructor
	ADialogueActor();
	~ADialogueActor();

	// delete Function
	ADialogueActor(const ADialogueActor& _Other) = delete;
	ADialogueActor(ADialogueActor&& _Other) noexcept = delete;
	ADialogueActor& operator=(const ADialogueActor& _Other) = delete;
	ADialogueActor& operator=(ADialogueActor&& _Other) noexcept = delete;

	void SetDialogue(const std::vector<std::wstring>& _Dialogue)
	{
		Dialogue = _Dialogue;
	}

	void SetTextColor(EFontColor _TextColor)
	{
		TextColor = _TextColor;
	}

protected:
	void BeginPlay() override;
	void RegisterEvents() override;
private:
	UImageRenderer* Renderer = nullptr;
	std::vector<std::wstring> Dialogue;
	EFontColor TextColor = EFontColor::Black;

	FTileVector BeforeChatDirection = FTileVector::Zero;

	bool Event0();
	bool Event1();
	bool Event2();
	bool Event3();
	bool Event4();
};

