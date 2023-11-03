#include "Scene.h"

Scene::Scene() : camera(nullptr)
{
	testModel = new Model();
}

bool Scene::Init(ShaderFactory* shaderFactory)
{
	camera = new Camera();

	if (!testModel->Init(shaderFactory)) {
		return false;
	}
	if (!testModel->SendMesh()) {
		return false;
	}
	return true;
}

void Scene::RenderScene(float currentTime)
{
	testModel->Render(camera, currentTime);
}