#pragma once
#include "Canvas.h"
#include "PlayerData.h"

class UPokemonUILevel;

class APokemonCanvas : public ACanvas
{
public:
	enum class EBoxState
	{
		Empty,
		Unfocused,
		Focused,
		From,
		To
	};
public:
	// constructor destructor
	APokemonCanvas();
	~APokemonCanvas();

	// delete Function
	APokemonCanvas(const APokemonCanvas& _Other) = delete;
	APokemonCanvas(APokemonCanvas&& _Other) noexcept = delete;
	APokemonCanvas& operator=(const APokemonCanvas& _Other) = delete;
	APokemonCanvas& operator=(APokemonCanvas&& _Other) noexcept = delete;

	// 캔버스 상태 초기화
	void Init();

	int GetActionCursor() const;
	void SetActionCursor(int _Cursor);
	void IncActionCursor();
	void DecActionCursor();

	int GetBattleActionCursor() const;
	void SetBattleActionCursor(int _Cursor);
	void IncBattleActionCursor();
	void DecBattleActionCursor();

	void SetTargetSelectionMsgBoxImage(std::string_view _ImageName);

	// 스위치 기능
	AImageElement* GetPokemonBox(int _Index);
	bool IsFirstBox(const AImageElement* _PokemonBox) const;
	void LerpPokemonBox(int _Index, const FVector& _Before, const FVector& _After, float _t);

	// Active 함수
	void SetTargetSelectionMsgBoxActive(bool _Value);
	void SetActionSelectionMsgBoxActive(bool _Value);
	void SetSwitchSelectionMsgBoxActive(bool _Value);
	void SetActionBoxActive(bool _Value);
	void SetBattleActionBoxActive(bool _Value);
	void SetBattleMsgBoxActive(bool _Value);
	void SetBattleMessage(std::wstring _Msg);

	// Refresh 함수
	void SetBoxState(int _BoxIndex, EBoxState _BoxState);
	void RefreshFirst(bool _IsSwitchMode);
	void RefreshEntry(int _Index, bool _IsSwitchMode);
	void RefreshCancel();
	void RefreshAllTargets(bool _IsSwitchMode = false);

protected:
private:
	void BeginPlay() override;

	AImageElement* Background = nullptr;
	AImageElement* TargetSelectionMsgBox = nullptr;
	AImageElement* ActionSelectionMsgBox = nullptr;
	AImageElement* SwitchSelectionMsgBox = nullptr;
	AImageElement* ActionBox = nullptr;
	ACursor* ActionCursor = nullptr;
	AImageElement* BattleActionBox = nullptr;
	ACursor* BattleActionCursor = nullptr;
	AImageElement* BattleMsgBox = nullptr;
	AText* BattleMsg = nullptr;

	AImageElement* FirstBox = nullptr;
	AImageElement* FirstPokemonIcon = nullptr;
	AText* FirstNameText = nullptr;
	AText* FirstLevelText = nullptr;
	AText* FirstHpText = nullptr;
	AText* FirstCurHpText = nullptr;
	AScrollBar* FirstHpBar = nullptr;
	AImageElement* FirstGender = nullptr;
	AImageElement* FirstStatus = nullptr;

	std::vector<AImageElement*> EntryBoxes;
	std::vector<AImageElement*> EntryPokemonIcons;
	std::vector<AText*> EntryNameTexts;
	std::vector<AText*> EntryLevelTexts;
	std::vector<AText*> EntryHpTexts;
	std::vector<AText*> EntryCurHpTexts;
	std::vector<AScrollBar*> EntryHpBars;
	std::vector<AImageElement*> EntryGenders;
	std::vector<AImageElement*> EntryStatuses;

	AImageElement* CancelButton = nullptr;

	// 고유 데이터
	const int* TargetCursorReadonly = nullptr;
	EBoxState FirstBoxState = EBoxState::Focused;
	std::vector<EBoxState> EntryBoxStates;
	EBoxState CancelBoxState = EBoxState::Unfocused;

	float SwitchMoveTimer = 0.0f;
	FVector SwitchFromPrevPos;
	FVector SwitchFromOutPos;
	AImageElement* SwitchMoveSwitchFrom = nullptr;
	FVector TargetPrevPos;
	FVector TargetOutPos;
	AImageElement* SwitchMoveTarget = nullptr;
};

