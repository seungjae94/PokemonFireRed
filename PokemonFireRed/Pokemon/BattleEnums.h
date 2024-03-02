#pragma once

enum class EEnemyType
{
	Wild,
	Trainer,
	GymLeader
};

enum class EBattleState
{
	BattleStart,
	PlayerActionSelect,
	PlayerMoveFailed,
	EnemyMoveFailed,
	PlayerMove,
	EnemyMove,
	PlayerSecondaryEffect,
	EnemySecondaryEffect,
	BattleEnd
};

enum class EBattleAction
{
	None,
	Fight,
	EscapeSuccess,
	EscapeFail,
	Shift,
	Item,
};