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
	Turn,
	BattleWin,		// ��� ���ϸ��� ��� �����߷��� ������ ������ ���
	BattleLose,		// �÷��̾� ���ϸ��� ��� �������� ������ ������ ���
	Run				// ������ ������ ���
};

enum class EBattleAction
{
	None,
	Fight,
	Escape,
	Shift,
	Item,
};