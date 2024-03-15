#pragma once
#include "PokemonLevel.h"
#include "BagCanvas.h"

class EBagUIState
{
public:
	static const EBagUIState None;
	static const EBagUIState TargetSelect;
	static const EBagUIState ActionSelect;

	void operator=(const EBagUIState& _Other);
	bool operator==(const EBagUIState& _Other);
	bool operator<(const EBagUIState& _Other);
	bool operator>(const EBagUIState& _Other);

protected:
	EBagUIState() : Index(MaxIndex++) {}
	int Index = 0;
	static int MaxIndex;
};

class UBagUILevel : public UPokemonLevel
{
public:
	// constructor destructor
	UBagUILevel();
	~UBagUILevel();

	// delete Function
	UBagUILevel(const UBagUILevel& _Other) = delete;
	UBagUILevel(UBagUILevel&& _Other) noexcept = delete;
	UBagUILevel& operator=(const UBagUILevel& _Other) = delete;
	UBagUILevel& operator=(UBagUILevel&& _Other) noexcept = delete;

	const FItem* GetTargetItem();
protected:
	ABagCanvas* Canvas = nullptr;
	EBagUIState State = EBagUIState::None;

	int Page = 0;
	std::vector<int> StartIndexMemory = { 0, 0, 0 };
	std::vector<int> TargetIndexMemory = { 0, 0, 0 };

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void LevelStart(ULevel* _PrevLevel) override;

	// ���� ƽ
	void ProcessTargetSelect();
	void ProcessActionSelect();

	// ���� ���� �Լ�
	virtual void SelectTarget() {};
	virtual void CancelTargetSelection() {};
	virtual void SelectAction() {};

	// ��ƿ �Լ�
	EItemType GetCurItemType() const;
	static void PlayClickSE();
private:
	static bool IsCommonResourcesLoaded;

	// ���� �Լ�
	void ScrollUp();
	void ScrollDown();
	void FixIndexes();

	// ��ƿ �Լ�
	void RefreshPage();
	static EItemType PageToItemType(int _Page);
	static std::string PageToBackgroundName(int _Page);
	static int ItemTypeToPage(EItemType _ItemType);
};

