#pragma once
#include <EngineBase/EngineMath.h>

// 전역으로 사용할 상수를 정의한다.
namespace Global
{
	// Cheat
	const float CheatDamageBonusCoeff = 100.0f;
	const float CheatExpBonusCoeff = 2.0f;
	const float CheatWildBattleFrequency = 0.5f;
	extern bool IsCheatOn;
	extern float DamageBonusCoeff;
	extern float ExpBonusCoeff;
	extern float WildBattleFrequency;

	// 기타
	const float CharacterWalkSpeed = 3.6f;
	const float CharacterJumpSpeed = 1.4f;
	const float CharacterWalkAnimFrameLength = 1.0f / 8;
	const float CharacterJumpAnimFrameLength = 1.0f / 48;

	const int PixelSize = 3;
	const int PixelScreenX = 240;
	const int PixelScreenY = 160;
	const int ScreenX = (240 * PixelSize);
	const int ScreenY = (160 * PixelSize);;
	const int ImageTileSize = 16;
	const int TileSize = (ImageTileSize * PixelSize);
	const int HalfScreenX = (ScreenX / 2);
	const int HalfScreenY = (ScreenY / 2);

	const float FloatPixelSize = static_cast<float>(3);
	const float FloatPixelScreenX = static_cast<float>(240);
	const float FloatPixelScreenY = static_cast<float>(160);
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
	const std::string ExteriorViridianForestLevel = "ExteriorViridianForestLevel";
	const std::string ExteriorPewterCityLevel = "ExteriorPewterCityLevel";
	const std::string InteriorOaksLabLevel = "InteriorOaksLabLevel";
	const std::string InteriorRivalsHouseLevel = "InteriorRivalsHouseLevel";
	const std::string InteriorPlayersHouse1FLevel = "InteriorPlayersHouse1FLevel";
	const std::string InteriorPlayersHouse2FLevel = "InteriorPlayersHouse2FLevel";
	const std::string InteriorPokemonCenterLevel = "InteriorPokemonCenterLevel";
	const std::string InteriorShopLevel = "InteriorShopLevel";
	const std::string InteriorPewterGymLevel = "InteriorPewterGymLevel";
	const std::string PokemonUILevel = "PokemonUILevel";
	const std::string PokemonSummaryUILevel = "PokemonUISummaryLevel";
	const std::string TrainerCardUILevel = "TrainerCardUILevel";
	const std::string BattleLevel = "BattleLevel";
	const std::string BagUILevel = "BagUILevel";

	const std::string PokemonFrontPrefix = "PokemonFront";
	const std::string PokemonBackPrefix = "PokemonBack";
	const std::string PokemonMiniMovePrefix = "PokemonMiniMove";
	const std::string PokemonMiniStopPrefix = "PokemonMiniStop";
	const std::string PlayerBattlerIdle = "PlayerBattlerIdle";
	const std::string PlayerBattlerThrow = "PlayerBattlerThrow";
	
	const std::string TutorialLevelFader = "TutorialLevelFader";
	const std::string ThrowedBall = "ThrowedBall";
	const std::string Player = "Player";
	const std::string FadeCanvas = "FadeCanvas";
	const std::string MenuWindow = "MenuWindow";
	const std::string DialogueWindow = "DialogueWindow";
	const std::string MapNameWindow = "MapNameWindow";
	const std::string FadeLevelChanger = "FadeLevelChanger";
	const std::string WildBattleTrigger = "WildBattleTrigger";

	const std::string BattleEnemyGroundBallShowAnim = "BattleEnemyGroundBallAnim";

	const Color8Bit PixelColliderBlock(255, 0, 255, 0);
	const Color8Bit PixelColliderBlockUpDown(255, 1, 255, 0);
};

// EventTarget Name
namespace EN
{
	// Common
	const std::string SpecialTrigger = "SpecialTrigger";

