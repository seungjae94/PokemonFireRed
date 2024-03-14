#pragma once
#include <string>
#include "EngineString.h"

// 이름이 있는 오브젝트
// - 굉장히 광범위한 개념이기 때문에 EngineBase에 둔다.
class UNameObject
{
public:
	// constructor destructor
	UNameObject();
	virtual ~UNameObject();			// 최상위 부모 객체의 소멸자는 가상 함수로 선언해야 한다.

	// delete Function
	UNameObject(const UNameObject& _Other) = delete;
	UNameObject(UNameObject&& _Other) noexcept = delete;
	UNameObject& operator=(const UNameObject& _Other) = delete;
	UNameObject& operator=(UNameObject&& _Other) noexcept = delete;

	void SetName(std::string_view _View)
	{
		Name = UEngineString::ToUpper(_View);
	}

	std::string GetName()
	{
		return Name;
	}

protected:

private:
	std::string Name;
};

