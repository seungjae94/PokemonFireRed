#pragma once
#include <EngineCore/Level.h>

// ���ϸ� ������ ��� ������ ���������� �����ؾ� �ϴ� Tick ������ �����ϴ� Ŭ����
// - ���� ���� �ǵ�: �̺�Ʈ �Ŵ����� Tick�� ������ ���� ����
class UPokemonLevel : public ULevel
{
public:
	// constructor destructor
	UPokemonLevel();
	~UPokemonLevel();

	// delete Function
	UPokemonLevel(const UPokemonLevel& _Other) = delete;
	UPokemonLevel(UPokemonLevel&& _Other) noexcept = delete;
	UPokemonLevel& operator=(const UPokemonLevel& _Other) = delete;
	UPokemonLevel& operator=(UPokemonLevel&& _Other) noexcept = delete;

protected:
	void Tick(float _DeltaTime) override;
private:

};

