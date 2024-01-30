#include <EngineCore/EngineCore.h>
#include <Pokemon/PokemonCore.h>

//ENGINESTART(PokemonCore);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, 
	_In_opt_ HINSTANCE hPrevInstance, 
	_In_ LPWSTR    lpCmdLine, 
	_In_ int       nCmdShow) 
{ 
	LeakCheck; 
	PokemonCore MainCore = PokemonCore(); 
	EngineCore::EngineStart(hInstance, &MainCore); 
}