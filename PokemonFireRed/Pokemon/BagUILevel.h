#pragma once
#include "PokemonLevel.h"

class UBagUILevel : public UPokemonLevel
{
private:
	enum class EState
	{
		None,
		End
	};
public:
	// constructor destructor
	UBagUILevel();
	~UBagUILevel();

	// delete Function
	UBagUILevel(const UBagUILevel& _Other) = delete;
	UBagUILevel(UBagUILevel&& _Other) noexcept = delete;
	UBagUILevel& operator=(const UBagUILevel& _Other) = delete;
	UBagUILevel& operator=(UBagUILevel&& _Other) noexcept = delete;

protected:

private:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;


};

