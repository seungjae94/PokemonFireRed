#pragma once
#include <string>
#include "EngineString.h"

// �̸��� �ִ� ������Ʈ
// - ������ �������� �����̱� ������ EngineBase�� �д�.
class UNameObject
{
public:
	// constructor destructor
	UNameObject();
	virtual ~UNameObject();			// �ֻ��� �θ� ��ü�� �Ҹ��ڴ� ���� �Լ��� �����ؾ� �Ѵ�.

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

