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
		// ���� ����� ���� �ڿ��� ���� �������� �Ѿ��.
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
		// �ƹ� Ű�� ������ ��� ���� �������� �Ѿ��.
		PlayNextVideo();
		return;
	}

	if (Timer < 0.0f)
	{
		// ���� ����� ���� ��� ���� ��Ʈ�� �Ѿ��.
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
		// �ƹ� Ű�� ������ ��� ���� �������� �Ѿ��.
		PlayNextVideo();
		return;
	}

	if (Timer < 0.0f)
	{
		// ���� ����� ���� ��� ���� ��Ʈ�� �Ѿ��.
		PlayNextPart();
	}
}

void ATitleLevelManager::Video3Logic(float _DeltaTime)
{
	if (true == UPokemonInput::IsAnykeyDown())
	{
		// �ƹ� Ű�� ������ Ÿ��Ʋ ������ �����Ѵ�.
		UEventManager::ChangeLevelFade(GetWorld(), Global::TutorialLevel, 1.0f, 1.0f);
		TitleScreenSoundPlayer.Off();
		return;
	}

	if (Timer < 0.0f)
	{
		// ���� ����� ���� ��� ���� ��Ʈ�� �Ѿ��.
		PlayNextPart();
	}
}

void ATitleLevelManager::Video4Logic(float _DeltaTime)
{
	if (Timer < 0.0f)
	{
		// ���� ����� ���� �ڿ��� ���� ��Ʈ�� �Ѿ��.
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
