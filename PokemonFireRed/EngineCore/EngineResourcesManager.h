#pragma once
#include <string>
#include <string_view>
#include <map>

class UWindowImage;

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

	static UEngineResourcesManager& GetInst()
	{
		// ó�� ȣ��� ������ ������ GetInst �Լ� ���� ���� Inst�� �����ȴ�.
		static UEngineResourcesManager Inst = UEngineResourcesManager();
		return Inst;
	}

	// ���� ��θ� �޾Ƽ� �̹��� ��ü�� �����ϰ� ���ҽ� �Ŵ����� �����Ѵ�.
	// - �̹��� ��ü�� Load �Լ��� ȣ���� ���� ��ο��� ���� �̹����� �ε��Ѵ�.
	UWindowImage* LoadImg(std::string_view _Path);

	// ���� ��ο� �̹��� �̸��� �޾Ƽ� �̹��� ��ü�� �����ϰ� ���ҽ� �Ŵ����� �����Ѵ�.
	// - �̹��� ��ü�� Load �Լ��� ȣ���� ���� ��ο��� ���� �̹����� �ε��Ѵ�.
	UWindowImage* LoadImg(std::string_view _Path, std::string_view _Name);

	// �̹��� �̸��� �޾Ƽ� ���ҽ� �Ŵ����� ���� �ʿ��� �̹����� ã���ش�.
	UWindowImage* FindImg(std::string_view _Name);

	// Ư�� ���� ���� �̹����� ���� �̹����� �����´�.
	UWindowImage* LoadFolder(std::string_view _Path);

	UWindowImage* LoadFolder(std::string_view _Path, std::string_view _Name);

	// ������ �̹� �ε��� �̹����� ���� ���� �̹����� �߶��ش�.
	void CuttingImage(std::string_view _Name, int _X, int _Y);

protected:

private:
	UEngineResourcesManager();			// �̱������� ��ü�� �����ϱ� ���� �����ڸ� �����.
	~UEngineResourcesManager();			// �ε��� �̹����� ������ å���� ������.

	// ������� ���ӿ� �ε��� �̹���
	std::map<std::string, UWindowImage*> Images;
};

