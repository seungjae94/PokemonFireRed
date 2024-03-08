#include "BattleCanvas.h"
#include "PokemonUtil.h"
#include "PlayerData.h"

ABattleCanvas::ABattleCanvas()
{
}

ABattleCanvas::~ABattleCanvas()
{
}

void ABattleCanvas::BeginPlay()
{
	ACanvas::BeginPlay();

	// �ֻ��� ���
	Background = CreateImageElement(this, ERenderingOrder::UI0, EPivotType::LeftTop, 0, 0);
	Background->SetImage(RN::BattleBackground);

	ActionBox = CreateImageElement(this, ERenderingOrder::UI7, EPivotType::RightBot, 0, 0);
	ActionBox->SetImage(RN::BattleActionBox);

	MoveSelectBox = CreateImageElement(this, ERenderingOrder::UI7, EPivotType::LeftBot, 0, 0);
	MoveSelectBox->SetImage(RN::BattleMoveSelectBox);

	EnemyPokemonBox = CreateImageElement(this, ERenderingOrder::UI1, EPivotType::LeftTop, 13, 16);
	EnemyPokemonBox->SetImage(RN::BattleEnemyPokemonBox);
	EnemyPokemonBoxInitPos = EnemyPokemonBox->GetRelativePosition();
	EnemyPokemonBoxHidePos = EnemyPokemonBoxInitPos + FVector::Left * Global::FloatScreenX;

	PlayerPokemonBox = CreateImageElement(this, ERenderingOrder::UI4, EPivotType::RightBot, -10, -48);
	PlayerPokemonBox->SetImage(RN::BattlePlayerPokemonBox);
	PlayerPokemonBoxInitPos = PlayerPokemonBox->GetRelativePosition();
	PlayerPokemonBoxHidePos = PlayerPokemonBoxInitPos + FVector::Right * Global::FloatScreenX;

	EnemyGround = CreateImageElement(this, ERenderingOrder::UI1, EPivotType::RightTop, 0, 46);
	EnemyGround->SetImage(RN::BattleEnemyGround);
	EnemyGroundInitPos = EnemyGround->GetRelativePosition();
	EnemyGroundHidePos = EnemyGroundInitPos + FVector::Left * Global::FloatScreenX;

	PlayerGround = CreateImageElement(this, ERenderingOrder::UI1, EPivotType::LeftBot, 4, -48);
	PlayerGround->SetImage(RN::BattlePlayerGround);
	PlayerGroundInitPos = PlayerGround->GetRelativePosition();
	PlayerGroundHidePos = PlayerGroundInitPos + FVector::Right * Global::FloatScreenX;

	ThrowedBall = CreateImageElement(this, ERenderingOrder::UI2, EPivotType::LeftTop, 42, 56);
	ThrowedBall->SetImage(Global::ThrowedBall);
	ThrowedBall->CreateAnimation(Global::ThrowedBall, 0, 41, 1.0f / 60, false);

	StatBox = CreateImageElement(this, ERenderingOrder::UI7, EPivotType::RightBot, -1, -1);
	StatBox->SetImage(RN::BattleStatUpBox);

	PlayerEntryArrow = CreateImageElement(this, ERenderingOrder::UI4, EPivotType::RightBot, 0, -60);
	PlayerEntryArrow->SetImage(RN::BattlePlayerEntryArrow);
	PlayerEntryArrowInitPos = PlayerEntryArrow->GetRelativePosition();
	PlayerEntryArrowHidePos = PlayerEntryArrowInitPos + FVector::Right * Global::FloatScreenX;

	EnemyEntryArrow = CreateImageElement(this, ERenderingOrder::UI1, EPivotType::LeftTop, 0, 40);
	EnemyEntryArrow->SetImage(RN::BattleEnemyEntryArrow);
	EnemyEntryArrowInitPos = EnemyEntryArrow->GetRelativePosition();
	EnemyEntryArrowHidePos = EnemyEntryArrowInitPos + FVector::Left * Global::FloatScreenX;

	// EnemyPokemonBox ���
	EnemyPokemonNameText = CreateText(EnemyPokemonBox, ERenderingOrder::UI2, EPivotType::LeftTop, 7, 12, EAlignType::Left, EFontColor::Black, EFontSize::Mini);
	EnemyPokemonLevelText = CreateText(EnemyPokemonBox, ERenderingOrder::UI2, EPivotType::LeftTop, 85, 12, EAlignType::Right, EFontColor::Black, EFontSize::Mini);
	EnemyPokemonHpBar = CreateScrollBar(EnemyPokemonBox, ERenderingOrder::UI2, EPivotType::LeftTop, 39, 17, EScrollType::Hp);
	EnemyPokemonGenderMark = CreateImageElement(EnemyPokemonBox, ERenderingOrder::UI2, EPivotType::LeftTop, 10, 5);
	EnemyPokemonStatusMark = CreateImageElement(EnemyPokemonBox, ERenderingOrder::UI2, EPivotType::LeftTop, 7, 14);

	// PlayerPokemonBox ���
	PlayerPokemonNameText = CreateText(PlayerPokemonBox, ERenderingOrder::UI5, EPivotType::RightTop, -84, 12, EAlignType::Left, EFontColor::Black, EFontSize::Mini);
	PlayerPokemonLevelText = CreateText(PlayerPokemonBox, ERenderingOrder::UI5, EPivotType::RightTop, -9, 12, EAlignType::Right, EFontColor::Black, EFontSize::Mini);
	PlayerPokemonCurHpText = CreateText(PlayerPokemonBox, ERenderingOrder::UI5, EPivotType::RightBot, -29, -6, EAlignType::Right, EFontColor::Black, EFontSize::Mini);
	PlayerPokemonHpText = CreateText(PlayerPokemonBox, ERenderingOrder::UI5, EPivotType::RightBot, -9, -6, EAlignType::Right, EFontColor::Black, EFontSize::Mini);
	PlayerPokemonHpBar = CreateScrollBar(PlayerPokemonBox, ERenderingOrder::UI5, EPivotType::RightTop, -55, 17, EScrollType::Hp);
	PlayerPokemonExpBar = CreateScrollBar(PlayerPokemonBox, ERenderingOrder::UI5, EPivotType::RightTop, -71, 33, EScrollType::Exp);
	PlayerPokemonGenderMark = CreateImageElement(PlayerPokemonBox, ERenderingOrder::UI5, EPivotType::LeftTop, 10, 5);
	PlayerPokemonStatusMark = CreateImageElement(PlayerPokemonBox, ERenderingOrder::UI5, EPivotType::RightTop, -65, 22);

	// EnemyGround ���
	EnemyBattler = CreateImageElement(EnemyGround, ERenderingOrder::UI3, EPivotType::RightBot, -40, -10);
	EnemyBattlerInitPos = EnemyBattler->GetRelativePosition();
	EnemyBattlerHidePos = EnemyBattlerInitPos + FVector::Right * Global::FloatHalfScreenX;

	EnemyPokemonImage = CreateImageElement(EnemyGround, ERenderingOrder::UI2, EPivotType::LeftTop, 36, -25, EImageElementType::PokemonFront);
	EnemyPokemonImageInitPos = EnemyPokemonImage->GetRelativePosition();
	EnemyPokemonImageHidePos = EnemyPokemonImageInitPos + UPokemonUtil::PixelVector(0, 16);
	EnemyPokemonImageFaintPos = EnemyPokemonImageInitPos + UPokemonUtil::PixelVector(0, 64);

	EnemyGroundBall = CreateImageElement(EnemyGround,
		ERenderingOrder::UI5, EPivotType::RightBot, -56, -12);
	EnemyGroundBall->SetImage(RN::BattleEnemyGroundBall);
	EnemyGroundBall->CreateAnimation(Global::BattleEnemyGroundBallShowAnim, 0, 2, 0.15f, false);

	// PlayerGround ���
	PlayerBattler = CreateImageElement(PlayerGround, ERenderingOrder::UI3, EPivotType::RightBot, -12, 0);
	PlayerBattler->SetImage(RN::PlayerBattler);
	PlayerBattler->CreateAnimation(Global::PlayerBattlerIdle, 0, 0, 0.0f, false);
	PlayerBattler->CreateAnimation(Global::PlayerBattlerThrow, { 1, 2, 3, 4 }, { 16.0f / 60, 8.0f / 60, 4.0f / 60, 4.0f / 60 }, false);
	PlayerBattlerInitPos = PlayerBattler->GetRelativePosition();
	PlayerBattlerHidePos = PlayerBattlerInitPos + UPokemonUtil::PixelVector(-120, 0);

	PlayerPokemonImage = CreateImageElement(PlayerGround, ERenderingOrder::UI2, EPivotType::LeftTop, 35, -49, EImageElementType::PokemonBack);
	PlayerPokemonImageInitPos = PlayerPokemonImage->GetRelativePosition();
	PlayerPokemonImageHidePos = PlayerPokemonImageInitPos + UPokemonUtil::PixelVector(0, 32);
	PlayerPokemonImageFaintPos = PlayerPokemonImageInitPos + UPokemonUtil::PixelVector(0, 64);

	// ActionBox ���
	ActionCursor = CreateCursor(ActionBox, ERenderingOrder::UI8, EPivotType::LeftTop, 9, 12);
	ActionCursor->SetOptionCount(4);

	int CursorHorGap = 56;
	int CursorVerGap = 16;
	ActionCursor->SetCursorPositions({ {0, 0}, {CursorHorGap, 0}, {0, CursorVerGap}, {CursorHorGap, CursorVerGap} });

	// MoveSelectBox ���
	MoveSelectCursor = CreateCursor(MoveSelectBox, ERenderingOrder::UI8, EPivotType::LeftTop, 9, 12);
	MoveSelectCursor->SetOptionCount(4);

	int MoveHorGap = 71;
	int MoveVerGap = 17;
	MoveSelectCursor->SetCursorPositions({ {0, 0}, {MoveHorGap, 0}, {0, MoveVerGap}, {MoveHorGap, MoveVerGap} });

	CurPPText = CreateText(MoveSelectBox, ERenderingOrder::UI8, EPivotType::RightTop, -26, 21, EAlignType::Right, EFontColor::Black2);
	MaxPPText = CreateText(MoveSelectBox, ERenderingOrder::UI8, EPivotType::RightTop, -9, 21, EAlignType::Right, EFontColor::Black2);
	MoveTypeText = CreateText(MoveSelectBox, ERenderingOrder::UI8, EPivotType::RightTop, -48, 37, EAlignType::Left, EFontColor::Black3);

	for (int i = 0; i < 4; ++i)
	{
		int RowIndex = i / 2;
		int ColIndex = i % 2;
		AText* MoveText = CreateText(MoveSelectBox, ERenderingOrder::UI8, EPivotType::LeftTop,
			17 + ColIndex * MoveHorGap, 20 + RowIndex * MoveVerGap, EAlignType::Left, EFontColor::Black3, EFontSize::Mini);
		MoveTexts.push_back(MoveText);
	}

	// StatBox ���
	StatHpText = CreateText(StatBox, ERenderingOrder::UI8, EPivotType::RightTop, -7, 18, EAlignType::Right, EFontColor::Black3);
	StatAtkText = CreateText(StatBox, ERenderingOrder::UI8, EPivotType::RightTop, -7, 33, EAlignType::Right, EFontColor::Black3);
	StatDefText = CreateText(StatBox, ERenderingOrder::UI8, EPivotType::RightTop, -7, 48, EAlignType::Right, EFontColor::Black3);
	StatSpAtkText = CreateText(StatBox, ERenderingOrder::UI8, EPivotType::RightTop, -7, 63, EAlignType::Right, EFontColor::Black3);
	StatSpDefText = CreateText(StatBox, ERenderingOrder::UI8, EPivotType::RightTop, -7, 78, EAlignType::Right, EFontColor::Black3);
	StatSpeedText = CreateText(StatBox, ERenderingOrder::UI8, EPivotType::RightTop, -7, 93, EAlignType::Right, EFontColor::Black3);

	// EntryArrow ���
	PlayerEntryBalls.resize(6);
	EnemyEntryBalls.resize(6);
	PlayerEntryBallsInitPos.resize(6);
	PlayerEntryBallsHidePos.resize(6);
	EnemyEntryBallsInitPos.resize(6);
	EnemyEntryBallsHidePos.resize(6);

	int BallInterval = 10;
	for (int i = 0; i < 6; ++i)
	{
		PlayerEntryBalls[i] = CreateImageElement(PlayerEntryArrow, ERenderingOrder::UI4, EPivotType::LeftTop, 20 + BallInterval * i, -10);
		PlayerEntryBallsInitPos[i] = PlayerEntryBalls[i]->GetRelativePosition();
		PlayerEntryBallsHidePos[i] = PlayerEntryBallsInitPos[i] + FVector::Right * Global::FloatScreenX;

		EnemyEntryBalls[i] = CreateImageElement(EnemyEntryArrow, ERenderingOrder::UI4, EPivotType::RightTop, -20 - BallInterval * i, -10);
		EnemyEntryBallsInitPos[i] = EnemyEntryBalls[i]->GetRelativePosition();
		EnemyEntryBallsHidePos[i] = EnemyEntryBallsInitPos[i] + FVector::Left * Global::FloatScreenX;
	}
}