	// Pallet Town
	const std::string OaksLabDoor = "OaksLabDoor";
	const std::string PlayersHouseDoor = "PlayersHouseDoor";
	const std::string RivalshouseDoor = "RivalsHouseDoor";
	const std::string TechMan = "TechMan";
	const std::string GetStarterEventTrigger = "GetStarterEventTrigger";
	const std::string Oak = "Oak";
	const std::string Player = "Player";

	// Players House
	const std::string PlayersMom = "PlayersMom";

	// Oaks Lab
	const std::string RivalGreen = "RivalGreen";
	const std::string AfterRivalBattleTrigger = "AfterRivalBattleTrigger";

	// Common
	const std::string InteriorDoor = "InteriorDoor";

	// Viridian City
	const std::string Nurse = "Nurse";
	const std::string ShopOwner = "ShopOwner";
	const std::string ViridianPokemonCenterDoor = "ViridianPokemonCenterDoor";
	const std::string ViridianShopDoor = "ViridianShopDoor";

	// Viridian Forest
	const std::string BugCatcher = "BugCatcher";
	const std::string BugCatcher0 = "BugCatcher0";
	const std::string BugCatcher1 = "BugCatcher1";

	// Pewter City
	const std::string Camper = "Camper";
	const std::string LeaderBrock = "LeaderBrock";
	const std::string PewterPokemonCenterDoor = "PewterPokemonCenterDoor";
	const std::string PewterGymDoor = "PewterGymDoor";
};

// Resource Name
namespace RN
{
	const std::string RivalGreenCharacter = "RivalGreenCharacter.png";

	const std::string BlackScreen = "BlackScreen.png";
	const std::string WhiteScreen = "WhiteScreen.png";
	const std::string MenuWindowExplain = "MenuWindowExplain.png";
	const std::string DialogueWindow = "DialogueWindow.png";
	const std::string DialogueWindowCover = "DialogueWindowCover.png";
	const std::string NextMsgArrow = "NextMsgArrow.png";
	const std::string MapNameWindow = "MapNameWindow.png";

	const std::string PokemonFront = "PokemonFront.png";
	const std::string PokemonFrontStat0 = "PokemonFrontStat0.png";
	const std::string PokemonFrontStat1 = "PokemonFrontStat1.png";
	const std::string PokemonFrontStat2 = "PokemonFrontStat2.png";
	const std::string PokemonFrontStat3 = "PokemonFrontStat3.png";
	const std::string PokemonBack = "PokemonBack.png"; 
	const std::string PokemonMini = "PokemonMini.png"; 
	const std::string PokemonFrontPlaceHolder = "PokemonFrontPlaceHolder.png"; 
	const std::string PokemonBackPlaceHolder = "PokemonBackPlaceHolder.png"; 
	const std::string PokemonMiniPlaceHolder = "PokemonMiniPlaceHolder.png"; 
	const std::string GenderMarkMale = "GenderMarkMale.png"; 
	const std::string GenderMarkFemale = "GenderMarkFemale.png"; 
	const std::string BigGenderMarkMale = "BigGenderMarkMale.png"; 
	const std::string BigGenderMarkFemale = "BigGenderMarkFemale.png"; 
	const std::string HpBarRed = "HpBarRed.png"; 
	const std::string HpBarYellow = "HpBarYellow.png"; 
	const std::string HpBarGreen = "HpBarGreen.png"; 
	const std::string HpBarArea = "HpBarArea.png"; 
	const std::string ExpBar = "ExpBar.png"; 
	const std::string ExpBarArea = "ExpBarArea.png"; 
	const std::string BlackCursor = "BlackCursor.png"; 

