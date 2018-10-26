#pragma once

class Game;

class InputManager
{
public:
	InputManager(Game*);
	~InputManager();

public:
	void Update();
	void AddSphere();

private:
	Game* _game = nullptr;
	bool _addSphere = false;
};