void ABattleCanvas::Init(const UBattler* _Player, const UBattler* _Enemy)
{
	Player = _Player;
	Enemy = _Enemy;

	RefreshPlayerPokemonBox();
	RefreshEnemyPokemonBox();
	RefreshMoveSelectBox();
	RefreshPlayerEntry();
	RefreshEnemyEntry();
	InitPlayerImages();
	InitEnemyImages();
	InitPlayerUI();
	InitEnemyUI();
	ActionCursor->SetCursor(0);
	MoveSelectCursor->SetCursor(0);

	// Activeness ����
	Background->SetActive(true);
	ActionBox->SetActive(false);
	MoveSelectBox->SetActive(false);
	ThrowedBall->SetActive(false);
	PlayerGround->SetActive(true);
	EnemyGround->SetActive(true);
	EnemyGroundBall->SetActive(false);
	PlayerPokemonBox->SetActive(true);
	EnemyPokemonBox->SetActive(true);
	StatBox->SetActive(false);
	PlayerEntryArrow->SetActive(true);
	EnemyEntryArrow->SetActive(true);
}

void ABattleCanvas::InitPlayerImages()
{
	const UPokemon* PlayerPokemon = Player->CurPokemonReadonly();
	PlayerPokemonImage->SetPokemon(PlayerPokemon);
	PlayerPokemonImage->SetRelativePosition(PlayerPokemonImageHidePos);
	PlayerPokemonImage->SetScaleFactor(0.0f);
	
	PlayerBattler->ChangeAnimation(Global::PlayerBattlerIdle);
	PlayerBattler->SetRelativePosition(PlayerBattlerInitPos);
}

