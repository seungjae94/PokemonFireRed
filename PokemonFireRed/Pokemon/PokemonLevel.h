#pragma once
#include <EngineCore/Level.h>

// 포켓몬 게임의 모든 레벨이 공통적으로 수행해야 하는 Tick 동작을 정의하는 클래스
// - 최초 개발 의도: 이벤트 매니저의 Tick을 돌리기 위해 생성
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

