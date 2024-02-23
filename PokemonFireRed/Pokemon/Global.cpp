#include "Global.h"
#include <Windows.h>

const float Global::CharacterWalkSpeed = 3.6f;
const float Global::CharacterJumpSpeed = 1.4f;
const float Global::CharacterWalkAnimFrameLength = 1.0f / 8;
const float Global::CharacterJumpAnimFrameLength = 1.0f / 48;

const int Global::PixelSize = 3;
const int Global::ScreenX = (240 * PixelSize);
const int Global::ScreenY = (160 * PixelSize);
const int Global::ImageTileSize = 16;
const int Global::TileSize = (ImageTileSize * PixelSize);
const int Global::HalfScreenX = (ScreenX / 2);
const int Global::HalfScreenY = (ScreenY / 2);

const float Global::FloatPixelSize = static_cast<float>(3);
const float Global::FloatScreenX = static_cast<float>(240 * PixelSize);
const float Global::FloatScreenY = static_cast<float>(160 * PixelSize);
const float Global::FloatImageTileSize = static_cast<float>(16);
const float Global::FloatTileSize = static_cast<float>(ImageTileSize * PixelSize);
const float Global::FloatHalfScreenX = static_cast<float>(ScreenX / 2);
const float Global::FloatHalfScreenY = static_cast<float>(ScreenY / 2);

const FVector Global::Screen = FVector(ScreenX, ScreenY);
const FVector Global::HalfScreen = FVector(HalfScreenX, HalfScreenY);
const FVector Global::MiniPokemonRenderScale = FVector(32.0f, 24.0f) * Global::FloatPixelSize;
const FVector Global::ScrollBarRenderScale = FVector(1.0f, 3.0f * Global::FloatPixelSize);

const std::string Global::SuffixUpperBody = "UpperBody";
const std::string Global::SuffixLowerBody = "LowerBody";

const std::string Global::TitleLevel = "TitleLevel";
const std::string Global::TutorialLevel = "TutorialLevel";
const std::string Global::ExteriorPalletTownLevel = "ExteriorPalletTownLevel";
const std::string Global::InteriorOaksLabLevel = "InteriorOaksLabLevel";
const std::string Global::InteriorRivalsHouseLevel = "InteriorRivalsHouseLevel";
const std::string Global::InteriorPlayersHouse1FLevel = "InteriorPlayersHouse1FLevel";
const std::string Global::InteriorPlayersHouse2FLevel = "InteriorPlayersHouse2FLevel";
const std::string Global::PokemonUILevel = "PokemonUILevel";

const std::string Global::PokemonFrontPrefix = "PokemonFront";
const std::string Global::PokemonBackPrefix = "PokemonBack";
const std::string Global::PokemonMiniPrefix = "PokemonMini";

const std::string Global::PokemonFrontImageName = "PokemonFront.png";
const std::string Global::PokemonBackImageName = "PokemonBack.png";
const std::string Global::PokemonMiniImageName = "PokemonMini.png";
const std::string Global::GenderMarkMaleImageName = "GenderMarkMale.png";
const std::string Global::GenderMarkFemaleImageName = "GenderMarkFemale.png";
const std::string Global::HpBarBlackImageName = "HpBarBlack.png";
const std::string Global::HpBarRedImageName = "HpBarRed.png";
const std::string Global::HpBarYellowImageName = "HpBarYellow.png";
const std::string Global::HpBarGreenImageName = "HpBarGreen.png";
const std::string Global::ExpBarImageName = "ExpBar.png";
const std::string Global::BlackCursorImageName = "BlackCursor.png";

const std::string Global::PlayerName = "Player";

const Color8Bit Global::PixelColliderBlock = Color8Bit(255, 0, 255, 0);
const Color8Bit Global::PixelColliderBlockUpDown = Color8Bit(255, 1, 255, 0);