#include <gl/glew.h>
#include "Scene.h"

Scene::Scene() : camera(nullptr) {
	testModel = new Model();
	testTerrain = new Terrain();

	grid = new InfiniGrid();
	sky = new Sky(); 

	fractal = new Fractal();
	compass = new Compass();
}

void Scene::ClearScreen() {
	const GLfloat color[] = { 0, 0, 0, 1 };
	const GLfloat one = 1.0f;
	glClearBufferfv(GL_COLOR, 0, color);
	glClearBufferfv(GL_DEPTH, 0, &one);
}

bool Scene::Init(ShaderFactory* shaderFactory) {
	save = new SaveFile();
	camera = new Camera();
	camera->Reset(save->Config.position, save->Config.forwards, save->Config.up);

	if (!sky->Init(shaderFactory)) {
		return false;
	}

	if (!grid->Init(shaderFactory)) {
		return false;
	}

	if (!testTerrain->Init(shaderFactory)) {
		return false;
	}

	if (!testModel->Init(shaderFactory)) {
		return false;
	}
	if (!testModel->SendMesh()) {
		return false;
	}

	if (!fractal->Init(shaderFactory)) {
		return false;
	}

	if (!compass->Init(shaderFactory)) {
		return false;
	}
	if (!compass->SendMesh()) {
		return false;
	}

	return true;
}

void Scene::Update() {
	camera->Update();
}

void Scene::RenderScene() {
	ClearScreen();

	compass->Render(camera);
	testModel->Render(camera);

	testTerrain->Render(camera);

	// fractal->Render(currentTime);
	grid->Render(camera);
	sky->Render(camera);
}

Scene::~Scene() {
	// Update save file
	save->Config.position = camera->Position();
	save->Config.forwards = camera->Forwards();
	save->Config.up = camera->Up();
	save->Save();

	delete save;

	if (camera != nullptr) {
		delete camera;
	}

	delete compass;
	delete fractal;

	delete sky;
	delete testTerrain;
	delete testModel;
}