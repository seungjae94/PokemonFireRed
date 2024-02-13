#include "Background.h"
#include "MainLevel.h"
#include "MapEditorUtil.h"

ABackground::ABackground() 
{
}

ABackground::~ABackground() 
{
}

void ABackground::SetImageName(std::string_view _ImageName)
{
	Renderer = CreateImageRenderer(5);
	Renderer->CameraEffectOff();
	Renderer->SetImage(_ImageName);

	for (int i = 0; i < UMainLevel::ClipCountX; i++)
	{
		for (int j = 0; j < UMainLevel::ClipCountY; j++)
		{
			int OneDimIndex = i + UMainLevel::ClipCountX * j;

			Renderer->CreateAnimation(MapEditorUtil::int2ToAnimName(i, j), _ImageName, OneDimIndex, OneDimIndex, 0.0f, false);
		}
	}

	SetClip(X, Y);
}

void ABackground::SetClip(int _X, int _Y)
{
	Renderer->ChangeAnimation(MapEditorUtil::int2ToAnimName(X, Y), true);

	int ScaleX = 48 * 20;
	int ScaleY = 48 * 20;


	if (X == UMainLevel::ClipCountX - 1)
	{
		ScaleX = UMainLevel::ImageX % (48 * 20);
	}

	if (Y == UMainLevel::ClipCountY - 1)
	{
		ScaleY = UMainLevel::ImageY % (48 * 20);
	}

	Renderer->SetTransform({ { 0, 0 }, { ScaleX, ScaleY } });
}

