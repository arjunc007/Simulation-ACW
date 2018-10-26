#include "InputManager.h"
#include "Game.h"


InputManager::InputManager(Game* game) : _game(game)
{
}


InputManager::~InputManager()
{
}

void InputManager::Update()
{
	if (_addSphere)
	{
		_game->AddNewSphere();
		_addSphere = false;
	}
}

void InputManager::AddSphere()
{
	_addSphere = true;
}