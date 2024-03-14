#include "SoundManager.h"
#include "PokemonMath.h"

UEngineSoundPlayer USoundManager::BgmPlayer;
UEngineSoundPlayer USoundManager::SEPlayer;

USoundManager::EState USoundManager::FadeState = USoundManager::EState::None;
float USoundManager::CurVolume = 1.0f;
float USoundManager::StartVolume = 0.0f;
float USoundManager::TargetVolume = 0.0f;
float USoundManager::FadeTime = 0.0f;
float USoundManager::Timer = 0.0f;

USoundManager::USoundManager()
{
}

USoundManager::~USoundManager()
{
}

void USoundManager::PlayBgm(std::string_view _BgmName)
{
	BgmPlayer.Off();
	BgmPlayer = UEngineSound::SoundPlay(_BgmName);
	BgmPlayer.Replay();
	BgmPlayer.Loop(-1);
	BgmPlayer.On();
}

void USoundManager::StopBgm()
{
	BgmPlayer.Off();
	BgmPlayer.Replay();
}

void USoundManager::PauseBgm()
{
	BgmPlayer.Off();
}

void USoundManager::ResumeBgm()
{
	BgmPlayer.On();
}

void USoundManager::SetBgmVolume(float _Volume)
{
	CurVolume = _Volume;
	BgmPlayer.SetVolume(_Volume);
}

void USoundManager::PlaySE(std::string_view _SEName)
{
	SEPlayer = UEngineSound::SoundPlay(_SEName);
	SEPlayer.On();
}

void USoundManager::FadeBgm(float _TargetVolume, float _FadeTime)
{
	StartVolume = CurVolume;
	TargetVolume = _TargetVolume;
	FadeTime = _FadeTime;

	FadeState = EState::Fade;
	Timer = FadeTime;
}

void USoundManager::Tick(float _DeltaTime)
{
	if (FadeState != EState::Fade)
	{
		return;
	}

	Timer -= _DeltaTime;
	float NewVolume = UPokemonMath::Lerp(TargetVolume, StartVolume, Timer / FadeTime);
	SetBgmVolume(NewVolume);


	if (Timer <= 0.0f)
	{
		FadeState = EState::None;
	}

}