	const std::string PokemonUIBackground = "UPBackground.png"; 
	const std::string PokemonUITargetSelectionMsgBox = "UPTargetSelectionMsgBox.png"; 
	const std::string PokemonUITargetSelectionMsgBoxBagMode = "UPTargetSelectionMsgBoxBagMode.png";
	const std::string PokemonUIActionSelectionMsgBox = "UPActionSelectionMsgBox.png"; 
	const std::string PokemonUISwitchSelectionMsgBox = "UPSwitchSelectionMsgBox.png"; 
	const std::string PokemonUIBattleMsgBox = "UPBattleMsgBox.png"; 
	const std::string PokemonUIActionBox = "UPActionBox.png"; 
	const std::string PokemonUIBattleActionBox = "UPBActionBox.png"; 
	const std::string PokemonUIFirstBox = "UPFirst.png"; 
	const std::string PokemonUIFirstFocusedBox = "UPFirstFocused.png"; 
	const std::string PokemonUIFirstFromBox = "UPFirstFrom.png"; 
	const std::string PokemonUIFirstToBox = "UPFirstTo.png"; 
	const std::string PokemonUIEntryBox = "UPEntry.png"; 
	const std::string PokemonUIEntryEmptyBox = "UPEntryEmpty.png"; 
	const std::string PokemonUIEntryFocusedBox = "UPEntryFocused.png"; 
	const std::string PokemonUIEntryFromBox = "UPEntryFrom.png"; 
	const std::string PokemonUIEntryToBox = "UPEntryTo.png"; 
	const std::string PokemonUICancel = "UPCancel.png"; 
	const std::string PokemonUICancelFocused = "UPCancelFocused.png"; 

	const std::string PokemonSummaryUIBackground = "UPSBackground.png"; 
	const std::string PokemonSummaryUICommonBox = "UPSCommonBox.png"; 
	const std::string PokemonSummaryUISmallCommonBox = "UPSSmallCommonBox.png"; 
	const std::string PokemonSummaryUIInfoBox = "UPSInfoBox.png"; 
	const std::string PokemonSummaryUISkillsBox = "UPSSkillsBox.png"; 
	const std::string PokemonSummaryUIMovesBox = "UPSMovesBox.png"; 
	const std::string PokemonSummaryUIMovesDetailBox = "UPSMovesDetailBox.png"; 
	const std::string PokemonSummaryUIMoveFocus = "UPSMoveFocus.png"; 
	const std::string PokemonSummaryUINavInfo = "UPSNavInfo.png"; 
	const std::string PokemonSummaryUINavSkills = "UPSNavSkills.png"; 
	const std::string PokemonSummaryUINavMoves = "UPSNavMoves.png"; 
	const std::string PokemonSummaryUINavMovesDetail = "UPSNavMovesDetail.png"; 

	const std::string TypeBug = "TypeBug.png"; 
	const std::string TypeDark = "TypeDark.png"; 
	const std::string TypeDragon = "TypeDragon.png"; 
	const std::string TypeElectric = "TypeElectric.png"; 
	const std::string TypeFighting = "TypeFighting.png"; 
	const std::string TypeFire = "TypeFire.png"; 
	const std::string TypeFlying = "TypeFlying.png"; 
	const std::string TypeGhost = "TypeGhost.png"; 
	const std::string TypeGrass = "TypeGrass.png"; 
	const std::string TypeGround = "TypeGround.png"; 
	const std::string TypeIce = "TypeIce.png"; 
	const std::string TypeNormal = "TypeNormal.png"; 
	const std::string TypePoison = "TypePoison.png"; 
	const std::string TypePsychic = "TypePsychic.png"; 
	const std::string TypeRock = "TypeRock.png"; 
	const std::string TypeSteel = "TypeSteel.png"; 
	const std::string TypeWater = "TypeWater.png"; 
	const std::string TypePlaceHolder = "TypePlaceHolder.png"; 

	const std::string StatusBurn = "StatusBurn.png";
	const std::string StatusFaint = "StatusFaint.png";
	const std::string StatusFreeze = "StatusFreeze.png";
	const std::string StatusParalysis = "StatusParalysis.png";
	const std::string StatusPoison = "StatusPoison.png";
	const std::string StatusSleep = "StatusSleep.png";
	const std::string StatusPlaceHolder = "StatusPlaceHolder.png";

	const std::string MaleTrainerCard = "MaleTrainerCard.png";

