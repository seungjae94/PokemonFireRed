#pragma once
#include <EngineBase/EngineDirectory.h>
#include <EngineCore/EngineCore.h>

// Ό³Έν :
class UPokemonCore : public UEngineCore
{
public:
	// constructor destructor
	UPokemonCore();
	~UPokemonCore();

	// delete Function
	UPokemonCore(const UPokemonCore& _Other) = delete;
	UPokemonCore(UPokemonCore&& _Other) noexcept = delete;
	UPokemonCore& operator=(const UPokemonCore& _Other) = delete;
	UPokemonCore& operator=(UPokemonCore&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
private:
	void LoadUIResources();
	void LoadPokemonResources();
	void LoadSounds();
	UEngineDirectory CurDir;
};


