#pragma once
#include <EngineBase/EngineDirectory.h>
#include <EnginePlatform/EngineSound.h>
#include <EngineCore/ImageRenderer.h>
#include <EngineCore/EngineResourcesManager.h>
#include "Global.h"
#include "Canvas.h"

class ATitleLevelManager : public ACanvas
{
public:
	// constructor destructor
	ATitleLevelManager();
	~ATitleLevelManager();

	// delete Function
	ATitleLevelManager(const ATitleLevelManager& _Other) = delete;
	ATitleLevelManager(ATitleLevelManager&& _Other) noexcept = delete;
	ATitleLevelManager& operator=(const ATitleLevelManager& _Other) = delete;
	ATitleLevelManager& operator=(ATitleLevelManager&& _Other) noexcept = delete;

	void SetCurDir(UEngineDirectory* _CurDir)
	{
		CurDir = _CurDir;
		CurDir->MoveToSearchChild("Resources");
		CurDir->Move("TitleLevel");

		// 첫 번째 이미지 로드
		std::string PathName = GetPathName();
		UEngineResourcesManager::GetInst().LoadImg(CurDir->AppendPath(PathName));
		VideoImage->SetImage(GetImageName());
		VideoImage->SetTransColor(Color8Bit::White);
	}

protected:
private:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	AImageElement* VideoImage = nullptr;
	UEngineDirectory* CurDir = nullptr;

	float Interval = 1 / 60.0f;
	float Timer = Interval;

	const int ImageCount = 4331;
	int VideoNo = 0;
	int ImageIndex = 0;
	int VideoStartIndex[5] = { 0, 187, 1447, 1709, 4151 };

	void Video0Logic(float _DeltaTime);
	void Video1Logic(float _DeltaTime);
	void Video2Logic(float _DeltaTime);
	void Video3Logic(float _DeltaTime);
	void Video4Logic(float _DeltaTime);

	std::string GetIndexName()
	{
		std::string IndexName = "";
		if (ImageIndex < 1000)
		{
			IndexName += "0";
		}
		if (ImageIndex < 100)
		{
			IndexName += "0";
		}
		if (ImageIndex < 10)
		{
			IndexName += "0";
		}
		IndexName += std::to_string(ImageIndex);
		return IndexName;
	}

	std::string GetImageName()
	{
		return std::string("Intro") + GetIndexName() + ".bmp";
	}

	std::string GetPathName()
	{
		return std::string("Video")
			+ std::to_string(VideoNo) + "/" + GetImageName();
	}

	void PlayNextPart();
	void PlayNextVideo();
	void Play();

	// 사운드 재생
	bool IsFirstTick = true;

	// 레벨 종료 마킹
	bool LevelEndMark = false;
};

