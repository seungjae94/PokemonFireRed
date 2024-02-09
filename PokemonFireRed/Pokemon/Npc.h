#pragma once
#include "EventActor.h"

// Ό³Έν :
class ANpc : public AEventActor
{
public:
	// constructor destructor
	ANpc();
	~ANpc();

	// delete Function
	ANpc(const ANpc& _Other) = delete;
	ANpc(ANpc&& _Other) noexcept = delete;
	ANpc& operator=(const ANpc& _Other) = delete;
	ANpc& operator=(ANpc&& _Other) noexcept = delete;

protected:

private:
	UImageRenderer* Renderer = nullptr;
	std::vector<std::string> Dialogues;
};

