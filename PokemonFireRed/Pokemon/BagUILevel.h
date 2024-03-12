#pragma once
#include "PokemonLevel.h"
#include "BagCanvas.h"

class UBattler;

class UBagUILevel : public UPokemonLevel
{
private:
	enum class EState
	{
		None,
		TargetSelect,
		ActionSelect,
		BattleModeItemUsageCheck,
		End
	};

public:
	enum class EItemUsage
	{
		None,
		Used,
		NotUsed,
	};

public:
	// constructor destructor
	UBagUILevel();
	~UBagUILevel();

	// delete Function
	UBagUILevel(const UBagUILevel& _Other) = delete;
	UBagUILevel(UBagUILevel&& _Other) noexcept = delete;
	UBagUILevel& operator=(const UBagUILevel& _Other) = delete;
	UBagUILevel& operator=(UBagUILevel&& _Other) noexcept = delete;

	bool IsBattleMode() const;

	const FItem* GetTargetItem();

	void SetItemUsage(EItemUsage _Usage);

protected:

private:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void LevelStart(ULevel* _PrevLevel) override;
	void LevelEnd(ULevel* _NextLevel) override;

	// 하위 요소
	ABagCanvas* Canvas = nullptr;
	
	// 상태
	EState State = EState::None;
	bool BattleMode = false;
	bool BattleModeItemUsageChecker = false;

	// 상태 틱
	void ProcessTargetSelect();
	void ProcessActionSelect();
	void ProcessBattleModeItemUsageCheck();

	// 고유 데이터
	int Page = 0;
	std::vector<int> StartIndexMemory = { 0, 0, 0 };
	std::vector<int> TargetIndexMemory = { 0, 0, 0 };
	std::string PrevLevelName;

	// 배틀 아이템 사용 기능 지원
	UBattler* PlayerBattler = nullptr;
	EItemUsage ItemUsage = EItemUsage::None;

	// 로직 함수
	void ScrollUp();
	void ScrollDown();
	void FixIndexes();

	// 유틸 함수
	void RefreshPage();
	static EItemType PageToItemType(int _Page);
	static std::string PageToBackgroundName(int _Page);
	static int ItemTypeToPage(EItemType _ItemType);
};

