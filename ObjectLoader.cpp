#include <algorithm>
#include <sstream>
#include <imgui\imgui.h>
#include "TestCube.h"
#include "TestGrid.h"
#include "ObjectLoader.h"

ObjectLoader::ObjectLoader()
    : objects(), objectCreationMap(), objectCreationNames(), selectedObjectToCreate(0), selectedObject(0)
{
    objectCreationMap["Test Cube"] = []() { return new TestCube(); };
    objectCreationMap["Test Grid"] = []() { return new TestGrid(); };
    for (auto it = objectCreationMap.begin(); it != objectCreationMap.end(); it++)
    {
        objectCreationNames.push_back(it->first.c_str());
    }
}

void ObjectLoader::Update(float frameTime)
{
    for (IObject* object : objects)
    {
        object->Update(frameTime);
    }
}

void ObjectLoader::UpdateObjectNames()
{
    std::map<std::string, int> objectNameCounter;
    
    backingObjectNames.clear();
    for (IObject* object : objects)
    {
        std::string name = object->GetName();
        if (objectNameCounter.find(name) == objectNameCounter.end())
        {
            objectNameCounter.insert(std::make_pair(name, 0));
        }

        objectNameCounter[name]++; // This ensures we start counting at 1, not zero.


        std::stringstream nameWithId;
        nameWithId << name << " *" << objectNameCounter[name];
        backingObjectNames.push_back(nameWithId.str());
    }

    // ImGui needs a const char** data structure, so convert our backing store to that.
    objectNames.clear();
    for (const std::string& name : backingObjectNames)
    {
        objectNames.push_back(name.c_str());
    }
}

void ObjectLoader::DisplayLoaderDialog()
{
    
    ImGui::Begin("Object Loader", nullptr, ImVec2(100, 100), 0.50f);
    ImGui::ListBox("Objects To Create", &selectedObjectToCreate, &objectCreationNames[0], objectCreationNames.size());
    if (ImGui::Button("Create"))
    {
        objects.push_back(objectCreationMap[objectCreationNames[selectedObjectToCreate]]());
        UpdateObjectNames();
    }

    if (objectNames.size() != 0)
    {
        ImGui::ListBox("Objects", &selectedObject, &objectNames[0], objectNames.size());
        if (ImGui::Button("Delete"))
        {
            IObject* object = objects[selectedObject];
            objects.erase(objects.begin() + selectedObject);
            delete object;

            UpdateObjectNames();
            selectedObject = selectedObject >= (int)objectNames.size() ? std::max(0, (int)objectNames.size() - 1) : selectedObject;
        }
    }
    
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