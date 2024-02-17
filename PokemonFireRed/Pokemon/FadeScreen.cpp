#include "FadeScreen.h"

AFadeScreen::EFadeState AFadeScreen::State = AFadeScreen::EFadeState::Hide;
float AFadeScreen::FadeTime = 0.0f;
float AFadeScreen::CurFadeTime = 0.0f;

AFadeScreen::AFadeScreen()
{
}

AFadeScreen::~AFadeScreen()
{
}
