#pragma once

enum class EEventTriggerAction
{
	ZClick,		// 플레이어가 트리거에 인접 and 트리거를 바라봄 and Z키 입력 (NPC 등)
	Read,		// 플레이어가 트리거에 인접 and 트리거를 바라봄 (표지판 등)
	ArrowClick,	// 플레이어가 트리거에 인접 and 트리거가 있는 방향의 방향키 입력 (문, 계단 등)
	StepOn,		// 플레이어가 트리거와 같은 위치에 있음 (태초마을 처음 나갈 때 이벤트를 발생시키는 발판 등)
	Direct		// 이벤트를 직접 발생시키는 경우 (타이틀 레벨 페이드 아웃 트리거 등)
};