void ABattleCanvas::InitEnemyImages()
{
	const UPokemon* EnemyPokemon = Enemy->CurPokemonReadonly();
	EnemyPokemonImage->SetPokemon(EnemyPokemon);

	if (true == Enemy->IsWildPokemon())
	{
		EnemyBattler->SetImage(RN::BattlerPlaceHolder);
		EnemyPokemonImage->SetRelativePosition(EnemyPokemonImageInitPos);
	}
	else
	{
		EnemyBattler->SetImage(Enemy->GetTrainerImageName());
		EnemyPokemonImage->SetRelativePosition(EnemyPokemonImageHidePos);
		EnemyPokemonImage->SetScaleFactor(0.0f);
	}
}

void ABattleCanvas::InitPlayerUI()
{
	PlayerPokemonBox->SetRelativePosition(PlayerPokemonBoxHidePos);
	PlayerGround->SetRelativePosition(PlayerGroundHidePos);
	PlayerEntryArrow->SetRelativePosition(PlayerEntryArrowHidePos);

	for (int i = 0; i < 6; ++i)
	{
		PlayerEntryBalls[i]->SetRelativePosition(PlayerEntryBallsHidePos[i]);
	}
}

void ABattleCanvas::InitEnemyUI()
{
	EnemyGround->SetRelativePosition(EnemyGroundHidePos);
	EnemyPokemonBox->SetRelativePosition(EnemyPokemonBoxHidePos);
	EnemyEntryArrow->SetRelativePosition(EnemyEntryArrowHidePos);

	for (int i = 0; i < 6; ++i)
	{
		EnemyEntryBalls[i]->SetRelativePosition(EnemyEntryBallsHidePos[i]);
	}
}

