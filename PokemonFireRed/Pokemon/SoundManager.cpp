#include "SoundManager.h"
#include "PokemonMath.h"

UEngineSoundPlayer USoundManager::BgmPlayer;
UEngineSoundPlayer USoundManager::SEPlayer;

USoundManager::EState USoundManager::State = USoundManager::EState::None;
float USoundManager::CurVolume = 1.0f;
float USoundManager::StartVolume = 0.0f;
float USoundManager::TargetVolume = 0.0f;
float USoundManager::FadeTime = 0.0f;
float USoundManager::FadeTimer = 0.0f;
float USoundManager::MuteTimer = 0.0f;

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

void USoundManager::PlaySE(std::string_view _SEName, float _MuteTime)
{
	SEPlayer = UEngineSound::SoundPlay(_SEName);
	SEPlayer.On();

	if (_MuteTime >= 0.0f)
	{
		State = EState::Mute;
		MuteTimer = _MuteTime;
		PauseBgm();
	}
}

void USoundManager::FadeBgm(float _TargetVolume, float _FadeTime)
{
	StartVolume = CurVolume;
	TargetVolume = _TargetVolume;
	FadeTime = _FadeTime;

	State = EState::Fade;
	FadeTimer = FadeTime;
}

void USoundManager::Tick(float _DeltaTime)
{
	if (State == EState::Fade)
	{
		FadeTimer -= _DeltaTime;
		float NewVolume = UPokemonMath::Lerp(TargetVolume, StartVolume, FadeTimer / FadeTime);
		SetBgmVolume(NewVolume);


		if (FadeTimer <= 0.0f)
		{
			State = EState::None;
		}
	}
	else if (State == EState::Mute)
	{
		MuteTimer -= _DeltaTime;

		if (MuteTimer <= 0.0f)
		{
			State = EState::None;
			ResumeBgm();
		}
	}

}