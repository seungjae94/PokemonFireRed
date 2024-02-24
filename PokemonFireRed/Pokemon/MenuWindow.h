#pragma once
#include "Page.h"
#include "PokemonText.h"

class AEventTrigger;

class AMenuWindow : public APage
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
protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
private:
	void Refresh();

	enum EMenuIndex
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
	ACursor* Cursor = nullptr;
	static int MenuCount;

	// 현재 커서 위치에 따라 메뉴 액션 실행
	void MenuAction();

	
	int GetMenuIndex()
	{
		return static_cast<int>(MenuNames.size() - MenuCount + Cursor->GetCursor());
	}

	void RefreshMenuWindow();
	void RefreshMenuTexts();
	void RefreshExplainText();
};