void ABattleCanvas::RefreshEnemyPokemonBox()
{
	const UPokemon* EnemyPokemon = Enemy->CurPokemonReadonly();

	if (EnemyPokemon->GetStatusId() == EPokemonStatus::Normal)
	{
		EnemyPokemonBox->SetImage(RN::BattleEnemyPokemonBox);
	}
	else
	{
		EnemyPokemonBox->SetImage(RN::BattleEnemyPokemonSimpleBox);
	}

	EnemyPokemonNameText->SetText(EnemyPokemon->GetNameW());
	EnemyPokemonLevelText->SetText(L"��" + EnemyPokemon->GetLevelW());
	EnemyPokemonHpBar->SetMaxValue(HpBarMaxValue);
	EnemyPokemonHpBar->SetValue(UPokemonMath::Floor(static_cast<float>(HpBarMaxValue) * EnemyPokemon->GetCurHp() / EnemyPokemon->GetHp()));

	int EPNTPixelWidth = EnemyPokemonNameText->GetPixelLineWidth(0);
	EnemyPokemonGenderMark->SetRelativePosition(UPokemonUtil::PixelVector(7 + EPNTPixelWidth, 5));
	EnemyPokemonGenderMark->SetImage(EnemyPokemon->GetGenderImageName());
	EnemyPokemonStatusMark->SetImage(EnemyPokemon->GetStatusImageName());
}

