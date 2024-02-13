#include "MainLevel.h"

#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineResourcesManager.h>
#include <EngineCore/EngineCore.h>
#include "Button.h"
#include "Background.h"

const int UMainLevel::ImageContainerLeft = 0;
const int UMainLevel::ImageContainerTop = 0;
int UMainLevel::ImageX;
int UMainLevel::ImageY;
int UMainLevel::TileCountX;
int UMainLevel::TileCountY;
int UMainLevel::ClipCountX;
int UMainLevel::ClipCountY;


UMainLevel::UMainLevel()
{
}

UMainLevel::~UMainLevel()
{
}

void UMainLevel::BeginPlay()
{
	ULevel::BeginPlay();

	// ���ҽ� �ε�
	UEngineDirectory CurDir;
	CurDir.Move("Resource");

	std::list<UEngineFile> AllFiles = CurDir.AllFile({ ".png" }, true);

	for (UEngineFile& File : AllFiles)
	{
		std::string PathName = File.GetFullPath();
		UEngineResourcesManager::GetInst().LoadImg(PathName);
	}

	// ��ư 
	AButton* PassBlockButton = SpawnActor<AButton>();
	PassBlockButton->SetActorLocation({ 1000, 200 });
	PassBlockButton->SetIcon("P_Block.png");

	AButton* PassBlockUpButton = SpawnActor<AButton>();
	PassBlockUpButton->SetActorLocation({ 1100, 200 });
	PassBlockUpButton->SetIcon("P_BlockUp.png");

	AButton* PassBlockDownButton = SpawnActor<AButton>();
	PassBlockDownButton->SetActorLocation({ 1200, 200 });
	PassBlockDownButton->SetIcon("P_BlockDown.png");

	AllButtons.push_back(PassBlockButton);
	AllButtons.push_back(PassBlockUpButton);
	AllButtons.push_back(PassBlockDownButton);

	// ��� ����
	std::string InputImageName = "InteriorPlayersHouse2FLevelBackground.png";
	UWindowImage* InputImage = UEngineResourcesManager::GetInst().FindImg(InputImageName);

	if (nullptr == InputImage)
	{
		MsgBoxAssert("�Է� �̹��� �̸��� �߸��Ǿ����ϴ�.");
		return;
	}

	ImageX = InputImage->GetScale().iX();
	ImageY = InputImage->GetScale().iY();

	FVector TileCounts = InputImage->GetScale() * (1 / 48.0f);
	TileCountX = TileCounts.iX();
	TileCountY = TileCounts.iY();

	ClipCountX = (TileCountX - 1) / 20 + 1;
	ClipCountY = (TileCountY - 1) / 20 + 1;

	for (int i = 0; i < TileCountX; i++)
	{
		std::vector<TileData> Row;
		for (int j = 0; j < TileCountY; j++)
		{
			TileData Data;
			Data.PassageType = EPassageType::None;
			Row.push_back(Data);
		}
		Tiles.push_back(Row);
	}

	UEngineResourcesManager::GetInst().CuttingImage(InputImageName, ClipCountX, ClipCountY);

	ABackground* Background = SpawnActor<ABackground>();
	Background->SetActorLocation(FVector(ImageContainerLeft + ImageX / 2, ImageContainerTop + ImageY / 2));
	Background->SetImageName(InputImageName);
}

void UMainLevel::Tick(float _DeltaTime)
{
	FVector MousePos = GEngine->MainWindow.GetMousePosition();

	if (false == UEngineInput::IsDown(VK_LBUTTON))
	{
		// �׼��� �߻����� ���� ���
		return;
	}

	// ��ư Ŭ�� �׼� üũ
	for (AButton* Button : AllButtons)
	{
		if (true == Button->IsClicked(MousePos))
		{
			if (SelectedButton == Button)
			{
				Button->ToggleHighlight();
				SelectedButton = nullptr;
				return;
			}

			if (nullptr != SelectedButton)
			{
				SelectedButton->ToggleHighlight();
			}
			Button->ToggleHighlight();
			SelectedButton = Button;
			return;
		}
	}

	// Ÿ�� Ŭ�� �׼� üũ

}