#include "Scene.h"

Scene::Scene()
{
    testModel = new Model();
}

void Scene::Init(ShaderFactory* shaderFactory)
{
    testModel->Init(shaderFactory);
}

void Scene::RenderScene()
{

}