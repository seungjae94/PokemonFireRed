#pragma once
#include <EngineBase/EngineDirectory.h>
#include <EngineCore/Actor.h>
#include <EngineCore/ImageRenderer.h>

class ATitleLevelManager : public AActor
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
	}

protected:
private:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	UImageRenderer* Renderer = nullptr;
	UEngineDirectory* CurDir = nullptr;

	bool FirstVideoLoaded = false;
	int VideoNo = 0;
	int PartNo = 0;
	int VideoPartCount[5] = { 1, 13, 1, 24, 1 };
	int VideoLoadCount[5] = { 0, 0, 0, 0, 0 };
	int FirstPartImageCount[5] = { 187, 72, 262, 150, 180 };
	int LastPartImageCount[5] = { 187, 88, 262, 92, 180 };

	void PrepareFirstPlay();

	void Video0Logic();
	void Video1Logic();
	void Video2Logic();
	void Video3Logic(float _DeltaTime);
	void Video4Logic();

	std::string GetVideoName(int _No, int _PartNo)
	{
		std::string PartName = "";
		if (_PartNo < 10)
		{
			PartName = "0" + std::to_string(_PartNo);
		}
		else
		{
			PartName = std::to_string(_PartNo);
		}

		return "Video" + std::to_string(_No) + PartName;
	}

	void PlayNextPart();
	void PlayNextVideo();
	void Play();

	bool IsAnyKeyboardDown();
};

