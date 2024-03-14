#include "Level.h"

#include <EngineBase/EngineDebug.h>
#include "EngineDebug.h"
#include "Actor.h"
#include "EngineCore.h"

ULevel::ULevel()
{
}

ULevel::~ULevel()
{
	for (std::pair<const int, std::list<AActor*>>& OrderListPair : AllActor)
	{
		std::list<AActor*>& ActorList = OrderListPair.second;
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


void ULevel::LevelTick(float _DeltaTime)
{
	for (std::pair<const int, std::list<AActor*>>& OrderListPair : AllActor)
	{
		int Order = OrderListPair.first;

		if (false == TimeScale.contains(Order))
		{
			TimeScale[Order] = 1.0f;
		}

		float OrderTime = _DeltaTime * TimeScale[Order];

		std::list<AActor*>& ActorList = OrderListPair.second;
		for (AActor* Actor : ActorList)
		{
			Actor->ActiveUpdate(_DeltaTime);
			Actor->DestroyUpdate(_DeltaTime);

			if (false == Actor->IsActive())
			{
				continue;
			}

			Actor->Tick(OrderTime);
			Actor->ChildTick(OrderTime);
		}
	}
}

void ULevel::LevelRender(float _DeltaTime)
{
	for (std::pair<const int, std::list<UImageRenderer*>>& OrderListPair : Renderers)
	{
		std::list<UImageRenderer*>& RendererList = OrderListPair.second;
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
		UEngineDebug::PrintDebugText(GEngine->MainWindow.GetBackBufferImage());
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
				UCollision* Collision = StartIter.operator*();

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
				UImageRenderer* Renderer = StartIter.operator*();

				if (false == Renderer->IsDestroy())
				{
					++StartIter;
					continue;
				}

				StartIter = List.erase(StartIter);
			}
		}
	}

	for (std::pair<const int, std::list<AActor*>>& OrderListPair : AllActor)
	{
		std::list<AActor*>& ActorList = OrderListPair.second;

		std::list<AActor*>::iterator StartIter = ActorList.begin();
		std::list<AActor*>::iterator EndIter = ActorList.end();

		for (; StartIter != EndIter; )
		{
			AActor* Actor = StartIter.operator*();

			if (Actor == nullptr)
			{
				MsgBoxAssert("릴리즈 구조에서 액터가 nullptr인 경우가 존재합니다.");
				return;
			}

			if (false == Actor->IsDestroy())
			{
				// 액터를 삭제하지 않는 경우에도 렌더러나 콜리전이 Destory 할 수 있다. 
				// 앞 부분은 레벨이 렌더러나 콜리전을 리스트에서 지우는 작업이다. 
				// 여기서 액터가 렌더러와 콜리전을실제로 삭제한다.
				Actor->CheckReleaseChild();
				++StartIter;
				continue;
			}

			delete Actor;
			Actor = nullptr;
			StartIter = ActorList.erase(StartIter);
		}
	}
}

void ULevel::ActorInit(AActor* _Actor)
{
	_Actor->SetWorld(this);
	_Actor->BeginPlay();
}
