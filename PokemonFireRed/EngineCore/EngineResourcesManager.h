#pragma once

#include <map>
#include <string>
#include <string_view>
#include <EnginePlatform/WindowImage.h>

// ���ҽ��� �����ϴ� �̱��� ��ü
// - ���� �̱����� ������� �ʰ� static ����� ����ص� ������ ������� �̱������� �����ߴ�.
class UEngineResourcesManager
{
public:
	// delete Function
	UEngineResourcesManager(const UEngineResourcesManager& _Other) = delete;
	UEngineResourcesManager(UEngineResourcesManager&& _Other) noexcept = delete;
	UEngineResourcesManager& operator=(const UEngineResourcesManager& _Other) = delete;
	UEngineResourcesManager& operator=(UEngineResourcesManager&& _Other) noexcept = delete;

	// [�̹��� �ε� �Լ�]

	// ���� ��θ� �޾Ƽ� �̹��� ��ü�� �����ϰ� ���ҽ� �Ŵ����� �����Ѵ�.
	UWindowImage* LoadImg(std::string_view _Path);

	// ���� ��ο� �̹��� �̸��� �޾Ƽ� �̹��� ��ü�� �����ϰ� ���ҽ� �Ŵ����� �����Ѵ�.
	UWindowImage* LoadImg(std::string_view _Path, std::string_view _Name);

	// �̹��� �̸��� �޾Ƽ� ���ҽ� �Ŵ����� ���� �ʿ��� �̹����� ã���ش�.
	UWindowImage* FindImg(std::string_view _Name);

	static UEngineResourcesManager& GetInst()
	{
		// ó�� ȣ��� ������ ������ GetInst �Լ� ���� ���� Inst�� �����ȴ�.
		static UEngineResourcesManager Inst = UEngineResourcesManager();
		return Inst;
	}

protected:
	// �̱������� ��ü�� �����ϱ� ���� �����ڸ� �����.
	UEngineResourcesManager();
	~UEngineResourcesManager();

private:
	// ������� ���ӿ� �ε��� �̹���
	std::map<std::string, UWindowImage*> Images;
};

