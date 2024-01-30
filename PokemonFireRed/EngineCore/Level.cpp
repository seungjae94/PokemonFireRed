#include "Level.h"

#include <EngineBase/EngineDebug.h>
#include "Actor.h"

ULevel::ULevel()
{
}

ULevel::~ULevel()
{
	for (std::pair<const int, std::list<AActor*>>& Pair : AllActor)
	{
		int Order = Pair.first;
		std::list<AActor*>& ActorList = Pair.second;

		for (AActor* Actor : ActorList)
		{
			if (nullptr == Actor)
			{
				continue;
			}

			delete Actor;
			Actor = nullptr;
		}
	}
	AllActor.clear();
}

void ULevel::ActorInit(AActor* _Actor)
{
	_Actor->SetWorld(this);
	_Actor->BeginPlay();
}

void ULevel::LevelTick(float _DeltaTime)
{
	for (std::pair<const int, std::list<AActor*>>& Pair : AllActor)
	{
		std::list<AActor*>& ActorList = Pair.second;
		for (AActor* Actor : ActorList)
		{
			if (Actor == nullptr)
			{
				MsgBoxAssert("������Ʈ �������� ���Ͱ� nullptr�� ��찡 �����մϴ�.")
			}

			if (false == Actor->IsActive())
			{
				continue;
			}
	
			Actor->Tick(_DeltaTime);
		}
	}
}

void ULevel::LevelRender(float _DeltaTime)
{
}

void ULevel::LevelRelease(float _DeltaTime)
{
	for (std::pair<const int, std::list<AActor*>>& Pair : AllActor)
	{
		std::list<AActor*>& ActorList = Pair.second;

		std::list<AActor*>::iterator StartIter = ActorList.begin();
		std::list<AActor*>::iterator EndIter = ActorList.end();

		for (; StartIter != EndIter; )
		{
			AActor* Actor = *StartIter;

			if (Actor == nullptr)
			{
				MsgBoxAssert("������ �������� ���Ͱ� nullptr�� ��찡 �����մϴ�.")
			}

			if (false == Actor->IsDestroy())
			{
				// ���͸� ���������� �ʴ� ���
				++StartIter;
				continue;
			}

			delete Actor;
			Actor = nullptr;
			StartIter = ActorList.erase(StartIter);
		}
	}
}
