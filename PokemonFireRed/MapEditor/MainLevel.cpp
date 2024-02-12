#include "MainLevel.h"
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EngineCore/EngineResourcesManager.h>
#include "Button.h"

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
	AButton* Button = SpawnActor<AButton>();
	Button->SetActorLocation({ 500, 500 });
}
