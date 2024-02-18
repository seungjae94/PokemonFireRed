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

	virtual void FadeIn(float _FadeTime)
	{
		FadeTime = _FadeTime;
		CurFadeTime = FadeTime;

		SetActive(true);
		AllRenderersActiveOn();

		if (nullptr != Renderer)
		{
			Renderer->SetAlpha(1.0f);
		}

		State = EFadeState::FadeIn;
	}
	
	virtual void FadeOut(float _FadeTime)
	{
		FadeTime = _FadeTime;
		CurFadeTime = FadeTime;
		
		SetActive(true);
		AllRenderersActiveOn();

		if (nullptr != Renderer)
		{
			Renderer->SetAlpha(0.0f);
		}

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
				"�� FadeScreen�� �ƴմϴ�. Sync �Լ����� �ٿ� ĳ������ �����߽��ϴ�.");
			return;
		}

		State = Other->State;
		FadeTime = Other->FadeTime;
		CurFadeTime = Other->CurFadeTime;

		if (nullptr != Renderer)
		{
			Renderer->SetActive(Other->Renderer->IsActive());
		}
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