	const std::string BgmTitleBattle = "TitleBattle.wav"; 
	const std::string BgmTitleScreen = "TitleScreen.wav"; 
	const std::string BgmTutorial = "Tutorial.wav"; 
	const std::string BgmPalletTown = "PalletTown.wav"; 
	const std::string BgmWildBattle = "WildBattle.wav";

	const std::string BattleLevel = "BattleLevel"; 
	const std::string BattleMsgBox = "BattleMsgBox.png"; 
	const std::string BattleMsgBoxCover = "BattleMsgBoxCover.png"; 
	const std::string BattleBackground = "BattleBackground.png"; 
	const std::string BattleActionBox = "BattleActionBox.png"; 
	const std::string BattleMoveSelectBox = "BattleMoveSelectBox.png"; 
	const std::string BattleEnemyPokemonBox = "BattleEnemyPokemonBox.png"; 
	const std::string BattleEnemyPokemonSimpleBox = "BattleEnemyPokemonSimpleBox.png"; 
	const std::string BattlePlayerPokemonBox = "BattlePlayerPokemonBox.png"; 
	const std::string BattleEnemyGround = "BattleEnemyGround.png"; 
	const std::string BattlePlayerGround = "BattlePlayerGround.png"; 
	const std::string BattleStatUpBox = "BattleStatUpBox.png";
	const std::string BattleStatAfterBox = "BattleStatAfterBox.png";

	const std::string BattlerPlaceHolder = "BattlerPlaceHolder.png";
	const std::string PlayerBattler = "PlayerBattler.png"; 
	const std::string RivalGreenBattler = "RivalGreenBattler.png";
	const std::string BugCatcherBattler = "BugCatcherBattler.png";
	const std::string CamperBattler = "CamperBattler.png";
	const std::string LeaderBrockBattler = "LeaderBrockBattler.png";

	const std::string BattlePlayerEntryArrow = "BattlePlayerEntryArrow.png";
	const std::string BattleEnemyEntryArrow = "BattleEnemyEntryArrow.png";
	const std::string BattleEntryBallEmpty = "BattleEntryBallEmpty.png";
	const std::string BattleEntryBallAlive = "BattleEntryBallAlive.png";
	const std::string BattleEntryBallFaint = "BattleEntryBallFaint.png";
	const std::string BattleEnemyGroundBall = "BattleEnemyGroundBall.png";

	// 캐릭터 리소스
	const std::string BugCatcher = "BugCatcherIdle.png";

	// 오브젝트 리소스
	const std::string MapBall = "MapBall.png";
	const std::string GreenDoor = "GreenDoor";
	const std::string RedDoor = "RedDoor";
	const std::string SlideDoor = "SlideDoor";
	const std::string GymDoor = "GymDoor";
	const std::string BallHeal = "BallHeal.png";

	// 맵 레벨 UI 리소스
	const std::string YesNoOptionBox = "YesNoOptionBox.png";
	const std::string StarterBackground = "StarterBackground.png";

	// 가방 UI 레벨 리소스
	const std::string UBItemsBackground = "UBItemsBackground.png";
	const std::string UBKeyItemsBackground = "UBKeyItemsBackground.png";
	const std::string UBPokeBallsBackground = "UBPokeBallsBackground.png";

	const std::string UBArrowLeft = "UBArrowLeft.png";
	const std::string UBArrowRight = "UBArrowRight.png";
	const std::string UBCloseBag = "UBCloseBag.png";
	const std::string UBActionBox = "UBActionBox.png";
	const std::string UBActionItemBox = "UBActionItemBox.png";

	// 엔딩 리소스
	const std::string EndingBackground = "EndingBackground.png";
	const std::string EndingMessage = "EndingMessage.png";
	const std::string EndingRunnerGround = "EndingRunnerGround.png";
	const std::string EndingRunner = "EndingRunner.png";

	// 배틀 이펙트 리소스
	const std::string TackleEffect = "TackleEffect.png";
	const std::string GrowlEffect = "GrowlEffect.png";
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
	UI7,
	UI8,
	UI9,
	Fade,
	Super
};

enum ECollisionOrder
{
	Player,
	NPC,
};