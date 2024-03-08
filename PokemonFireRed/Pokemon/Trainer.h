#pragma once
#include "EventTrigger.h"

class ATrainer : public AEventTrigger
{
public:
	// constructor destructor
	ATrainer();
	~ATrainer();

	// delete Function
	ATrainer(const ATrainer& _Other) = delete;
	ATrainer(ATrainer&& _Other) noexcept = delete;
	ATrainer& operator=(const ATrainer& _Other) = delete;
	ATrainer& operator=(ATrainer&& _Other) noexcept = delete;

	std::vector<UPokemon>* GetEntry()
	{
		return &Entry;
	}

	void AddPokemonToEntry(const UPokemon& _Pokemon)
	{
		Entry.push_back(_Pokemon);
	}

	void SetBattler(std::string_view _BattlerName, std::string_view _BattlerImageName)
	{
		BattlerName = _BattlerName;
		BattlerImageName = _BattlerImageName;
	}

	std::string GetBattlerName() const
	{
		return BattlerName;
	}

	std::string GetBattlerImageName() const
	{
		return BattlerImageName;
	}

protected:

private:
	std::vector<UPokemon> Entry;
	std::string BattlerName;
	std::string BattlerImageName;
};

