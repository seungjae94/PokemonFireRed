#pragma once
#include "PokemonMath.h"
#include "EventTarget.h"

class AMap;
class UMapLevel;
class UEventManager;
class UEventProcessor;

enum class EPlayerState
{
	None,
	Idle,
	Walk,
	WalkInPlace,
	Jump,
	OutOfControl
};

enum class EPlayerAction
{
	ZClick,
	Read,
	ArrowClick,
	StepOn,
	Menu,
};

// ����� �Է¿� ���� �̵��ϴ� ����
class APlayerCharacter : public AEventTarget
{
	friend UEventManager;
	friend UEventProcessor;
public:
	// constructor destructor
	APlayerCharacter();
	~APlayerCharacter();

	// delete Function
	APlayerCharacter(const APlayerCharacter& _Other) = delete;
	APlayerCharacter(APlayerCharacter&& _Other) noexcept = delete;
	APlayerCharacter& operator=(const APlayerCharacter& _Other) = delete;
	APlayerCharacter& operator=(APlayerCharacter&& _Other) noexcept = delete;

	void SetMap(AMap* _Map)
	{
		Map = _Map;
	}

	// FSM (Finite State Machine)
	void StateChange(EPlayerState _State, bool _Restart = false);

	bool HasControl() const;

protected:
private:
	class UInputStatus
	{
	public:
		bool ZKeyDown = false;
		bool ArrowKeyDown = false;
		int ArrowKey = 0;
		float SinceLastZKeyDown = 100000000.0f;
		float SinceLastArrowKeyDown = 100000000.0f;
	};

	UInputStatus InputStatus;
	void UpdateInputState(float _DeltaTime);

	// ����
	AMap* Map = nullptr;
	
	// ����
	EPlayerState State = EPlayerState::None;

	// �̵� ���� ����
	FTileVector MemoryDirection = FTileVector::Zero;
	FTileVector PrevPoint;
	FTileVector NextPoint;
	
	// ���۰� ����� �ȱ� �ӵ��� 3.6f
	float WalkSpeed = Global::CharacterWalkSpeed;
	float JumpSpeed = Global::CharacterJumpSpeed;

	bool IsRotating = false;
	float RotateTime = 0.5f / WalkSpeed;
	float CurRotateTime = RotateTime;
	float WalkTime = 1.0f / WalkSpeed;
	float CurWalkTime = WalkTime;
	float JumpTime = 1.0f / JumpSpeed; // 2ĭ �����ϴµ� �ɸ��� �ð�
	float CurJumpTime = JumpTime;
	float WalkInputLatency = 0.75f; // �ȱ� ������ �� �ۼ�Ʈ�� �������� ������ �Է��� �ޱ� ������ ������.
	float JumpInputLatency = 0.875f; // �ȱ� ������ �� �ۼ�Ʈ�� �������� ������ �Է��� �ޱ� ������ ������.

	const float CollisionSEInterval = 2.0f / WalkSpeed;
	float CollisionSETimer = 0.0f;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	// FSM (Finite State Machine)
	void StateUpdate(float _DeltaTime);
	void ChangeAnimation(EPlayerState _State, FTileVector _Direction);

	void IdleStart();
	void Idle(float _DeltaTime);
	void TryWalk();

	void WalkStart();
	void Walk(float _DeltaTime);

	void WalkInPlaceStart();
	void WalkInPlace(float _DeltaTime);

	void JumpStart();
	void Jump(float _DeltaTime);

	// �̺�Ʈ üũ �Լ�
	bool IsZDown();
	bool IsEnterDown();

	// �̺�Ʈ ���� �õ� �Լ�
	bool TryZClickEvent();
	bool TryReadEvent();
	bool TryArrowClickEvent();
	bool TryStepOnEvent();
	bool TryBattleEvent();

	// �浹 üũ
	Color8Bit GetPointColor(FTileVector _RelativePoint = FTileVector::Zero); // �浹�ʿ��� (CurPoint + _RelativePoint)�� ����
	bool IsLedge(FTileVector _Direction);
	bool IsGrass();
	//const FGrassInfo* GetGrassType();
	bool IsPixelCollider(FTileVector _Direction);
	bool IsComponentCollider(FTileVector _Direction);
	bool IsCollider(FTileVector _Direction);
};

