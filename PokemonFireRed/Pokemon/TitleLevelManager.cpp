#include "TitleLevelManager.h"
#include <EnginePlatform/EngineSound.h>
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
		UEngineSound::SoundPlay(RN::BgmTitleBattle);
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
		UEngineSound::SoundPlay(RN::BgmTitleScreen);
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
	}

	Play();
}

void ATitleLevelManager::PlayNextVideo()
{
	VideoNo = (VideoNo + 1) % 5;
	ImageIndex = VideoStartIndex[VideoNo];

	Play();
}

void ATitleLevelManager::Play()
{
	std::string ImageName = GetImageName();

	// ���� ������ �ε���� ���� ��� �ε��Ѵ�.
	if (false == ImageLoaded[ImageIndex])
	{
		UEngineResourcesManager::GetInst().LoadImg(CurDir->AppendPath(GetPathName()));
		ImageLoaded[ImageIndex] = true;
	}

	Renderer->SetImage(ImageName);
	Timer = Interval;
}
