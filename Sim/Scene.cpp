#include <gl/glew.h>
#include "Scene.h"

Scene::Scene() : camera(nullptr) {
	testModel = new Model();
	testTerrain = new Terrain();
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
	camera = new Camera();

	if (!sky->Init(shaderFactory)) {
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
	sky->Render(camera);
}