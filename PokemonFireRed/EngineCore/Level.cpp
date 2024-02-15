#include "Level.h"

#include <EngineBase/EngineDebug.h>
#include "Actor.h"
#include "EngineCore.h"

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
			/*if (Actor == nullptr)
			{
				MsgBoxAssert("업데이트 구조에서 액터가 nullptr인 경우가 존재합니다.")
			}*/

			Actor->ActiveUpdate(_DeltaTime);
			Actor->DestroyUpdate(_DeltaTime);

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
	for (std::pair<const int, std::list<UImageRenderer*>>& Pair : Renderers)
	{
		std::list<UImageRenderer*>& RendererList = Pair.second;
		for (UImageRenderer* Renderer : RendererList)
		{
			if (false == Renderer->IsActive())
			{
				continue;
			}

			Renderer->Render(_DeltaTime);
		}
	}

	// 디버그용 충돌체 렌더링 기능
	if (true == GEngine->IsDebug())
	{
		for (std::pair<const int, std::list<UCollision*>>& OrderListPair : Collisions)
		{
			std::list<UCollision*>& RendererList = OrderListPair.second;
			for (UCollision* Collision : RendererList)
			{
				if (false == Collision->IsActive())
				{
					continue;
				}

				Collision->DebugRender(CameraPos);
			}
		}
	}
}

void ULevel::LevelRelease(float _DeltaTime)
{
	{
		for (std::pair<const int, std::list<UCollision*>>& OrderListPair : Collisions)
		{
			std::list<UCollision*>& List = OrderListPair.second;

			std::list<UCollision*>::iterator StartIter = List.begin();
			std::list<UCollision*>::iterator EndIter = List.end();

			for (; StartIter != EndIter; )
			{
				UCollision* Collision = *StartIter;

				if (false == Collision->IsDestroy())
				{
					++StartIter;
					continue;
				}

				StartIter = List.erase(StartIter);
			}
		}
	}

	{
		for (std::pair<const int, std::list<UImageRenderer*>>& OrderListPair : Renderers)
		{
			std::list<UImageRenderer*>& List = OrderListPair.second;

			std::list<UImageRenderer*>::iterator StartIter = List.begin();
			std::list<UImageRenderer*>::iterator EndIter = List.end();

			for (; StartIter != EndIter; )
			{
				UImageRenderer* Renderer = *StartIter;

				if (false == Renderer->IsDestroy())
				{
					++StartIter;
					continue;
				}

				StartIter = List.erase(StartIter);
			}
		}
	}

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
				MsgBoxAssert("릴리즈 구조에서 액터가 nullptr인 경우가 존재합니다.");
				return;
			}

			if (false == Actor->IsDestroy())
			{
				// 액터를 릴리즈하지 않는 경우
				++StartIter;
				continue;
			}

			delete Actor;
			Actor = nullptr;
			StartIter = ActorList.erase(StartIter);
		}
	}
}
