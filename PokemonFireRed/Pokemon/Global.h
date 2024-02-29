#pragma once
#include <EngineBase/EngineMath.h>

enum class Id
{
	Player,
	PlayersMom,
	Rival,
	RivalsSister,
	Oaks,
	OaksLabM00,
	OaksLabM01,
	OaksLabF00,
	PalletM00,
	PalletF00,
	NES = 1000,
	StairTo1F,
	StairTo2F,
	MenuWindow = 2000,
	DialogueWindow,
	MapNameWindow,
	BlackScreen,
	WhiteScreen,
	CurtainScreen
};

// 전역으로 사용할 상수를 정의한다.
namespace Global
{
	const float CharacterWalkSpeed = 3.6f;
	const float CharacterJumpSpeed = 1.4f;
	const float CharacterWalkAnimFrameLength = 1.0f / 8;
	const float CharacterJumpAnimFrameLength = 1.0f / 48;

	const int PixelSize = 3;
	const int ScreenX = (240 * PixelSize);
	const int ScreenY = (160 * PixelSize);;
	const int ImageTileSize = 16;
	const int TileSize = (ImageTileSize * PixelSize);
	const int HalfScreenX = (ScreenX / 2);
	const int HalfScreenY = (ScreenY / 2);

	const float FloatPixelSize = static_cast<float>(3);
	const float FloatScreenX = static_cast<float>(240 * PixelSize);
	const float FloatScreenY = static_cast<float>(160 * PixelSize);
	const float FloatImageTileSize = static_cast<float>(16);
	const float FloatTileSize = static_cast<float>(ImageTileSize * PixelSize);
	const float FloatHalfScreenX = static_cast<float>(ScreenX / 2);
	const float FloatHalfScreenY = static_cast<float>(ScreenY / 2);

	const FVector Screen = FVector(ScreenX, ScreenY);
	const FVector HalfScreen = FVector(HalfScreenX, HalfScreenY);
	const FVector HpBarAreaRenderScale = FVector(48.0f, 3.0f) * Global::FloatPixelSize;
	const FVector ExpBarAreaRenderScale = FVector(64.0f, 3.0f) * Global::FloatPixelSize;

	const std::string SuffixUpperBody = "UpperBody";
	const std::string SuffixLowerBody = "LowerBody";
	const std::string TitleLevel = "TitleLevel";
	const std::string TutorialLevel = "TutorialLevel";
	const std::string ExteriorPalletTownLevel = "ExteriorPalletTownLevel";
	const std::string InteriorOaksLabLevel = "InteriorOaksLabLevel";
	const std::string InteriorRivalsHouseLevel = "InteriorRivalsHouseLevel";
	const std::string InteriorPlayersHouse1FLevel = "InteriorPlayersHouse1FLevel";
	const std::string InteriorPlayersHouse2FLevel = "InteriorPlayersHouse2FLevel";
	const std::string PokemonUILevel = "PokemonUILevel";
	const std::string PokemonSummaryUILevel = "PokemonUISummaryLevel";
	const std::string TrainerCardUILevel = "TrainerCardUILevel";
	const std::string WildBattleLevel = "WildBattleLevel";
	const std::string PokemonFrontPrefix = "PokemonFront";
	const std::string PokemonBackPrefix = "PokemonBack";
	const std::string PokemonMiniMovePrefix = "PokemonMiniMove";
	const std::string PokemonMiniStopPrefix = "PokemonMiniStop";
	const std::string PlayerBattlerIdle = "PlayerBattlerIdle";
	const std::string PlayerBattlerThrow = "PlayerBattlerThrow";
	const std::string ThrowedBall = "ThrowedBall";
	const std::string Player = "Player";
	const std::string BlackScreen = "BlackScreen";
	const std::string WhiteScreen = "WhiteScreen";
	const std::string CurtainScreen = "CurtainScreen";
	const std::string FadeLevelChanger = "FadeLevelChanger";
	const std::string WildBattleTrigger = "WildBattleTrigger";
	const Color8Bit PixelColliderBlock(255, 0, 255, 0);
	const Color8Bit PixelColliderBlockUpDown(255, 1, 255, 0);
};

