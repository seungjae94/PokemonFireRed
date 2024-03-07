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

protected:

private:
	std::vector<UPokemon> Entry;
};

