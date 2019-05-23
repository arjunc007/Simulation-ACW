//Using scale of 1m = 1units
//or 1cm = 0.01units
//=> 1kg = 1 units. 0.02kg = 0.02 units
// g = 9.81 m/s2 => 9.81 units/s
//Multiply everything with scale


#include "Game.h"
#include <gl\gl.h>                                // Header File For The OpenGL32 Library
#include <gl\GLU.h>
#include<stdio.h>
#include<ostream>
#include<stdlib.h>
#include "Logger.h"
//#include<typeinfo>
//#include<fstream>

float Game::e;
float Game::k;

Game::Game(HDC hdc) : m_hdc(hdc), m_previousTime(0)
{
	m_inputManager = std::make_unique<InputManager>(this);
	//Add Trays
	//3 cm below top
	trays[0] = Plane(Vector3f(0, 1, 0), Vector3f(-0.05f, 0.045f, 0.05f), Vector3f(0.05f, 0.045f, 0.05f), 0.1f);
	trays[0].SetColor(1, 1, 0);
	trays[0].SetFriction(0.0f);

	//7.5cm below top
	trays[1] = Plane(Vector3f(0, 1, 0), Vector3f(-0.05f, 0, 0.05f), Vector3f(0.05f, 0, 0.05f), 0.1f);
	trays[1].SetColor(1, 1, 0);
	//trays[1].SetFriction(0.3f);
	trays[1].AddHoles();

	//12cm down
	trays[2] = Plane(Vector3f(0, 1, 0), Vector3f(-0.05f, -0.045f, 0.05f), Vector3f(0.05f, -0.045f, 0.05f), 0.1f);
	trays[2].SetColor(1, 1, 0);
	//trays[2].SetFriction(0.3f);
	
	//Add box - 15cm high, 10x10cm area
	walls[0] = Plane(Vector3f(1, 0, 0), Vector3f(-0.05f, -0.075f, -0.05f), Vector3f(-0.05f, -0.075f, 0.05f), 0.15f);
	walls[0].SetColor(0, 1, 1);
	walls[1] = Plane(Vector3f(-1, 0, 0), Vector3f(0.05f, -0.075f, 0.05f), Vector3f(0.05f, -0.075f, -0.05f), 0.15f);
	walls[1].SetColor(0, 1, 1);
	walls[2] = Plane(Vector3f(0.0f, 0.0f, 1.0f), Vector3f(0.05f, -0.075f, -0.05f), Vector3f(-0.05f, -0.075f, -0.05f), 0.15f);
	walls[2].SetColor(1, 1, 1);
	walls[3] = Plane(Vector3f(0, 0, -1), Vector3f(-0.05f, -0.075f, 0.05f), Vector3f(0.05f, -0.075f, 0.05f), 0.15f);
	walls[3].SetColor(1, 1, 1);

	//Add bowl
	bowl = new Bowl();

	m_itemManifold = new ContactManifold();
	//m_surfaceManifold = new ContactManifold();

	/*AllocConsole();
	freopen("CONOUT$", "a", stdout);*/

	ResetGame();

	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&start);
	//CLEARLOG;
}

void Game::AddNewSphere() {
	m_spheres.push_back(new Sphere());
	m_spheres.back()->SetPos(-0.0f, 0.115f, 0.0f);
	m_spheres.back()->SetVel(0.03f, 0.0f, 0.05f);
	m_spheres.back()->SetSpin(20, Vector3f(0,0,1));						//In rpm
	WriteToConsole();
	//LOG("Sphere " << m_spheres.size() << " Added");
}

Game::~Game(void)
{
	std::vector<Sphere*>().swap(m_spheres);
	delete m_itemManifold;
}

void Game::Update()
{
	// **************************************************
	// The simulation loop should be on its own thread(s)
	// **************************************************
	m_inputManager->Update();
	SimulationLoop();
	Render();
}

void Game::SimulationLoop()
{
	// calculate dt based on the simulation loop rate using a timer
	QueryPerformanceCounter(&end);
	m_dt = static_cast<float>((end.QuadPart - start.QuadPart) / static_cast<double>(frequency.QuadPart));
	start = end;

	m_fps = static_cast<int>(1.0 / m_dt);

	if (notPaused)
	{
		// Calculate the physic calculations on all objects (e.g. new position, velocity, etc)
		CalculateObjectPhysics();

		// Clear the manifold so that we can calculate all collisions for this simulation loop
		m_itemManifold->Clear();
		//m_surfaceManifold->Clear();

		// Find dynamic collisions for all objects and add to contact manifold 
		DynamicCollisionDetection();

		// Handle dynamic collision responses using the contact manifold
		DynamicCollisionResponse();

		// Update the physics calculations on all objects (e.g. new position, velocity, etc)
		UpdateObjectPhysics();
	}
}


