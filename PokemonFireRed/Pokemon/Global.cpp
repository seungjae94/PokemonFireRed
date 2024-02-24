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
const FVector Global::HpBarAreaRenderScale = FVector(48.0f, 3.0f) * Global::FloatPixelSize;
const FVector Global::ExpBarAreaRenderScale = FVector(64.0f, 3.0f) * Global::FloatPixelSize;

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
const std::string Global::PokemonSummaryUILevel = "PokemonUISummaryLevel";

const std::string Global::PokemonFrontPrefix = "PokemonFront";
const std::string Global::PokemonBackPrefix = "PokemonBack";
const std::string Global::PokemonMiniPrefix = "PokemonMini";
const std::string Global::PlayerName = "Player";

const Color8Bit Global::PixelColliderBlock = Color8Bit(255, 0, 255, 0);
const Color8Bit Global::PixelColliderBlockUpDown = Color8Bit(255, 1, 255, 0);

const std::string RN::PokemonFront = "PokemonFront.png";
const std::string RN::PokemonBack = "PokemonBack.png";
const std::string RN::PokemonMini = "PokemonMini.png";
const std::string RN::GenderMarkMale = "GenderMarkMale.png";
const std::string RN::GenderMarkFemale = "GenderMarkFemale.png";
const std::string RN::HpBarRed = "HpBarRed.png";
const std::string RN::HpBarYellow = "HpBarYellow.png";
const std::string RN::HpBarGreen = "HpBarGreen.png";
const std::string RN::HpBarArea = "HpBarArea.png";
const std::string RN::ExpBar = "ExpBar.png";
const std::string RN::ExpBarArea = "ExpBarArea.png";
const std::string RN::BlackCursor = "BlackCursor.png";
const std::string RN::PokemonUIBackground = "UPBackground.png";
const std::string RN::PokemonUITargetSelectionMsgBox = "UPTargetSelectionMsgBox.png";
const std::string RN::PokemonUIActionSelectionMsgBox = "UPActionSelectionMsgBox.png";
const std::string RN::PokemonUISwitchSelectionMsgBox = "UPSwitchSelectionMsgBox.png";
const std::string RN::PokemonUIActionBox = "UPActionBox.png";
const std::string RN::PokemonUIFirstBox = "UPFirst.png";
const std::string RN::PokemonUIFirstFocusedBox = "UPFirstFocused.png";
const std::string RN::PokemonUIFirstFromBox = "UPFirstFrom.png";
const std::string RN::PokemonUIFirstToBox = "UPFirstTo.png";
const std::string RN::PokemonUIEntryBox = "UPEntry.png";
const std::string RN::PokemonUIEntryEmptyBox = "UPEntryEmpty.png";
const std::string RN::PokemonUIEntryFocusedBox = "UPEntryFocused.png";
const std::string RN::PokemonUIEntryFromBox = "UPEntryFrom.png";
const std::string RN::PokemonUIEntryToBox = "UPEntryTo.png";
const std::string RN::PokemonUICancel = "UPCancel.png";
const std::string RN::PokemonUICancelFocused = "UPCancelFocused.png";
const std::string RN::PokemonSummaryUIBackground = "UPSBackground.png";
const std::string RN::PokemonSummaryUICommonBox = "UPSCommonBox.png";
const std::string RN::PokemonSummaryUIInfoBox = "UPSInfoBox.png";
const std::string RN::PokemonSummaryUISkillsBox = "UPSSkillsBox.png";
const std::string RN::PokemonSummaryUIMovesBox = "UPSMovesBox.png";
const std::string RN::PokemonSummaryUIMovesDetailBox = "UPSMovesDetailBox.png";
const std::string RN::PokemonSummaryUIMoveFocusBox = "UPSFocus.png";
const std::string RN::PokemonSummaryUINavInfo = "UPSNavInfo.png";
const std::string RN::PokemonSummaryUINavSkills = "UPSNavSkills.png";
const std::string RN::PokemonSummaryUINavMoves = "UPSNavMoves.png";
const std::string RN::PokemonSummaryUINavMovesDetail = "UPSNavMovesDetail.png";

const std::string RN::TypeBug = "TypeBug.png";
const std::string RN::TypeDark = "TypeDark.png";
const std::string RN::TypeDragon = "TypeDragon.png";
const std::string RN::TypeElectric = "TypeElectric.png";
const std::string RN::TypeFighting = "TypeFighting.png";
const std::string RN::TypeFire = "TypeFire.png";
const std::string RN::TypeFlying = "TypeFlying.png";
const std::string RN::TypeGhost = "TypeGhost.png";
const std::string RN::TypeGrass = "TypeGrass.png";
const std::string RN::TypeGround = "TypeGround.png";
const std::string RN::TypeIce = "TypeIce.png";
const std::string RN::TypeNormal = "TypeNormal.png";
const std::string RN::TypePoison = "TypePoison.png";
const std::string RN::TypePsychic = "TypePsychic.png";
const std::string RN::TypeRock = "TypeRock.png";
const std::string RN::TypeSteel = "TypeSteel.png";
const std::string RN::TypeWater = "TypeWater.png";