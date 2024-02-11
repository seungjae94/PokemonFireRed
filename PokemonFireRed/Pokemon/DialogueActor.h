#pragma once
#include "EventTrigger.h"

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

protected:
	void BeginPlay() override;
	void RegisterEvents() override;
private:
	UImageRenderer* Renderer = nullptr;
	std::vector<std::string> Dialogues;

	bool Event0();
	bool Event1();
};

