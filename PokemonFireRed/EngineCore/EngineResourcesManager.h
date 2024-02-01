#pragma once
#include <string>
#include <string_view>
#include <map>

class UWindowImage;

// 리소스를 관리하는 싱글톤 객체
// - 굳이 싱글톤을 사용하지 않고 static 멤버만 사용해도 되지만 연습삼아 싱글톤으로 구현했다.
class UEngineResourcesManager
{
public:
	// delete Function
	UEngineResourcesManager(const UEngineResourcesManager& _Other) = delete;
	UEngineResourcesManager(UEngineResourcesManager&& _Other) noexcept = delete;
	UEngineResourcesManager& operator=(const UEngineResourcesManager& _Other) = delete;
	UEngineResourcesManager& operator=(UEngineResourcesManager&& _Other) noexcept = delete;

	static UEngineResourcesManager& GetInst()
	{
		// 처음 호출시 데이터 영역에 GetInst 함수 전용 변수 Inst가 생성된다.
		static UEngineResourcesManager Inst = UEngineResourcesManager();
		return Inst;
	}

	// 절대 경로를 받아서 이미지 객체를 생성하고 리소스 매니저에 저장한다.
	// - 이미지 객체의 Load 함수를 호출해 파일 경로에서 실제 이미지를 로딩한다.
	UWindowImage* LoadImg(std::string_view _Path);

	// 절대 경로와 이미지 이름을 받아서 이미지 객체를 생성하고 리소스 매니저에 저장한다.
	// - 이미지 객체의 Load 함수를 호출해 파일 경로에서 실제 이미지를 로딩한다.
	UWindowImage* LoadImg(std::string_view _Path, std::string_view _Name);

	// 이미지 이름을 받아서 리소스 매니저가 가진 맵에서 이미지를 찾아준다.
	UWindowImage* FindImg(std::string_view _Name);

	// 특정 폴더 안의 이미지를 전부 이미지로 가져온다.
	UWindowImage* LoadFolder(std::string_view _Path);

	UWindowImage* LoadFolder(std::string_view _Path, std::string_view _Name);

	// 기존에 이미 로드한 이미지를 여러 개의 이미지로 잘라준다.
	void CuttingImage(std::string_view _Name, int _X, int _Y);

protected:

private:
	UEngineResourcesManager();			// 싱글톤으로 객체를 생성하기 위해 생성자를 숨긴다.
	~UEngineResourcesManager();			// 로드한 이미지를 삭제할 책임을 가진다.

	// 현재까지 게임에 로딩된 이미지
	std::map<std::string, UWindowImage*> Images;
};

