#include "TitleLevelManager.h"
#include <EngineCore/EngineCore.h>
#include "EventManager.h"
#include "PokemonUtil.h"
#include "PokemonInput.h"
#include "TitleLevel.h"
#include "SoundManager.h"

ATitleLevelManager::ATitleLevelManager()
{
}

ATitleLevelManager::~ATitleLevelManager()
{
}

void ATitleLevelManager::BeginPlay()
{
	VideoImage = CreateImageElement(this, ERenderingOrder::UI0, EPivotType::LeftTop, 0, 0);
}

void ATitleLevelManager::Tick(float _DeltaTime)
{
	if (true == LevelEndMark)
	{
		return;
	}

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
		USoundManager::StopBgm();
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
		USoundManager::PlayBgm(RN::BgmTitleBattle);
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
		USoundManager::PlayBgm(RN::BgmTitleScreen);
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
		USoundManager::PlaySE(RN::TitleCharizardCry);
		UEventManager::FadeChangeLevel(Global::TutorialLevel, false, 0.5f, 2.0f, true, RN::BgmTutorial);
		LevelEndMark = true;
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
	UWindowImage* CurImage = VideoImage->GetImage();
	UEngineResourcesManager::GetInst().UnloadImg(CurImage->GetName());
	UEngineResourcesManager::GetInst().LoadImg(CurDir->AppendPath(GetPathName()));
	VideoImage->SetImage(ImageName);
	Timer = Interval;
}
