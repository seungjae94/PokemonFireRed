#include "PokemonUtil.h"
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EngineCore/EngineResourcesManager.h>
#include "Global.h"
#include "Pokemon.h"
#include "Canvas.h"

UPokemonUtil::UPokemonUtil()
{
}

UPokemonUtil::~UPokemonUtil()
{
}

std::vector<int> UPokemonUtil::FrameCountVectorToIndexVector(std::vector<int> _FrameCounts)
{
	std::vector<int> Indexes;

	int Index = 0;
	for (int FrameCount : _FrameCounts)
	{
		for (int i = 0; i < FrameCount; i++)
		{
			Indexes.push_back(Index);
		}
		Index++;
	}

	return Indexes;
}

FVector UPokemonUtil::GetImageScale(UImageRenderer* _Renderer)
{
	UWindowImage* Image = _Renderer->GetImage();
	return Image->ImageInfo(0).CuttingTrans.GetScale();;
}

FVector UPokemonUtil::GetRenderScale(UImageRenderer* _Renderer)
{
	return GetImageScale(_Renderer) * Global::FloatPixelSize;
}

void UPokemonUtil::AlignImage(
	UImageRenderer* _Renderer, EPivotType _PivotType)
{
	FVector RenderScale = GetRenderScale(_Renderer);

	FVector Pos;
	switch (_PivotType)
	{
	case EPivotType::LeftTop:
		Pos = RenderScale.Half2D();
		break;
	case EPivotType::LeftBot:
		Pos = RenderScale.Half2D() + FVector(0.0f, -RenderScale.Y + Global::FloatPixelSize);
		break;
	case EPivotType::RightTop:
		Pos = RenderScale.Half2D() + FVector(-RenderScale.X + Global::FloatPixelSize, 0.0f);
		break;
	case EPivotType::RightBot:
		Pos = RenderScale.Half2D() + FVector(-RenderScale.X + Global::FloatPixelSize, -RenderScale.Y + Global::FloatPixelSize);
		break;
	case EPivotType::CenterTop:
	{
		int PixelScaleX = RenderScale.iX() / Global::PixelSize;
		int PixelScaleY = RenderScale.iY() / Global::PixelSize;
		Pos = RenderScale.Half2D() + FVector((PixelScaleX / 2) * Global::FloatPixelSize, 0.0f);
	}
		break;
	case EPivotType::CenterBot:
	{
		int PixelScaleX = RenderScale.iX() / Global::PixelSize;
		int PixelScaleY = RenderScale.iY() / Global::PixelSize;
		Pos = RenderScale.Half2D() + FVector((PixelScaleX / 2) * Global::FloatPixelSize, -RenderScale.Y + Global::FloatPixelSize);
	}
		break;
	default:
		break;
	}

	_Renderer->SetTransform({ Pos, RenderScale });
}


void UPokemonUtil::PlaceImageOnScreen(
	UImageRenderer* _Renderer, 
	EPivotType _PivotType, 
	int _ScreenPixelX, int _ScreenPixelY)
{
	_Renderer->CameraEffectOff();
	AlignImage(_Renderer, _PivotType);
	FVector Pos = _Renderer->GetPosition();
	FVector RendererScale = _Renderer->GetTransform().GetScale();

	switch (_PivotType)
	{
	case EPivotType::LeftTop:
		break;
	case EPivotType::LeftBot:
		Pos += FVector(0.0f, Global::FloatScreenY - Global::FloatPixelSize);
		break;
	case EPivotType::RightTop:
		Pos += FVector(Global::FloatScreenX - Global::FloatPixelSize, 0.0f);
		break;
	case EPivotType::RightBot:
		Pos += FVector(Global::FloatScreenX - Global::FloatPixelSize, Global::FloatScreenY - Global::FloatPixelSize);
		break;
	case EPivotType::CenterTop:
	{
		int PixelScaleX = Global::ScreenX / Global::PixelSize;
		int PixelScaleY = Global::ScreenY / Global::PixelSize;
		Pos += FVector((PixelScaleX / 2) * Global::FloatPixelSize, 0.0f);
	}
		break;
	case EPivotType::CenterBot:
	{
		int PixelScaleX = Global::ScreenX / Global::PixelSize;
		int PixelScaleY = Global::ScreenY / Global::PixelSize;
		Pos += FVector((PixelScaleX / 2) * Global::FloatPixelSize, Global::FloatScreenY - Global::FloatPixelSize);
	}
		break;
	default:
		break;
	}

	FVector FinalPos = Pos + UPokemonUtil::PixelVector(_ScreenPixelX, _ScreenPixelY);
	_Renderer->SetPosition(FinalPos);
}

void UPokemonUtil::CreatePokemonMiniMoveAnimations(UImageRenderer* _Renderer)
{
	if (nullptr == _Renderer)
	{
		MsgBoxAssert("nullptr인 렌더러에서 PokemonMini Move 애니메이션을 만들려고 했습니다.");
		return;
	}

	if (nullptr == _Renderer->GetImage())
	{
		_Renderer->SetImage(RN::PokemonMini);
	}

	for (EPokedexNo No : UPokemonDB::GetImplementedSpeciesNo())
	{
		int Idx = static_cast<int>(No) * 2 - 2;
		_Renderer->CreateAnimation(
			Global::PokemonMiniMovePrefix + UPokemonDB::GetSpeciesName(No),
			RN::PokemonMini, 
			Idx, Idx + 1, 1.0f / 6, true);
	}
}

void UPokemonUtil::CreatePokemonMiniStopAnimations(UImageRenderer* _Renderer)
{
	if (nullptr == _Renderer)
	{
		MsgBoxAssert("nullptr인 렌더러에서 PokemonMini Stop 애니메이션을 만들려고 했습니다.");
		return;
	}

	if (nullptr == _Renderer->GetImage())
	{
		_Renderer->SetImage(RN::PokemonMini);
	}

	for (EPokedexNo No : UPokemonDB::GetImplementedSpeciesNo())
	{
		int Idx = static_cast<int>(No) * 2 - 2;
		_Renderer->CreateAnimation(
			Global::PokemonMiniStopPrefix + UPokemonDB::GetSpeciesName(No),
			RN::PokemonMini,
			Idx, Idx, 1.0f / 6, true);
	}
}

void UPokemonUtil::CreatePokemonFrontAnimations(UImageRenderer* _Renderer)
{
	if (nullptr == _Renderer)
	{
		MsgBoxAssert("nullptr인 렌더러에서 PokemonFront 애니메이션을 만들려고 했습니다.");
		return;
	}

	if (nullptr == _Renderer->GetImage())
	{
		_Renderer->SetImage(RN::PokemonFront);
	}

	for (EPokedexNo No : UPokemonDB::GetImplementedSpeciesNo())
	{
		int Idx = static_cast<int>(No) * 2  - 2;
		_Renderer->CreateAnimation(
			Global::PokemonFrontPrefix + UPokemonDB::GetSpeciesName(No),
			RN::PokemonFront,
			Idx, Idx, 1.0f / 6, false);
	}
}

void UPokemonUtil::CreatePokemonBackAnimations(UImageRenderer* _Renderer)
{
}

void UPokemonUtil::LoadAllResourcesIn(std::string_view _DirName)
{
	UEngineDirectory CurDir;
	CurDir.MoveToSearchChild("Resources");
	CurDir.Move(_DirName);

	std::list<UEngineFile> AllFiles = CurDir.AllFile({ ".bmp", ".png" }, true);
	for (UEngineFile& File : AllFiles)
	{
		UEngineResourcesManager::GetInst().LoadImg(File.GetFullPath());
	}
}