void ABattleCanvas::RefreshPlayerPokemonBox()
{
	const UPokemon* PlayerPokemon = Player->CurPokemonReadonly();

	PlayerPokemonNameText->SetText(PlayerPokemon->GetNameW());
	PlayerPokemonLevelText->SetText(L"��" + PlayerPokemon->GetLevelW());
	PlayerPokemonCurHpText->SetText(PlayerPokemon->GetCurHpW());
	PlayerPokemonHpText->SetText(PlayerPokemon->GetHpW());
	PlayerPokemonHpBar->SetMaxValue(HpBarMaxValue);
	PlayerPokemonHpBar->SetValue(UPokemonMath::Floor(static_cast<float>(HpBarMaxValue) * PlayerPokemon->GetCurHp() / PlayerPokemon->GetHp()));
	PlayerPokemonExpBar->SetMaxValue(ExpBarMaxValue);
	PlayerPokemonExpBar->SetValue(UPokemonMath::Floor(static_cast<float>(ExpBarMaxValue) * PlayerPokemon->GetExp() / PlayerPokemon->GetExpSize()));

	// �̸� ���� ��ġ
	int PPNTPixelWidth = PlayerPokemonNameText->GetPixelLineWidth(0);
	PlayerPokemonGenderMark->SetRelativePosition(UPokemonUtil::PixelVector(19 + PPNTPixelWidth, 5));
	PlayerPokemonGenderMark->SetImage(PlayerPokemon->GetGenderImageName());
	PlayerPokemonStatusMark->SetImage(PlayerPokemon->GetStatusImageName());
}

void ABattleCanvas::RefreshMoveSelectBox()
{
	const UPokemon* PlayerPokemon = Player->CurPokemonReadonly();

	MoveSelectCursor->SetCursor(0);

	for (int i = 0; i < PlayerPokemon->GetMoveCount(); ++i)
	{
		MoveTexts[i]->SetText(PlayerPokemon->GetMoveNameW(i));
	}
	for (int i = PlayerPokemon->GetMoveCount(); i < 4; ++i)
	{
		MoveTexts[i]->SetText(L"-");
	}

	CurPPText->SetText(PlayerPokemon->GetMoveCurPPW(0));
	MaxPPText->SetText(PlayerPokemon->GetMovePPW(0));
	MoveTypeText->SetText(PlayerPokemon->GetMoveTypeUpperW(0));

	MoveSelectBox->SetActive(MoveSelectBox->IsActive());
}

void ABattleCanvas::RefreshPlayerEntry()
{
	for (int i = 0; i < Player->GetEntrySize(); ++i)
	{
		if (true == Player->IsFaint(i))
		{
			PlayerEntryBalls[i]->SetImage(RN::BattleEntryBallFaint);
		}
		else
		{
			PlayerEntryBalls[i]->SetImage(RN::BattleEntryBallAlive);
		}
	}
	for (int i = Player->GetEntrySize(); i < 6; ++i)
	{
		PlayerEntryBalls[i]->SetImage(RN::BattleEntryBallEmpty);
	}
}

