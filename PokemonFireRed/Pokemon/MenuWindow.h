#pragma once
#include "UIElement.h"

class APokemonText;
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
private:
	UImageRenderer* MenuWindowRenderer = nullptr;
	UImageRenderer* MenuWindowExplainRenderer = nullptr;
	UImageRenderer* ArrowRenderer = nullptr;

	std::vector<APokemonText*> MenuTexts;
	APokemonText* MenuExplainText = nullptr;

	int Cursor = 0;
	int OptionCount = 3;

	void IncCursor();
	void DecCursor();
	void MoveCursor(int _Cursor);
	FVector GetArrowPos(int _Cursor);

	AEventTrigger* MenuWindowOpenTrigger = nullptr;
	AEventTrigger* MenuWindowCloseTrigger = nullptr;
};

