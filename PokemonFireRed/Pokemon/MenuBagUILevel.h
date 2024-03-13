#pragma once
#include "BagUILevel.h"

class UMenuBagUILevel : public UBagUILevel
{
public:
	// constructor destructor
	UMenuBagUILevel();
	~UMenuBagUILevel();

	// delete Function
	UMenuBagUILevel(const UMenuBagUILevel& _Other) = delete;
	UMenuBagUILevel(UMenuBagUILevel&& _Other) noexcept = delete;
	UMenuBagUILevel& operator=(const UMenuBagUILevel& _Other) = delete;
	UMenuBagUILevel& operator=(UMenuBagUILevel&& _Other) noexcept = delete;

protected:

private:
	std::string PrevMapName;
	void LevelStart(ULevel* _PrevLevel) override;

	void SelectTarget() override;
	void CancelTargetSelection() override;
	void SelectAction() override;
};

