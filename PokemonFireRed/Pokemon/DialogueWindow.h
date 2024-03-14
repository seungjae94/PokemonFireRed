#pragma once
#include <EngineCore/Actor.h>
#include "PokemonMsgBox.h"

class UImageRenderer;
class AText;
enum class EFontColor;

class ADialogueWindow : public AActor
{
private:
	enum class EState
	{
		Hide,
		Show,
		End
	};
public:
	// constructor destructor
	ADialogueWindow();
	~ADialogueWindow();

	// delete Function
	ADialogueWindow(const ADialogueWindow& _Other) = delete;
	ADialogueWindow(ADialogueWindow&& _Other) noexcept = delete;
	ADialogueWindow& operator=(const ADialogueWindow& _Other) = delete;
	ADialogueWindow& operator=(ADialogueWindow&& _Other) noexcept = delete;
	
	void SetActive(bool _Value, float _ActiveTime = 0.0f) override;

	bool IsHide() const
	{
		return State == EState::Hide;
	}

	bool IsEnd() const
	{
		return State == EState::End;
	}

	void Hide()
	{
		State = EState::Hide;
		SetActive(false);
	}

	void Open(const std::vector<std::wstring>& _Dialogue, EFontColor _Color, int _LineSpace = 14);
protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
private:
	APokemonMsgBox* MsgBox = nullptr;

	// 상태
	EState State = EState::Hide;

	// 데이터
	int Index = 0;
	std::vector<std::wstring> Dialogue;

	// 상태 틱 함수
	void ProcessShow();

	// 유틸 함수
	void PlayClickSE();
};

