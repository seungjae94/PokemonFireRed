#pragma once

#include <string>
#include <string_view>

// ���ڿ��� �ٷ�� ���� Ŭ����
// - static �Լ��� ����� ���̹Ƿ� �����ڸ� private���� �����ߴ�.
class UEngineString
{
public:
	static std::string ToUpper(std::string_view _View);
	static std::wstring AnsiToUnicode(std::string_view _View);

protected:

private:
	// constructor destructor
	UEngineString();
	~UEngineString();

	// delete Function
	UEngineString(const UEngineString& _Other) = delete;
	UEngineString(UEngineString&& _Other) noexcept = delete;
	UEngineString& operator=(const UEngineString& _Other) = delete;
	UEngineString& operator=(UEngineString&& _Other) noexcept = delete;
};

