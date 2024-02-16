#include "TitleLevelManager.h"
#include <EngineCore/EngineCore.h>
#include <EngineCore/EngineResourcesManager.h>
#include <EngineCore/ImageRenderer.h>
#include "EventManager.h"
#include "Global.h"
#include "PokemonUtil.h"

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

	Renderer->CreateAnimation(VideoName[0], VideoName[0], 0, 187 - 1, 1 / 60.0f, false);
	Renderer->CreateAnimation(VideoName[1], VideoName[1], 0, 1260 - 1, 1 / 60.0f, false);
	Renderer->CreateAnimation(VideoName[2], VideoName[2], 0, 262 - 1, 1 / 60.0f, false);
	Renderer->CreateAnimation(VideoName[3], VideoName[3], 0, 2442 - 1, 1 / 60.0f, false);
	Renderer->CreateAnimation(VideoName[4], VideoName[4], 0, 180 - 1, 1 / 60.0f, false);

	Renderer->SetImage(VideoName[0]);
	Renderer->SetTransform({ {0, 0}, Global::Screen });
	Renderer->ChangeAnimation(VideoName[0]);

	// �Ͼ� ��� ȭ��
	WhiteScreenRenderer = CreateImageRenderer(ERenderingOrder::UpperUI);
	WhiteScreenRenderer->CameraEffectOff();
	WhiteScreenRenderer->SetImage("WhiteScreen.png");
	WhiteScreenRenderer->SetTransColor(Color8Bit::WhiteA);
	WhiteScreenRenderer->SetAlpha(0.0f);
	WhiteScreenRenderer->SetTransform({ {0, 0}, Global::Screen });
}

void ATitleLevelManager::Tick(float _DeltaTime)
{
	if (true == IsEnd)
	{
		EndTime -= _DeltaTime;
		CurAlpha += _DeltaTime;
		WhiteScreenRenderer->SetAlpha(CurAlpha);

		if (EndTime <= 0.0f)
		{
			UEventManager::SetLevel(Global::TutorialLevel);
		}
		return;
	}

	switch (VideoIndex)
	{
	case 0:
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

void ATitleLevelManager::Video0Logic()
{
	if (true == Renderer->IsCurAnimationEnd())
	{
		// ���� ����� ���� �ڿ��� ���� �������� �Ѿ��.
		PlayNextVideo();
	}
}

void ATitleLevelManager::Video1Logic()
{
	if (true == IsAnyKeyboardDown() || true == Renderer->IsCurAnimationEnd())
	{
		// �ƹ� Ű�� ������ ��� ���� �������� �Ѿ��.
		// ���� ����� ���� ��쿡�� ���� �������� �Ѿ��.
		PlayNextVideo();
	}
}

void ATitleLevelManager::Video2Logic()
{
	if (true == IsAnyKeyboardDown() || true == Renderer->IsCurAnimationEnd())
	{
		// �ƹ� Ű�� ������ ��� ���� �������� �Ѿ��.
		// ���� ����� ���� ��쿡�� ���� �������� �Ѿ��.
		PlayNextVideo();
	}
}

void ATitleLevelManager::Video3Logic(float _DeltaTime)
{
	if (true == IsAnyKeyboardDown())
	{
		// �ƹ� Ű�� ������ Ÿ��Ʋ ������ �����Ѵ�.
		IsEnd = true;
		return;
	}

	if (true == Renderer->IsCurAnimationEnd())
	{
		// ���� ����� ���� ��� ���� �������� �Ѿ��.
		PlayNextVideo();
	}
}

void ATitleLevelManager::Video4Logic()
{
	if (true == Renderer->IsCurAnimationEnd())
	{
		// ���� ����� ���� �ڿ��� ���� �������� �Ѿ��.
		PlayNextVideo();
	}
}

void ATitleLevelManager::PlayNextVideo()
{
	VideoIndex = (VideoIndex + 1) % 5;
	Renderer->ChangeAnimation(VideoName[VideoIndex]);
}

bool ATitleLevelManager::IsAnyKeyboardDown()
{
	return true == UEngineInput::IsAnykeyDown() && false == UEngineInput::IsDown(VK_LBUTTON) && false == UEngineInput::IsDown(VK_RBUTTON);
}
