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
class Global
{
public:
	static const float CharacterWalkSpeed;
	static const float CharacterJumpSpeed;
	static const float CharacterWalkAnimFrameLength;
	static const float CharacterJumpAnimFrameLength;

	static const int PixelSize;
	static const int ScreenX;
	static const int ScreenY;
	static const int ImageTileSize;
	static const int TileSize;
	static const int HalfScreenX;
	static const int HalfScreenY;

	static const float FloatPixelSize;
	static const float FloatScreenX;
	static const float FloatScreenY;
	static const float FloatImageTileSize;
	static const float FloatTileSize;
	static const float FloatHalfScreenX;
	static const float FloatHalfScreenY;

	static const FVector Screen;
	static const FVector HalfScreen;
	static const FVector MiniPokemonRenderScale;
	static const FVector HpBarAreaRenderScale;
	static const FVector ExpBarAreaRenderScale;

	static const std::string SuffixUpperBody;
	static const std::string SuffixLowerBody;

	static const std::string TitleLevel;
	static const std::string TutorialLevel;
	static const std::string ExteriorPalletTownLevel;
	static const std::string InteriorOaksLabLevel;
	static const std::string InteriorRivalsHouseLevel;
	static const std::string InteriorPlayersHouse1FLevel;
	static const std::string InteriorPlayersHouse2FLevel;
	static const std::string PokemonUILevel;

	static const std::string PokemonFrontPrefix;
	static const std::string PokemonBackPrefix;
	static const std::string PokemonMiniPrefix;
	static const std::string PlayerName;

	static const Color8Bit PixelColliderBlock;
	static const Color8Bit PixelColliderBlockUpDown;
protected:

private:
	Global() {}
	~Global() {}
};

// Resource Name
class RN
{
public:
	static const std::string PokemonFront;
	static const std::string PokemonBack;
	static const std::string PokemonMini;
	static const std::string GenderMarkMale;
	static const std::string GenderMarkFemale;
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
	LowerUI,
	UpperUI,
	Upper2UI,
	ScrollBarArea,
	ScrollBar,
	Text,
	Fade,
	Super
};

enum ECollisionOrder
{
	Player,
	NPC,
};