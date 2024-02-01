#pragma once

#pragma once

#include <string>
#include <string_view>

// ���ڿ��� �ٷ�� ���� Ŭ����
// - static �Լ��� ����� ���̹Ƿ� �����ڸ� private���� �����ߴ�.
class UEngineString
{
public:
	static std::string ToUpper(std::string_view View);
	static std::wstring AnsiToUniCode(std::string_view View);

protected:

private:
	UEngineString();
	~UEngineString();

	// delete Function
	UEngineString(const UEngineString& _Other) = delete;
	UEngineString(UEngineString&& _Other) noexcept = delete;
	UEngineString& operator=(const UEngineString& _Other) = delete;
	UEngineString& operator=(UEngineString&& _Other) noexcept = delete;
};

