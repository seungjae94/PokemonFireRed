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

	static const std::string SuffixUpperBody;
	static const std::string SuffixLowerBody;

	static const std::string TitleLevel;
	static const std::string TutorialLevel;
	static const std::string ExteriorPalletTownLevel;
	static const std::string InteriorOaksLabLevel;
	static const std::string InteriorRivalsHouseLevel;
	static const std::string InteriorPlayersHouse1FLevel;
	static const std::string InteriorPlayersHouse2FLevel;

	static const std::string PlayerName;

	static const Color8Bit PixelColliderBlock;
	static const Color8Bit PixelColliderBlockUpDown;

	// delete Function
	Global(const Global& _Other) = delete;
	Global(Global&& _Other) noexcept = delete;
	Global& operator=(const Global& _Other) = delete;
	Global& operator=(Global&& _Other) noexcept = delete;

protected:

private:
	Global() {}
	~Global() {}
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
	Fade,
	Super
};

enum ECollisionOrder
{
	Player,
	NPC,
};