void ABattleCanvas::RefreshEnemyEntry()
{
	for (int i = 0; i < Enemy->GetEntrySize(); ++i)
	{
		if (true == Enemy->IsFaint(i))
		{
			EnemyEntryBalls[i]->SetImage(RN::BattleEntryBallFaint);
		}
		else
		{
			EnemyEntryBalls[i]->SetImage(RN::BattleEntryBallAlive);
		}
	}
	for (int i = Enemy->GetEntrySize(); i < 6; ++i)
	{
		EnemyEntryBalls[i]->SetImage(RN::BattleEntryBallEmpty);
	}
}

void ABattleCanvas::RefreshPlayerPokemonImage()
{
	const UPokemon* PlayerPokemon = Player->CurPokemonReadonly();
	PlayerPokemonImage->SetPokemon(PlayerPokemon);
}

void ABattleCanvas::RefreshEnemyPokemonImage()
{
	const UPokemon* EnemyPokemon = Enemy->CurPokemonReadonly();
	EnemyPokemonImage->SetPokemon(EnemyPokemon);
}


void ABattleCanvas::LerpShowGrounds(float _t)
{
	FVector EnemyGroundPos = UPokemonMath::Lerp(EnemyGroundInitPos, EnemyGroundHidePos, _t);
	FVector PlayerGroundPos = UPokemonMath::Lerp(PlayerGroundInitPos, PlayerGroundHidePos, _t);
	EnemyGround->SetRelativePosition(EnemyGroundPos);
	PlayerGround->SetRelativePosition(PlayerGroundPos);
}

void ABattleCanvas::LerpShowEnemyPokemonBox(float _t)
{
	FVector EnemyPokemonBoxPos = UPokemonMath::Lerp(EnemyPokemonBoxInitPos, EnemyPokemonBoxHidePos, _t);
	EnemyPokemonBox->SetRelativePosition(EnemyPokemonBoxPos);
}

void ABattleCanvas::LerpShowEnemyEntryArrow(float _t)
{
	FVector Pos = UPokemonMath::Lerp(EnemyEntryArrowInitPos, EnemyEntryArrowHidePos, _t);
	EnemyEntryArrow->SetRelativePosition(Pos);
}

void ABattleCanvas::LerpShowPlayerEntryArrow(float _t)
{
	FVector Pos = UPokemonMath::Lerp(PlayerEntryArrowInitPos, PlayerEntryArrowHidePos, _t);
	PlayerEntryArrow->SetRelativePosition(Pos);
}

void ABattleCanvas::LerpEnemyEntryBall(int _Index, float _t)
{
	FVector Pos = UPokemonMath::Lerp(EnemyEntryBallsInitPos[_Index], EnemyEntryBallsHidePos[_Index], _t);
	EnemyEntryBalls[_Index]->SetRelativePosition(Pos);
}

void ABattleCanvas::LerpPlayerEntryBall(int _Index, float _t)
{
	FVector Pos = UPokemonMath::Lerp(PlayerEntryBallsInitPos[_Index], PlayerEntryBallsHidePos[_Index], _t);
	PlayerEntryBalls[_Index]->SetRelativePosition(Pos);
}

void ABattleCanvas::LerpHideEnemyEntry(float _t)
{
	EnemyEntryArrow->SetAlpha(_t);
	for (int i = 0; i < 6; ++i)
	{
		EnemyEntryBalls[i]->SetAlpha(_t);
	}
}

void ABattleCanvas::LerpHidePlayerEntry(float _t)
{
	PlayerEntryArrow->SetAlpha(_t);
	for (int i = 0; i < 6; ++i)
	{
		PlayerEntryBalls[i]->SetAlpha(_t);
	}
}

void ABattleCanvas::PlayBattlerThrowingAnimation()
{
	PlayerBattler->ChangeAnimation(Global::PlayerBattlerThrow);
}

void ABattleCanvas::LerpHidePlayerBattler(float _t)
{
	FVector Pos = UPokemonMath::Lerp(PlayerBattlerHidePos, PlayerBattlerInitPos, _t);
	PlayerBattler->SetRelativePosition(Pos);
}

