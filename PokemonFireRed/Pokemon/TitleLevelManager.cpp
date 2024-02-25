#include "TitleLevelManager.h"
#include <EngineCore/EngineCore.h>
#include "EventManager.h"
#include "PokemonUtil.h"
#include "PokemonInput.h"
#include "TitleLevel.h"

ATitleLevelManager::ATitleLevelManager()
{
}

ATitleLevelManager::~ATitleLevelManager()
{
}

void ATitleLevelManager::BeginPlay()
{
	Renderer = CreateImageRenderer(ERenderingOrder::LowerUI);
	Renderer->CameraEffectOff();
	Renderer->SetTransColor(Color8Bit::White);

	TitleBattleSoundPlayer = UEngineSound::SoundPlay(RN::BgmTitleBattle);
	TitleScreenSoundPlayer = UEngineSound::SoundPlay(RN::BgmTitleScreen);
	TitleBattleSoundPlayer.Off();
	TitleScreenSoundPlayer.Off();
}

void ATitleLevelManager::Tick(float _DeltaTime)
{
	Timer -= _DeltaTime;

	switch (VideoNo)
	{
	case 0:
		Video0Logic(_DeltaTime);
		break;
	case 1:
		Video1Logic(_DeltaTime);
		break;
	case 2:
		Video2Logic(_DeltaTime);
		break;
	case 3:
		Video3Logic(_DeltaTime);
		break;
	case 4:
		Video4Logic(_DeltaTime);
		break;
	default:
		break;
	}
}

void ATitleLevelManager::Video0Logic(float _DeltaTime)
{
	if (true == IsFirstTick)
	{
		TitleScreenSoundPlayer.Off();
		IsFirstTick = false;
	}

	if (Timer < 0.0f)
	{
		// 비디오 재생이 끝난 뒤에만 다음 영상으로 넘어간다.
		PlayNextPart();
	}
}

void ATitleLevelManager::Video1Logic(float _DeltaTime)
{
	if (true == IsFirstTick)
	{
		TitleBattleSoundPlayer.Replay();
		TitleBattleSoundPlayer.On();
		IsFirstTick = false;
	}

	if (true == UPokemonInput::IsAnykeyDown())
	{
		// 아무 키나 누르면 즉시 다음 영상으로 넘어간다.
		PlayNextVideo();
		return;
	}

	if (Timer < 0.0f)
	{
		// 비디오 재생이 끝난 경우 다음 파트로 넘어간다.
		PlayNextPart();
	}
}

void ATitleLevelManager::Video2Logic(float _DeltaTime)
{
	if (true == IsFirstTick)
	{
		TitleBattleSoundPlayer.Off();
		TitleScreenSoundPlayer.Replay();
		TitleScreenSoundPlayer.On();
		IsFirstTick = false;
	}

	if (true == UPokemonInput::IsAnykeyDown())
	{
		// 아무 키나 누르면 즉시 다음 영상으로 넘어간다.
		PlayNextVideo();
		return;
	}

	if (Timer < 0.0f)
	{
		// 비디오 재생이 끝난 경우 다음 파트로 넘어간다.
		PlayNextPart();
	}
}

void ATitleLevelManager::Video3Logic(float _DeltaTime)
{
	if (true == UPokemonInput::IsAnykeyDown())
	{
		// 아무 키나 누르면 타이틀 레벨을 종료한다.
		UEventManager::ChangeLevelFade(GetWorld(), Global::TutorialLevel, 1.0f, 1.0f);
		TitleScreenSoundPlayer.Off();
		return;
	}

	if (Timer < 0.0f)
	{
		// 비디오 재생이 끝난 경우 다음 파트로 넘어간다.
		PlayNextPart();
	}
}

void ATitleLevelManager::Video4Logic(float _DeltaTime)
{
	if (Timer < 0.0f)
	{
		// 비디오 재생이 끝난 뒤에만 다음 파트로 넘어간다.
		PlayNextPart();
	}
}

void ATitleLevelManager::PlayNextPart()
{
	ImageIndex = (ImageIndex + 1) % ImageCount;
	int NextVideoNo = (VideoNo + 1) % 5;

	if (ImageIndex == VideoStartIndex[NextVideoNo])
	{
		VideoNo = NextVideoNo;
		IsFirstTick = true;
	}

	Play();
}

void ATitleLevelManager::PlayNextVideo()
{
	VideoNo = (VideoNo + 1) % 5;
	ImageIndex = VideoStartIndex[VideoNo];
	IsFirstTick = true;
	Play();
}

void ATitleLevelManager::Play()
{
	std::string ImageName = GetImageName();
	UWindowImage* CurImage = Renderer->GetImage();
	UEngineResourcesManager::GetInst().UnloadImg(CurImage->GetName());
	UEngineResourcesManager::GetInst().LoadImg(CurDir->AppendPath(GetPathName()));
	Renderer->SetImage(ImageName);
	Timer = Interval;
}
