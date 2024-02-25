#pragma once
#include <EnginePlatform/EngineSound.h>

class USoundManager
{
public:
	// constructor destructor
	USoundManager();
	~USoundManager();

	// delete Function
	USoundManager(const USoundManager& _Other) = delete;
	USoundManager(USoundManager&& _Other) noexcept = delete;
	USoundManager& operator=(const USoundManager& _Other) = delete;
	USoundManager& operator=(USoundManager&& _Other) noexcept = delete;

	static void PlayBgm(std::string_view _BgmName)
	{
		BgmPlayer.Off();
		BgmPlayer = UEngineSound::SoundPlay(_BgmName);
		BgmPlayer.Replay();
		BgmPlayer.Loop(-1);
		BgmPlayer.On();
	}

	static void StopBgm()
	{
		BgmPlayer.Off();
		BgmPlayer.Replay();
	}

	static void PauseBgm()
	{
		BgmPlayer.Off();
	}

	static void ResumeBgm()
	{
		BgmPlayer.On();
	}

protected:

private:
	static UEngineSoundPlayer BgmPlayer;
};

