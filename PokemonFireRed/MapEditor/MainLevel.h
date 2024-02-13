#pragma once
#include <vector>
#include <list>
#include <EngineCore/Level.h>

class AButton;

enum class EPassageType
{
	None,
	Block,
	BlockUpDown,
	JumpDown
};

class TileData
{
public:
	EPassageType PassageType = EPassageType::None;
};

class UMainLevel : public ULevel
{
public:
	// constructor destructor
	UMainLevel();
	~UMainLevel();

	// delete Function
	UMainLevel(const UMainLevel& _Other) = delete;
	UMainLevel(UMainLevel&& _Other) noexcept = delete;
	UMainLevel& operator=(const UMainLevel& _Other) = delete;
	UMainLevel& operator=(UMainLevel&& _Other) noexcept = delete;

	static const int ImageContainerLeft;
	static const int ImageContainerTop;

	static int ImageX;
	static int ImageY;
	static int TileCountX;
	static int TileCountY;
	static int ClipCountX;
	static int ClipCountY;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
private:
	std::vector<std::vector<TileData>> Tiles;
	std::list<AButton*> AllButtons;
	AButton* SelectedButton = nullptr;
};

