#pragma once
#include "EventTrigger.h"
#include "PokemonText.h"

class UPokemonLevel;

class ADialogueActor : public AEventTrigger
{
public:
	// constructor destructor
	ADialogueActor();
	~ADialogueActor();

	// delete Function
	ADialogueActor(const ADialogueActor& _Other) = delete;
	ADialogueActor(ADialogueActor&& _Other) noexcept = delete;
	ADialogueActor& operator=(const ADialogueActor& _Other) = delete;
	ADialogueActor& operator=(ADialogueActor&& _Other) noexcept = delete;

	void SetDialogue(const std::vector<std::wstring>& _Dialogue)
	{
		Dialogue = _Dialogue;
	}

	void SetTextColor(EFontColor _TextColor)
	{
		TextColor = _TextColor;
	}

	void RegisterPredefinedEvent() override;

	// ���� �̹��� ���� ���� ��ȭ�� ������ �ܼ� ������Ʈ�� ������ �� ���
	static ADialogueActor* GenerateObject(UPokemonLevel* _Level, std::string_view _Name, const FTileVector _Point, EFontColor _TextColor, const std::vector<std::wstring>& _Dialogue);

protected:
	void BeginPlay() override;
private:
	UImageRenderer* Renderer = nullptr;
	std::vector<std::wstring> Dialogue;
	EFontColor TextColor = EFontColor::Black;

	FTileVector BeforeChatDirection = FTileVector::Zero;
};

