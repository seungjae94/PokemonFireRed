#pragma once
#include "UIElement.h"
#include "PokemonText.h"

class AEventTrigger;

class AMenuWindow : public AUIElement
{
public:
	// constructor destructor
	AMenuWindow();
	~AMenuWindow();

	// delete Function
	AMenuWindow(const AMenuWindow& _Other) = delete;
	AMenuWindow(AMenuWindow&& _Other) noexcept = delete;
	AMenuWindow& operator=(const AMenuWindow& _Other) = delete;
	AMenuWindow& operator=(AMenuWindow&& _Other) noexcept = delete;

	void Open();

	void AllRenderersActiveOn();
	void AllRenderersActiveOff();

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void Sync(AUIElement* _Other) override
	{
		AMenuWindow* Other = dynamic_cast<AMenuWindow*>(_Other);

		if (nullptr == Other)
		{
			MsgBoxAssert(_Other->GetName() +
				"은 MenuWindow가 아닙니다. Sync 함수에서 다운 캐스팅이 실패했습니다.");
			return;
		}

		MenuCount = Other->MenuCount;
		Cursor = Other->Cursor;
		OptionCount = Other->OptionCount;

		MoveCursor(Cursor);
	}
private:
	bool IsFirstTick = true;

	UImageRenderer* MenuWindowRenderer = nullptr;
	UImageRenderer* MenuWindowExplainRenderer = nullptr;
	UImageRenderer* ArrowRenderer = nullptr;

	int MenuCount = 3;
	std::vector<APokemonText*> MenuTexts;
	std::vector<std::wstring> MenuNames = {L"POKéDEX", L"POKéMON", L"BAG", L"RED", L"EXIT"};
	std::vector<std::wstring> MenuExplains = {
		LR"(A device that records POKéMON secrets
upon meeting or catching them.)",
		LR"(Check and organize POKéMON that are
traveling with you in your party.)",
		LR"(Equipped with pockets for storing items
you bought, received, or found.)",
		LR"(Check your money and other game data.)",
		LR"(Close this MENU window)"
	};

	APokemonText* MenuExplainText = nullptr;

	int Cursor = 0;
	int OptionCount = 3;

	void IncCursor();
	void DecCursor();
	void MoveCursor(int _Cursor);
	
	int GetMenuIndex()
	{
		return static_cast<int>(MenuNames.size() - MenuCount + Cursor);
	}
	
	FVector GetArrowPos();

	AEventTrigger* MenuWindowOpenTrigger = nullptr;
	AEventTrigger* MenuWindowCloseTrigger = nullptr;
};