void ABattleCanvas::LerpHideEnemyBattler(float _t)
{
	FVector Pos = UPokemonMath::Lerp(EnemyBattlerHidePos, EnemyBattlerInitPos, _t);
	EnemyBattler->SetRelativePosition(Pos);
}

void ABattleCanvas::PlayEnemyGroundBallAnimation()
{
	EnemyGroundBall->SetActive(true);
	EnemyGroundBall->ChangeAnimation(Global::BattleEnemyGroundBallShowAnim, true);
}

bool ABattleCanvas::IsEnemyGroundBallAnimationEnd()
{
	return EnemyGroundBall->IsCurAnimationEnd();
}

void ABattleCanvas::SetEnemyGroundBallActive(bool _Value)
{
	EnemyGroundBall->SetActive(_Value);
}

void ABattleCanvas::PlayThrowedBallAnimation()
{
	ThrowedBall->SetActive(true);
	ThrowedBall->ChangeAnimation(Global::ThrowedBall, true);
}

bool ABattleCanvas::IsThrowedBallAnimationEnd()
{
	return ThrowedBall->IsCurAnimationEnd();
}

void ABattleCanvas::SetThrowedBallActive(bool _Value)
{
	ThrowedBall->SetActive(_Value);
}

void ABattleCanvas::TakeInPlayerPokemonToBall(float _t)
{
	TakeOutPlayerPokemonFromBall(1.0f - _t);
}

void ABattleCanvas::TakeOutPlayerPokemonFromBall(float _t)
{
	float t = _t;
	if (t < 0.0f)
	{
		t = 0.0f;
	}

	PlayerPokemonImage->SetScaleFactor(1.0f - t);
	PlayerPokemonImage->SetAlpha(1.0f - t);

	FVector PlayerPokemonImagePos = UPokemonMath::Lerp(PlayerPokemonImageInitPos, PlayerPokemonImageHidePos, _t);
	PlayerPokemonImage->SetRelativePosition(PlayerPokemonImagePos);
}

void ABattleCanvas::TakeInEnemyPokemonFromBall(float _t)
{
	TakeOutEnemyPokemonFromBall(1.0f - _t);
}

void ABattleCanvas::TakeOutEnemyPokemonFromBall(float _t)
{
	float t = _t;
	if (t < 0.0f)
	{
		t = 0.0f;
	}

	EnemyPokemonImage->SetScaleFactor(1.0f - t);
	EnemyPokemonImage->SetAlpha(1.0f - t);

	FVector EnemyPokemonImagePos = UPokemonMath::Lerp(EnemyPokemonImageInitPos, EnemyPokemonImageHidePos, _t);
	EnemyPokemonImage->SetRelativePosition(EnemyPokemonImagePos);
}

void ABattleCanvas::LerpShowPlayerPokemonBox(float _t)
{
	FVector PlayerPokemonBoxPos = UPokemonMath::Lerp(PlayerPokemonBoxInitPos, PlayerPokemonBoxHidePos, _t);
	PlayerPokemonBox->SetRelativePosition(PlayerPokemonBoxPos);
}

void ABattleCanvas::SetActionBoxActive(bool _Value)
{
	ActionBox->SetActive(_Value);
}

void ABattleCanvas::SetMoveSelectBoxActive(bool _Value)
{
	MoveSelectBox->SetActive(_Value);
}

void ABattleCanvas::SetEnemyEntryArrowAlpha(float _Value)
{
	EnemyEntryArrow->SetAlpha(_Value);
	for (int i = 0; i < 6; ++i)
	{
		EnemyEntryBalls[i]->SetAlpha(_Value);
	}
}

void ABattleCanvas::SetEnemyPokemonBoxAlpha(float _Value)
{
	EnemyPokemonBox->SetAlpha(_Value);
}

void ABattleCanvas::LerpPlayerHpInfo(int _BeforeHp, int _AfterHp, int _MaxHp, float _t)
{
	int LerpedHp = UPokemonMath::Round(UPokemonMath::Lerp(static_cast<float>(_AfterHp), static_cast<float>(_BeforeHp), _t));
	PlayerPokemonCurHpText->SetText(std::to_wstring(LerpedHp));

	float LerpedBarValue = UPokemonMath::Lerp(static_cast<float>(HpBarMaxValue) * _AfterHp / _MaxHp, static_cast<float>(HpBarMaxValue) * _BeforeHp / _MaxHp, _t);
	PlayerPokemonHpBar->SetValue(UPokemonMath::Round(LerpedBarValue));
}

