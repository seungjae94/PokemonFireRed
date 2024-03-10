#pragma once
#include "PokemonLevel.h"
#include "BagCanvas.h"

class UBagUILevel : public UPokemonLevel
{
private:
	enum class EState
	{
		None,
		TargetSelect,
		ActionSelect,
		End
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

protected:

private:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void LevelStart(ULevel* _PrevLevel) override;
	void LevelEnd(ULevel* _NextLevel) override;

	// ���� ���
	ABagCanvas* Canvas = nullptr;
	
	// ����
	EState State = EState::None;

	// ���� ������
	int Page = 0;
	std::vector<int> StartIndexMemory = { 0, 0, 0 };
	std::vector<int> TargetIndexMemory = { 0, 0, 0 };
	std::string PrevLevelName;

	// ��ƿ �Լ�
	static EItemType PageToItemType(int _Page);
	static std::string PageToBackgroundName(int _Page);
	static int ItemTypeToPage(EItemType _ItemType);
};

