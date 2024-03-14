#pragma once
#include "EventTarget.h"

class ASurprise : public AEventTarget
{
public:
	// constructor destructor
	ASurprise();
	~ASurprise();

	// delete Function
	ASurprise(const ASurprise& _Other) = delete;
	ASurprise(ASurprise&& _Other) noexcept = delete;
	ASurprise& operator=(const ASurprise& _Other) = delete;
	ASurprise& operator=(ASurprise&& _Other) noexcept = delete;

	void Play();

	bool IsEnd() const;

protected:

private:
	void BeginPlay() override;
	UImageRenderer* Renderer = nullptr;
};

