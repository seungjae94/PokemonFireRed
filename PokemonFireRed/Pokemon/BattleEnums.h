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
	BattleWin,		// 상대 포켓몬을 모두 쓰러뜨려서 전투가 끝나는 경우
	BattleLose,		// 플레이어 포켓몬이 모두 쓰러져서 전투가 끝나는 경우
	Run				// 도망에 성공한 경우
};

enum class EBattleAction
{
	None,
	Fight,
	Escape,
	Shift,
	Item,
};