void ABattleCanvas::LerpEnemyHpInfo(int _BeforeHp, int _AfterHp, int _MaxHp, float _t)
{
	float LerpedBarValue = UPokemonMath::Lerp(static_cast<float>(HpBarMaxValue) * _AfterHp / _MaxHp, static_cast<float>(HpBarMaxValue) * _BeforeHp / _MaxHp, _t);
	EnemyPokemonHpBar->SetValue(UPokemonMath::Round(LerpedBarValue));
}

void ABattleCanvas::RefreshPlayerPokemonCurHpText()
{
	const UPokemon* PlayerPokemon = Player->CurPokemonReadonly();
	PlayerPokemonCurHpText->SetText(PlayerPokemon->GetCurHpW());
}

void ABattleCanvas::LerpFaintPlayerPokemon(float _t)
{
	FVector PokemonRelativePos = UPokemonMath::Lerp(PlayerPokemonImageFaintPos, PlayerPokemonImageInitPos, _t);
	PlayerPokemonImage->SetRelativePosition(PokemonRelativePos);
}

void ABattleCanvas::LerpFaintEnemyPokemon(float _t)
{
	FVector PokemonRelativePos = UPokemonMath::Lerp(EnemyPokemonImageFaintPos, EnemyPokemonImageInitPos, _t);
	EnemyPokemonImage->SetRelativePosition(PokemonRelativePos);
}

void ABattleCanvas::SetPlayerPokemonBoxActive(bool _Value)
{
	PlayerPokemonBox->SetActive(_Value);
}

void ABattleCanvas::SetEnemyPokemonBoxActive(bool _Value)
{
	EnemyPokemonBox->SetActive(_Value);
}

void ABattleCanvas::SetPlayerPokemonImageActive(bool _Value)
{
	PlayerPokemonImage->SetActive(_Value);
}

void ABattleCanvas::SetEnemyPokemonImageActive(bool _Value)
{
	EnemyPokemonImage->SetActive(_Value);
}

void ABattleCanvas::LerpExpBar(int _BeforeExp, int _AfterExp, int _MaxExp, float _t)
{
	float LerpedBarValue = UPokemonMath::Lerp(static_cast<float>(ExpBarMaxValue) * _AfterExp / _MaxExp, static_cast<float>(ExpBarMaxValue) * _BeforeExp / _MaxExp, _t);
	PlayerPokemonExpBar->SetValue(UPokemonMath::Round(LerpedBarValue));
}

void ABattleCanvas::ShowStatUpBox(const FLevelUpData& _LevelUpData)
{
	StatBox->SetActive(true);
	StatBox->SetImage(RN::BattleStatUpBox);

	StatHpText->SetText(std::to_wstring(_LevelUpData.UpHp));
	StatAtkText->SetText(std::to_wstring(_LevelUpData.UpAtk));
	StatDefText->SetText(std::to_wstring(_LevelUpData.UpDef));
	StatSpAtkText->SetText(std::to_wstring(_LevelUpData.UpSpAtk));
	StatSpDefText->SetText(std::to_wstring(_LevelUpData.UpSpDef));
	StatSpeedText->SetText(std::to_wstring(_LevelUpData.UpSpeed));
}

void ABattleCanvas::ShowStatAfterBox(const UPokemon* _Pokemon)
{
	StatBox->SetActive(true);
	StatBox->SetImage(RN::BattleStatAfterBox);

	StatHpText->SetText(_Pokemon->GetHpW());
	StatAtkText->SetText(_Pokemon->GetAtkW());
	StatDefText->SetText(_Pokemon->GetDefW());
	StatSpAtkText->SetText(_Pokemon->GetSpAtkW());
	StatSpDefText->SetText(_Pokemon->GetSpDefW());
	StatSpeedText->SetText(_Pokemon->GetSpeedW());
}

void ABattleCanvas::HideStatUpWindow()
{
	StatBox->SetActive(false);
}

void ABattleCanvas::Tick(float _DeltaTime)
{
	ACanvas::Tick(_DeltaTime);
}

