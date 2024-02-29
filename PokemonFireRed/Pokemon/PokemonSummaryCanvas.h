#pragma once
#include "Canvas.h"
#include "Text.h"
#include "PokemonElement.h"

enum class EPokemonSummaryPageState
{
	Info,
	Skills,
	Moves,
	MovesDetail,
};

class APokemonSummaryCanvas : public ACanvas
{
public:
	// constructor destructor
	APokemonSummaryCanvas();
	~APokemonSummaryCanvas();

	// delete Function
	APokemonSummaryCanvas(const APokemonSummaryCanvas& _Other) = delete;
	APokemonSummaryCanvas(APokemonSummaryCanvas&& _Other) noexcept = delete;
	APokemonSummaryCanvas& operator=(const APokemonSummaryCanvas& _Other) = delete;
	APokemonSummaryCanvas& operator=(APokemonSummaryCanvas&& _Other) noexcept = delete;

	void SetPokemon(UPokemon* _Pokemon)
	{
		Pokemon = _Pokemon;
	}

	void RefreshAll();
	void Reset();

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
private:
	// FSM
	EPokemonSummaryPageState State = EPokemonSummaryPageState::Info;
	void InfoTick(float _DeltaTime);
	void SkillsTick(float _DeltaTime);
	void MovesTick(float _DeltaTime);
	void MovesDetailTick(float _DeltaTime);

	// 데이터
	UPokemon* Pokemon = nullptr;

	// 최상위 요소
	AImageElement* Background = nullptr;
	AImageElement* CommonBox = nullptr;
	AImageElement* InfoBox = nullptr;
	AImageElement* SkillsBox = nullptr;
	AImageElement* MovesBox = nullptr;
	AImageElement* MovesDetailBox = nullptr;
	AImageElement* SmallCommonBox = nullptr;
	AImageElement* Nav = nullptr;

	// CommonBox 요소
	AText* NameText = nullptr;
	AText* LevelText = nullptr;
	AImageElement* GenderIcon = nullptr;
	APokemonElement* FrontImage = nullptr;

	// InfoBox 요소
	AText* PokedexNoText = nullptr;
	AText* SpeciesNameText = nullptr;
	std::vector<AImageElement*> TypeImages;
	AText* TrainerText = nullptr;
	AText* IdNoText = nullptr;
	AText* ItemText = nullptr;
	AText* TrainerMemo = nullptr;

	// SkillsBox 요소
	AText* HpText = nullptr;
	AText* AtkText = nullptr;
	AText* DefText = nullptr;
	AText* SpAtkText = nullptr;
	AText* SpDefText = nullptr;
	AText* SpeedText = nullptr;
	AText* AccExpText = nullptr;
	AText* NextLevelExpText = nullptr;
	AText* AbilityText = nullptr;
	AText* AbilityExplainText = nullptr;
	AScrollBar* HpBar = nullptr;
	AScrollBar* ExpBar = nullptr;

	// MovesBox 요소
	std::vector<AText*> MoveNameTexts;
	std::vector<AText*> MovePPTexts;
	std::vector<AImageElement*> MoveTypeImages;

	// SmallCommonBox 요소
	APokemonElement* SCBMiniStopImage = nullptr;
	AImageElement* SCBGenderIcon = nullptr;
	AText* SCBNameText = nullptr;
	std::vector<AImageElement*> SCBTypeImages;

	// MovesDetailBox 요소
	AText* MovePowerText = nullptr;
	AText* MoveAccuracyText = nullptr;
	AText* MoveExplainText = nullptr;
	ACursor* MoveFocusCursor = nullptr;

	// 유틸
	void RefreshMoveDetailBox();
};

