#pragma once
#include <EnginePlatform/EngineSound.h>

class UEventManager;

class USoundManager
{
	friend UEventManager;
private:
	enum class EState
	{
		None,
		Fade,
		Mute,
	};
public:
	// constructor destructor
	USoundManager();
	~USoundManager();

	// delete Function
	USoundManager(const USoundManager& _Other) = delete;
	USoundManager(USoundManager&& _Other) noexcept = delete;
	USoundManager& operator=(const USoundManager& _Other) = delete;
	USoundManager& operator=(USoundManager&& _Other) noexcept = delete;

	static void PlayBgm(std::string_view _BgmName);

	static void StopBgm();

	static void PauseBgm();

	static void ResumeBgm();

	static void SetBgmVolume(float _Volume);

	static void FadeBgm(float _TargetVolume, float _FadeTime);

	static void PlaySE(std::string_view _SEName, float _MuteTime = -1.0f);

protected:

private:
	static EState State;
	static float CurVolume;
	static float StartVolume;
	static float TargetVolume;
	static float FadeTime;
	static float FadeTimer;
	static float MuteTimer;
	static void Tick(float _DeltaTime);

	static UEngineSoundPlayer BgmPlayer;
	static UEngineSoundPlayer SEPlayer;
};

