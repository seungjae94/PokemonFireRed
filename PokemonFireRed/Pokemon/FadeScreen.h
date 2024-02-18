#pragma once
#include "UIElement.h"

class AFadeScreen : public AUIElement
{
public:
	// constructor destructor
	AFadeScreen();
	~AFadeScreen();

	// delete Function
	AFadeScreen(const AFadeScreen& _Other) = delete;
	AFadeScreen(AFadeScreen&& _Other) noexcept = delete;
	AFadeScreen& operator=(const AFadeScreen& _Other) = delete;
	AFadeScreen& operator=(AFadeScreen&& _Other) noexcept = delete;

	void FadeIn(float _FadeTime)
	{
		FadeTime = _FadeTime;
		CurFadeTime = FadeTime;
		Renderer->SetActive(true);
		Renderer->SetAlpha(1.0f);
		State = EFadeState::FadeIn;
	}
	
	void FadeOut(float _FadeTime)
	{
		FadeTime = _FadeTime;
		CurFadeTime = FadeTime;
		Renderer->SetActive(true);
		Renderer->SetAlpha(0.0f);
		State = EFadeState::FadeOut;
	}

	void Hide()
	{
		State = EFadeState::Hide;
		SetActive(false);
		AllRenderersActiveOff();
	}

	void Show()
	{
		State = EFadeState::Show;
		SetActive(true);
		AllRenderersActiveOn();
	}

	void Sync(AUIElement* _Other) override
	{
		AFadeScreen* Other = dynamic_cast<AFadeScreen*>(_Other);
		
		if (nullptr == Other)
		{
			MsgBoxAssert(_Other->GetName() + 
				"은 FadeScreen이 아닙니다. Sync 함수에서 다운 캐스팅이 실패했습니다.");
			return;
		}

		State = Other->State;
		FadeTime = Other->FadeTime;
		CurFadeTime = Other->CurFadeTime;
		Renderer->SetActive(Other->Renderer->IsActive());
	}

protected:
	enum class EFadeState
	{
		Hide,
		FadeIn,
		FadeOut,
		Show,
	};

	EFadeState State = AFadeScreen::EFadeState::Hide;
	float FadeTime = 0.0f;
	float CurFadeTime = 0.0f;
	UImageRenderer* Renderer = nullptr;
private:
};

