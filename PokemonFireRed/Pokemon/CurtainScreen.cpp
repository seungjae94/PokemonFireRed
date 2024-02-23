#include "CurtainScreen.h"
#include "Global.h"
#include "PokemonMath.h"

ACurtainScreen::ACurtainScreen()
{
}

ACurtainScreen::~ACurtainScreen()
{
}

void ACurtainScreen::FadeIn(float _FadeTime)
{
	AFadeScreen::FadeIn(_FadeTime);
}

void ACurtainScreen::FadeOut(float _FadeTime)
{
	AFadeScreen::FadeOut(_FadeTime);
}

void ACurtainScreen::BeginPlay()
{
	AActor::BeginPlay();

	LeftRenderer = CreateImageRenderer(ERenderingOrder::Fade);
	LeftRenderer->CameraEffectOff();
	LeftRenderer->SetImage("BlackScreen.png");
	LeftRenderer->SetTransform({ Global::HalfScreen - FVector(Global::FloatScreenX, 0.0f), Global::Screen});
	LeftRenderer->SetActive(false);

	RightRenderer = CreateImageRenderer(ERenderingOrder::Fade);
	RightRenderer->CameraEffectOff();
	RightRenderer->SetImage("BlackScreen.png");
	RightRenderer->SetTransform({ Global::HalfScreen + FVector(Global::FloatScreenX, 0.0f), Global::Screen });
	RightRenderer->SetActive(false);
}

void ACurtainScreen::Tick(float _DeltaTime)
{
	switch (State)
	{
	case AFadeScreen::EFadeState::Hide:
		AllRenderersActiveOff();
		break;
	case AFadeScreen::EFadeState::FadeIn:
		FadeInTick(_DeltaTime);
		break;
	case AFadeScreen::EFadeState::FadeOut:
		FadeOutTick(_DeltaTime);
		break;
	case AFadeScreen::EFadeState::Show:
		AllRenderersActiveOn();
		break;
	default:
		break;
	}
}

void ACurtainScreen::Sync(APage* _Other)
{
	AFadeScreen::Sync(_Other);

	ACurtainScreen* Other = dynamic_cast<ACurtainScreen*>(_Other);

	if (nullptr == Other)
	{
		MsgBoxAssert("다운 캐스팅 실패. ACurtainScreen::Sync 함수에서 _Other(" + _Other->GetName()  + ")가 커튼 스크린이 아닙니다. ");
		return;
	}

	LeftRenderer->SetPosition(Other->LeftRenderer->GetTransform().GetPosition());
	RightRenderer->SetPosition(Other->RightRenderer->GetTransform().GetPosition());
}

void ACurtainScreen::FadeInTick(float _DeltaTime)
{
	if (CurFadeTime <= 0.0f)
	{
		State = EFadeState::Hide;
		return;
	}

	CurFadeTime -= _DeltaTime;

	float t = (FadeTime - CurFadeTime) / FadeTime;

	FVector LeftFrom = Global::HalfScreen - FVector(Global::FloatHalfScreenX, 0.0f);
	FVector LeftTo = Global::HalfScreen - FVector(Global::FloatScreenX, 0.0f);
	FVector RightFrom = Global::HalfScreen + FVector(Global::FloatHalfScreenX, 0.0f);
	FVector RightTo = Global::HalfScreen + FVector(Global::FloatScreenX, 0.0f);

	LeftRenderer->SetPosition(UPokemonMath::Lerp(LeftFrom, LeftTo, t));
	RightRenderer->SetPosition(UPokemonMath::Lerp(RightFrom, RightTo, t));
}

void ACurtainScreen::FadeOutTick(float _DeltaTime)
{
	if (CurFadeTime <= 0.0f)
	{
		State = EFadeState::Show;
		return;
	}

	CurFadeTime -= _DeltaTime;
	
	float t = (FadeTime - CurFadeTime) / FadeTime;

	FVector LeftFrom = Global::HalfScreen - FVector(Global::FloatScreenX, 0.0f);
	FVector LeftTo = Global::HalfScreen - FVector(Global::FloatHalfScreenX, 0.0f);
	FVector RightFrom = Global::HalfScreen + FVector(Global::FloatScreenX, 0.0f);
	FVector RightTo = Global::HalfScreen + FVector(Global::FloatHalfScreenX, 0.0f);

	LeftRenderer->SetPosition(UPokemonMath::Lerp(LeftFrom, LeftTo, t));
	RightRenderer->SetPosition(UPokemonMath::Lerp(RightFrom, RightTo, t));
}

