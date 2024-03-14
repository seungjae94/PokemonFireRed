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
		Fade
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

	static void PlaySE(std::string_view _SEName);


protected:

private:
	static EState FadeState;
	static float CurVolume;
	static float StartVolume;
	static float TargetVolume;
	static float FadeTime;
	static float Timer;
	static void Tick(float _DeltaTime);

	static UEngineSoundPlayer BgmPlayer;
	static UEngineSoundPlayer SEPlayer;
};

