#pragma once

#include <string>
#include <string_view>

// 문자열을 다루기 위한 클래스
// - static 함수만 사용할 것이므로 생성자를 private으로 설정했다.
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

