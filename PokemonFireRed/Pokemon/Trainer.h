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

	void SetTrainerWinMessage(const std::list<std::wstring>& _MessageSet)
	{
		TrainerWinMessage = _MessageSet;
	}

	void SetPlayerWinMessage(const std::list<std::wstring>& _MessageSet)
	{
		PlayerWinMessage = _MessageSet;
	}

	std::list<std::wstring> GetTrainerWinMessage() const
	{
		return TrainerWinMessage;
	}

	std::list<std::wstring> GetPlayerWinMessage() const
	{
		return PlayerWinMessage;
	}

	void SetAfterBattleTrigger(AEventTrigger* _Trigger)
	{
		AfterBattleTrigger = _Trigger;
	}

	AEventTrigger* GetAfterBattleTrigger()
	{
		return AfterBattleTrigger;
	}

	void SetGymLeader(bool _GymLeader)
	{
		GymLeader = _GymLeader;
	}

	bool IsGymLeader() const
	{
		return GymLeader;
	}

	void SetRegionGrass(bool _RegionGrass)
	{
		RegionGrass = _RegionGrass;
	}

	bool IsRegionGrass() const
	{
		return RegionGrass;
	}

protected:

private:
	std::vector<UPokemon> Entry;
	std::string BattlerName;
	std::string BattlerImageName;
	bool GymLeader = false;
	bool RegionGrass = true;

	std::list<std::wstring> TrainerWinMessage;
	std::list<std::wstring> PlayerWinMessage;

	// 배틀 후 트리거 할 이벤트
	AEventTrigger* AfterBattleTrigger = nullptr;
};