//**************************Update the physics calculations on each object***********************
void Game::CalculateObjectPhysics()
{
	for (std::vector<Sphere*>::iterator it = m_spheres.begin(); it != m_spheres.end(); ++it) {
		(*it)->CalculatePhysics(timeScale*m_dt);
		//LOG("Sphere " <<  it - m_spheres.begin() << " Pos: " << (*it)->GetPos() << " NewPos: " << (*it)->GetNewPos() << " Velocity: " << (*it)->GetVel() << " NewVel: " << (*it)->GetNewVel());
	}
}

//**************************Handle dynamic collisions***********************
void Game::DynamicCollisionDetection()
{
	if (m_spheres.size() > 0)
	{
		float time = m_dt * timeScale;
		//Collision detection with scene objects
		for (std::vector<Sphere*>::iterator it = m_spheres.begin(); it != m_spheres.end(); ++it) {
			(*it)->CollisionDetection(trays, m_itemManifold, time);
			(*it)->CollisionDetection(trays + 1, m_itemManifold, time);
			(*it)->CollisionDetection(trays + 2, m_itemManifold, time);
			(*it)->CollisionDetection(walls, m_itemManifold, time);
			(*it)->CollisionDetection(walls + 1, m_itemManifold, time);
			(*it)->CollisionDetection(walls + 2, m_itemManifold, time);
			(*it)->CollisionDetection(walls + 3, m_itemManifold, time);
			(*it)->CollisionDetection(bowl, m_itemManifold, time);
		}
		//Collision with added items
		for (std::vector<Sphere*>::iterator it = m_spheres.begin(); it != (m_spheres.end() - 1); ++it) {
			for (std::vector<Sphere*>::iterator it2 = it + 1; it2 != m_spheres.end(); it2++)
				(*it)->CollisionDetection(*it2, m_itemManifold, m_dt*timeScale);
		}
	}
}

//**************************Handle dynamic collision responses***********************
void Game::DynamicCollisionResponse()
{
	//Responses for all collisions
	for (auto& sphere : m_spheres)
	{
		if(sphere->Shape::IsColliding())
			sphere->SetImpulse(Vector3f());
	}

	//Go through collisions one at a time and add to the impact force of that object for each collision.

	for (int i = 0; i < m_itemManifold->GetNumPoints(); i++)
	{
		ManifoldPoint &point = m_itemManifold->GetPoint(i);

		point.contactID1->CollisionResponse(point);
	}

	//Gathered all impulses, now convert to velocity and set isColliding to false
	for (auto& sphere : m_spheres)
	{
		if (sphere->Shape::IsColliding())
		{
			sphere->SetVelFromImpulse();
			sphere->SetColliding(false);
		}
	}
}

//**************************Update the physics calculations on each object***********************
void Game::UpdateObjectPhysics()
{
	for (std::vector<Sphere*>::iterator it = m_spheres.begin(); it != m_spheres.end();) 
	{
		(*it)->Update();
		//Removes item if it falls outside a sphere of 25cm radius from center of box
		if (((*it)->GetPos() - Vector3f(0.0f, 0.075f, 0.0f)).length() > 0.25f)
			it = m_spheres.erase(it);
		else
			it++;
	}
	trays[0].UpdatePos(m_dt*timeScale);
	trays[2].UpdatePos(m_dt*timeScale);
}

void Game::RemoveLastItem()
{
	if (!m_spheres.empty())
	{
		--Shape::countID;
		m_spheres.pop_back();
		WriteToConsole();
	}
}
void Game::ChangeElasticity(float c)
{
	e += c;
	if (e > 1.0f)
		e = 1.0f;
	else if (e < 0.1f)
		e = 0.1f;
	WriteToConsole();
}

void Game::ChangeFriction(float f)
{
	k += f;
	if (k < 0.0f)
		k = 0.0f;
	//trays[0].SetFriction(trays[0].GetFriction()+f);
	trays[1].SetFriction(trays[1].GetFriction() + f);
	trays[2].SetFriction(trays[2].GetFriction() + f);
	WriteToConsole();
}

void Game::ChangeTimescale(float scale) {
	timeScale *= scale;
}

void Game::PauseGame() {
	notPaused = !notPaused;
}

void Game::RemoveTopTray()
{
	if (!trays[0].IsMoving())
	{
		trays[0].StartMoving(1.f, 10);
	}
}

