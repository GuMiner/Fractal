#include <gl/glew.h>
#include "Scene.h"

Scene::Scene() : camera(nullptr) {
	testModel = new Model();
	fractal = new Fractal();
	sky = new Sky(); 
}

void Scene::ClearScreen() {
	const GLfloat color[] = { 0, 0, 0, 1 };
	const GLfloat one = 1.0f;
	glClearBufferfv(GL_COLOR, 0, color);
	glClearBufferfv(GL_DEPTH, 0, &one);
}

bool Scene::Init(ShaderFactory* shaderFactory) {
	camera = new Camera();

	if (!fractal->Init(shaderFactory)) {
		return false;
	}

	if (!sky->Init(shaderFactory)) {
		return false;
	}

	if (!testModel->Init(shaderFactory)) {
		return false;
	}
	if (!testModel->SendMesh()) {
		return false;
	}
	return true;
}

void Scene::RenderScene(float currentTime) {
	ClearScreen();

	testModel->Render(camera, currentTime);

	// fractal->Render(currentTime);
	sky->Render(currentTime);
}