#pragma once

#include <Windows.h>
#include "ContactManifold.h"
#include "Sphere.h"
#include "Plane.h"
#include "Bowl.h"
#include <vector>
#include<fstream>

class Game
{
public:
	Game(HDC hdc);
	~Game(void);

	void Update();
	void AddNewSphere();
	void PauseGame();
	void ResetGame();
	void ChangeTimescale(float);
	void ZoomCamera(float);
	void MoveCameraX(float);
	void MoveCameraY(float);
	void RotateCameraX(float);
	void RotateCameraY(float);
	void ResetCamera();
	void ChangeElasticity(float);
	void ChangeFriction(float);
	void RemoveLastItem();
	void RemoveTopTray();
	void AddTopTray();
	void RemoveBottomTray();
	void AddBottomTray();
	void WriteToConsole();

public:
	static float e, k;

private:
	void SimulationLoop();
	void CalculateObjectPhysics();
	void DynamicCollisionDetection();
	void DynamicCollisionResponse();
	void UpdateObjectPhysics();
	void Render();

private:
	HDC   m_hdc;
	float m_dt;
	int	  m_fps;
	float m_previousTime;
	float timeScale;					//Smaller value means slower play
	bool notPaused = true;
	Vector3f camera, focus;
	float fovy;
	std::vector <Sphere*> m_spheres;
	Plane trays[3];
	Plane walls[4];
	Bowl *bowl;
	ContactManifold *m_itemManifold;
	ContactManifold *m_surfaceManifold;
	LARGE_INTEGER start, end, frequency;
};

