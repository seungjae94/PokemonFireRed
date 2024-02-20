#include "TitleLevelManager.h"
#include <EngineCore/EngineCore.h>
#include <EngineCore/EngineResourcesManager.h>
#include <EngineCore/ImageRenderer.h>
#include "EventManager.h"
#include "Global.h"
#include "PokemonUtil.h"
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
	switch (VideoNo)
	{
	case 0:
		if (false == FirstVideoLoaded)
		{
			PrepareFirstPlay();
		}

		Video0Logic();
		break;
	case 1:
		Video1Logic();
		break;
	case 2:
		Video2Logic();
		break;
	case 3:
		Video3Logic(_DeltaTime);
		break;
	case 4:
		Video4Logic();
		break;
	default:
		break;
	}
}

void ATitleLevelManager::PrepareFirstPlay()
{
	// ù ��° ��Ʈ�� ���� �ε�
	UEngineResourcesManager::GetInst().LoadFolder(CurDir->AppendPath("Video000"));
	++VideoLoadCount[0];

	Renderer->CreateAnimation("Video000", "Video000", 0, 187 - 1, 1 / 60.0f, false);
	Renderer->SetImage("Video000");
	Renderer->SetTransform({ {0, 0}, Global::Screen });
	Renderer->ChangeAnimation("Video000");

	FirstVideoLoaded = true;
}

void ATitleLevelManager::Video0Logic()
{
	if (true == Renderer->IsCurAnimationEnd())
	{
		// ���� ����� ���� �ڿ��� ���� �������� �Ѿ��.
		PlayNextPart();
	}
}

void ATitleLevelManager::Video1Logic()
{
	if (true == IsAnyKeyboardDown())
	{
		// �ƹ� Ű�� ������ ��� ���� �������� �Ѿ��.
		PlayNextVideo();
		return;
	}

	if (true == Renderer->IsCurAnimationEnd())
	{
		// ���� ����� ���� ��� ���� ��Ʈ�� �Ѿ��.
		PlayNextPart();
	}
}

void ATitleLevelManager::Video2Logic()
{
	if (true == IsAnyKeyboardDown())
	{
		// �ƹ� Ű�� ������ ��� ���� �������� �Ѿ��.
		PlayNextVideo();
		return;
	}

	if (true == Renderer->IsCurAnimationEnd())
	{
		// ���� ����� ���� ��� ���� ��Ʈ�� �Ѿ��.
		PlayNextPart();
	}
}

void ATitleLevelManager::Video3Logic(float _DeltaTime)
{
	if (true == IsAnyKeyboardDown())
	{
		// �ƹ� Ű�� ������ Ÿ��Ʋ ������ �����Ѵ�.
		UTitleLevel* ThisLevel = dynamic_cast<UTitleLevel*>(GetWorld());
		ThisLevel->LevelChange();
		return;
	}

	if (true == Renderer->IsCurAnimationEnd())
	{
		// ���� ����� ���� ��� ���� ��Ʈ�� �Ѿ��.
		PlayNextPart();
	}
}

void ATitleLevelManager::Video4Logic()
{
	if (true == Renderer->IsCurAnimationEnd())
	{
		// ���� ����� ���� �ڿ��� ���� ��Ʈ�� �Ѿ��.
		PlayNextPart();
	}
}

void ATitleLevelManager::PlayNextPart()
{
	++PartNo;
	if (PartNo >= VideoPartCount[VideoNo])
	{
		PartNo = 0;
		VideoNo = (VideoNo + 1) % 5;
	}

	Play();
}

void ATitleLevelManager::PlayNextVideo()
{
	PartNo = 0;
	VideoNo = (VideoNo + 1) % 5;
	
	Play();
}

void ATitleLevelManager::Play()
{
	std::string VideoName = GetVideoName(VideoNo, PartNo);

	// ���� ������ �ε���� ���� ���
	if (PartNo >= VideoLoadCount[VideoNo])
	{
		int ImageCount = 100;
		if (PartNo == 0)
		{
			ImageCount = FirstPartImageCount[VideoNo];
		}
		else if (PartNo == VideoPartCount[VideoNo] - 1)
		{
			ImageCount = LastPartImageCount[VideoNo];
		}

		UEngineResourcesManager::GetInst().LoadFolder(CurDir->AppendPath(VideoName));
		Renderer->CreateAnimation(VideoName, VideoName, 0, ImageCount - 1, 1 / 60.0f, false);
		++VideoLoadCount[VideoNo];
	}

	Renderer->ChangeAnimation(VideoName);
}



bool ATitleLevelManager::IsAnyKeyboardDown()
{
	return true == UEngineInput::IsAnykeyDown() && false == UEngineInput::IsDown(VK_LBUTTON) && false == UEngineInput::IsDown(VK_RBUTTON);
}
