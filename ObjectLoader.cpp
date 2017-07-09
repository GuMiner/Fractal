#include <imgui\imgui.h>
#include "ObjectLoader.h"

ObjectLoader::ObjectLoader()
    : objects()
{
}

void ObjectLoader::Update(float frameTime)
{
    for (IObject* object : objects)
    {
        object->Update(frameTime);
    }
}

void ObjectLoader::DisplayLoaderDialog()
{
    ImGui::Begin("Object Loader", nullptr, ImVec2(100, 100), 0.50f);
    ImGui::SetCursorPos(ImVec2(5, 20));
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%.1f", 22.0f);
    ImGui::End();
}

void ObjectLoader::Render(glm::mat4& projectionMatrix)
{
    DisplayLoaderDialog();

    for (IObject* object : objects)
    {
        object->Render(projectionMatrix);
    }
}