void Game::AddTopTray()
{
	if (!trays[0].IsMoving())
	{
		trays[0].StartMoving(-1.f, 5);
	}
}

void Game::RemoveBottomTray()
{
	if (!trays[2].IsMoving())
	{
		trays[2].StartMoving(1.f, 10);
	}
}

void Game::AddBottomTray()
{
	if (!trays[2].IsMoving())
	{
		trays[2].StartMoving(-1.f, 5);
	}
}


void Game::ResetGame() {
	e = 0.8f;
	timeScale = 1.0f;
	m_spheres.clear();
	m_itemManifold->Clear();
	ResetCamera();
	k = 0.3f;
	WriteToConsole();
	trays[0].Reset();
	trays[2].Reset();
	trays[1].SetFriction(Game::k);
	trays[2].SetFriction(Game::k);
}

//**************************Render and display the scene in OpenGL***********************
void Game::Render()									// Here's Where We Do All The Drawing
{
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();
	gluPerspective(fovy, 1.0, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();									// Reset The Current Modelview Matrix
	gluLookAt(camera.GetX(), camera.GetY(), camera.GetZ(), focus.GetX(), focus.GetY(), focus.GetZ(), 0, 1, 0);

	glEnable(GL_TEXTURE_2D);
	for (std::vector<Sphere*>::iterator it = m_spheres.begin(); it != m_spheres.end(); ++it) {
		(*it)->Render();
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glDisable(GL_TEXTURE_2D);
	//Draw planes
	trays[0].Render();
	trays[1].Render();
	trays[2].Render();
	walls[0].Render();
	walls[1].Render();
	walls[2].Render();
	walls[3].Render();
	
	bowl->Render();

	SwapBuffers(m_hdc);				// Swap Buffers (Double Buffering)
}

//Camera controls
void Game::ResetCamera()
{
	camera = Vector3f(0.0f, 0.1f, 0.30f);
	focus = Vector3f();
	fovy = 50.0f;
}

void Game::MoveCameraX(float dx)
{
	Vector3f XZAxis = (focus - camera).cross(Vector3f(0, 1, 0)).normalise();
	camera.Set(camera.GetX() + dx*XZAxis.GetX(), camera.GetY(), camera.GetZ() + dx*XZAxis.GetZ());
	focus.Set(focus.GetX() + dx*XZAxis.GetX(), focus.GetY(), focus.GetZ() + dx*XZAxis.GetZ());
}

void Game::MoveCameraY(float dy)
{
	camera.Set(camera.GetX(), camera.GetY() + dy, camera.GetZ());
	focus.Set(focus.GetX(), focus.GetY() + dy, focus.GetZ());
}

void Game::ZoomCamera(float z)
{
	fovy += z;
	if (fovy > 120.0f)
		fovy =120.0f;
	else if (fovy < 5.0f)
		fovy = 5.0f;
}

void Game::RotateCameraX(float r)
{
	Vector3f viewDir = camera - Vector3f(0, focus.GetY(), focus.GetZ());

	const float deg2rad = static_cast<float>(3.14159 / 180);
	Vector3f newCamPos(camera.GetX(), viewDir.GetY()*cos(r*deg2rad) - viewDir.GetZ()*sin(r*deg2rad), viewDir.GetZ()*cos(r*deg2rad) + viewDir.GetY()*sin(r*deg2rad));
	newCamPos += Vector3f(0, focus.GetY(), focus.GetZ());
	camera = newCamPos;

}

void Game::RotateCameraY(float r)
{
	Vector3f viewDir = camera - Vector3f(focus.GetX(), 0, focus.GetZ());

	const float deg2rad = static_cast<float>(3.14159 / 180);
	Vector3f newCamPos(viewDir.GetX()*cos(r*deg2rad) - viewDir.GetZ()*sin(r*deg2rad), camera.GetY(), viewDir.GetZ()*cos(r*deg2rad) + viewDir.GetX()*sin(r*deg2rad));
	newCamPos += Vector3f(focus.GetX(), 0, focus.GetZ());
	camera = newCamPos;
}

void Game::WriteToConsole()
{
	using namespace std;
	system("CLS");
	cout << "1. Number of balls in the system: " << m_spheres.size()<<endl;
	cout << "2. Number of cubes in the system: 0" << endl;
	cout << "3. Magnitude of elasticity: " << Game::e << endl;
	cout << "4. Magnitude of frictional force: " << Game::k << endl;
}

InputManager * Game::GetInputManager() const
{
	return m_inputManager.get();
}