#pragma once

enum class EEventTriggerAction
{
	ZClick,		// �÷��̾ Ʈ���ſ� ���� and Ʈ���Ÿ� �ٶ� and ZŰ �Է� (NPC ��)
	Read,		// �÷��̾ Ʈ���ſ� ���� and Ʈ���Ÿ� �ٶ� (ǥ���� ��)
	ArrowClick,	// �÷��̾ Ʈ���ſ� ���� and Ʈ���Ű� �ִ� ������ ����Ű �Է� (��, ��� ��)
	StepOn,		// �÷��̾ Ʈ���ſ� ���� ��ġ�� ���� (���ʸ��� ó�� ���� �� �̺�Ʈ�� �߻���Ű�� ���� ��)
	Direct		// �̺�Ʈ�� ���� �߻���Ű�� ��� (Ÿ��Ʋ ���� ���̵� �ƿ� Ʈ���� ��)
};


enum class EEventType
{
	SetActive,
	Destroy,
	Move,
	MoveDynamicPath,
	MoveWithoutRestriction,
	Surprise,
	FadeIn,
	FadeOut,
	FadeInBgm,
	FadeOutBgm,
	Wait,
	Chat,
	ShowMapName,
	ChangeArea,
	ChangeLevel,
	ChangePoint,
	ChangePosition,
	ChangeDirection,
	StarePlayer,
	PlayAnimation,
	PlayBgm,
	PlaySE,
	PauseBgm,
	ResumeBgm,
	HideActor,
	ShowActor,
	CameraFocus,
	DeactivatePlayerControl,
	WildBattle,
	TrainerBattle,
	Achieve,
	Unachieve,
	GainItem,
	End,
};

enum class EFadeType
{
	Black,
	White,
	HCurtain,
	VCurtain
};

enum class EAnimTarget
{
	All,
	UpperBodyOnly,
	LowerBodyOnly,
};