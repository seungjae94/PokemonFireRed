#pragma once
#include <string>

class MapEditorUtil
{
public:
	// constructor destructor
	MapEditorUtil();
	~MapEditorUtil();

	// delete Function
	MapEditorUtil(const MapEditorUtil& _Other) = delete;
	MapEditorUtil(MapEditorUtil&& _Other) noexcept = delete;
	MapEditorUtil& operator=(const MapEditorUtil& _Other) = delete;
	MapEditorUtil& operator=(MapEditorUtil&& _Other) noexcept = delete;

	static std::string int2ToAnimName(int _X, int _Y)
	{
		return std::to_string(_X) + std::string("_") + std::to_string(_Y);
	}

protected:

private:

};

