#pragma once
#include "EventTrigger.h"
#include "EventTarget.h"

// Ό³Έν :
class ADialogueActor : public AEventTrigger, AEventTarget
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

protected:

private:
	UImageRenderer* Renderer = nullptr;
	std::vector<std::string> Dialogues;
};

