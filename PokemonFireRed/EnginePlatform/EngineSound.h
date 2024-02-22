#pragma once
#include <string>
#include <string_view>
#include <EngineBase\EnginePath.h>
#include <map>

#include "ThirdParty\FMOD\inc\fmod.hpp"

class UEngineSoundPlayer
{
	friend class UEngineSound;

public:
	void On()
	{
		Control->setPaused(false);
	}
	void Off()
	{
		Control->setPaused(true);
	}

	void Loop(int Count = -1)
	{
		Control->setLoopCount(Count);
	}


	void OnOffSwitch()
	{
		bool Check = false;
		Control->getPaused(&Check);

		if (true == Check)
		{
			Control->setPaused(false);
		}
		else {
			Control->setPaused(true);
		}
	}

	void SetLoop(int Count);
	void SetVolume(float _Volume);

private:
	// 사운드 재생에 관한 핸들(권한)
	FMOD::Channel* Control = nullptr;
};

class UEngineSound
{
	friend class ResControl;
private:
	static std::map<std::string, UEngineSound*> Resources;

public:
	// constrcutor destructor
	UEngineSound();
	~UEngineSound();

	// delete Function
	UEngineSound(const UEngineSound& _Other) = delete;
	UEngineSound(UEngineSound&& _Other) noexcept = delete;
	UEngineSound& operator=(const UEngineSound& _Other) = delete;
	UEngineSound& operator=(UEngineSound&& _Other) noexcept = delete;

	static UEngineSoundPlayer SoundPlay(std::string_view _Name);

	static void Load(std::string_view _Path)
	{
		UEnginePath NewPath = UEnginePath(std::filesystem::path(_Path));
		std::string FileName = NewPath.GetFileName();
		return Load(_Path, FileName);
	}

	static void Load(std::string_view _Path, std::string_view _Name);
	static void Update();

protected:
	void ResLoad(std::string_view _Path);

private:
	FMOD::Sound* SoundHandle = nullptr;

	static void ResourcesRelease();
};

