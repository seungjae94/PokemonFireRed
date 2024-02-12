#include "MapEditorCore.h"
#include "MainLevel.h"

UMapEditorCore::UMapEditorCore()
{
}

UMapEditorCore::~UMapEditorCore()
{
}

void UMapEditorCore::BeginPlay()
{
	MainWindow.SetWindowScale({ 1280, 720 });

	CreateLevel<UMainLevel>("MainLevel");
	ChangeLevel("MainLevel");
}