// Resource Name
class RN
{
public:
	static const std::string PokemonFront;
	static const std::string PokemonBack;
	static const std::string PokemonMini;
	static const std::string PokemonFrontPlaceHolder;
	static const std::string PokemonBackPlaceHolder;
	static const std::string PokemonMiniPlaceHolder;
	static const std::string GenderMarkMale;
	static const std::string GenderMarkFemale;
	static const std::string BigGenderMarkMale;
	static const std::string BigGenderMarkFemale;
	static const std::string HpBarRed;
	static const std::string HpBarYellow;
	static const std::string HpBarGreen;
	static const std::string HpBarArea;
	static const std::string ExpBar;
	static const std::string ExpBarArea;
	static const std::string BlackCursor;
	static const std::string PokemonUIBackground;
	static const std::string PokemonUITargetSelectionMsgBox;
	static const std::string PokemonUIActionSelectionMsgBox;
	static const std::string PokemonUISwitchSelectionMsgBox;
	static const std::string PokemonUIActionBox;
	static const std::string PokemonUIFirstBox;
	static const std::string PokemonUIFirstFocusedBox;
	static const std::string PokemonUIFirstFromBox;
	static const std::string PokemonUIFirstToBox;
	static const std::string PokemonUIEntryBox;
	static const std::string PokemonUIEntryEmptyBox;
	static const std::string PokemonUIEntryFocusedBox;
	static const std::string PokemonUIEntryFromBox;
	static const std::string PokemonUIEntryToBox;
	static const std::string PokemonUICancel;
	static const std::string PokemonUICancelFocused;
	static const std::string PokemonSummaryUIBackground;
	static const std::string PokemonSummaryUICommonBox;
	static const std::string PokemonSummaryUISmallCommonBox;
	static const std::string PokemonSummaryUIInfoBox;
	static const std::string PokemonSummaryUISkillsBox;
	static const std::string PokemonSummaryUIMovesBox;
	static const std::string PokemonSummaryUIMovesDetailBox;
	static const std::string PokemonSummaryUIMoveFocus;
	static const std::string PokemonSummaryUINavInfo;
	static const std::string PokemonSummaryUINavSkills;
	static const std::string PokemonSummaryUINavMoves;
	static const std::string PokemonSummaryUINavMovesDetail;
	static const std::string TypeBug;
	static const std::string TypeDark;
	static const std::string TypeDragon;
	static const std::string TypeElectric;
	static const std::string TypeFighting;
	static const std::string TypeFire;
	static const std::string TypeFlying;
	static const std::string TypeGhost;
	static const std::string TypeGrass;
	static const std::string TypeGround;
	static const std::string TypeIce;
	static const std::string TypeNormal;
	static const std::string TypePoison;
	static const std::string TypePsychic;
	static const std::string TypeRock;
	static const std::string TypeSteel;
	static const std::string TypeWater;
	static const std::string TypePlaceHolder;
	static const std::string MaleTrainerCard;

	static const std::string BgmTitleBattle;
	static const std::string BgmTitleScreen;
	static const std::string BgmTutorial;
	static const std::string BgmPalletTown;

	static const std::string BattleLevel;
	static const std::string BattleBackground;
	static const std::string BattleMsgBox;
	static const std::string BattleActionBox;
	static const std::string BattleEnemyPokemonBox;
	static const std::string BattlePlayerPokemonBox;
	static const std::string BattleEnemyGround;
	static const std::string BattlePlayerGround;
	static const std::string PlayerBattler;
protected:

private:
	RN() {}
	~RN() {}
};

enum ERenderingOrder
{
	None,
	BelowBackground,
	Background,
	BelowCharacter,
	Lower,
	Upper,
	Foreground,
	UI0,
	UI1,
	UI2,
	UI3,
	UI4,
	UI5,
	UI6,
	Fade,
	Super
};

enum ECollisionOrder
{
	Player,
	NPC,
};