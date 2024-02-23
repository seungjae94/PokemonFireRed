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
	void Close();

	void SetActive(bool _Active, float _ActiveTime = 0.0f) override;
protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
private:
	void Refresh();

	enum MenuIndex
	{
		Pokedex,
		Pokemon,
		Bag,
		Player,
		Exit,
	};

	bool IsFirstTick = true;

	UImageRenderer* MenuWindowRenderer = nullptr;
	UImageRenderer* MenuWindowExplainRenderer = nullptr;
	UImageRenderer* ArrowRenderer = nullptr;

	APokemonText* MenuExplainText = nullptr;
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


	// 커서 관련
	static int Cursor;
	static int MenuCount;
	void IncCursor();
	void DecCursor();
	void MoveCursor(int _Cursor);

	// 현재 커서 위치에 따라 메뉴 액션 실행
	void MenuAction();

	
	int GetMenuIndex()
	{
		return static_cast<int>(MenuNames.size() - MenuCount + Cursor);
	}

	void DrawMenuWindow();
	void DrawMenuTexts();
	void DrawArrow();
	void DrawExplainText();
	
	FVector GetArrowPos();
};

