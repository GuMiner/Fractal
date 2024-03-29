#include <algorithm>
#include <sstream>
#include <imgui\imgui.h>
#include "WireCube.h"
#include "InfiniteGrid.h"
#include "ObjectLoader.h"

ObjectLoader::ObjectLoader(GeometryGenerationScheduler* scheduler)
    : objects(), objectCreationMap(), objectCreationNames(), selectedObjectToCreate(0), selectedObject(0), lastSelectedObject(-1)
{
    objectCreationMap["Test Cube"] = [scheduler]() { return new WireCube(scheduler); };
    // objectCreationMap["Test Grid"] = []() { return new InfiniteGrid(); };
    for (auto it = objectCreationMap.begin(); it != objectCreationMap.end(); it++)
    {
        objectCreationNames.push_back(it->first.c_str());
    }
}

void ObjectLoader::UpdateObjectNames()
{
    std::map<std::string, int> objectNameCounter;
    
    backingObjectNames.clear();
    for (BaseObjectType* object : objects)
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

void ObjectLoader::Render()
{
    ImGui::Begin("Object Loader", nullptr, ImVec2(100, 100), 0.50f);
    ImGui::ListBox("Objects To Create", &selectedObjectToCreate, &objectCreationNames[0], (int)objectCreationNames.size());
    if (ImGui::Button("Create"))
    {
        objects.push_back(objectCreationMap[objectCreationNames[selectedObjectToCreate]]());
        UpdateObjectNames();
    }

    if (objectNames.size() != 0)
    {
        ImGui::ListBox("Objects", &selectedObject, &objectNames[0], (int)objectNames.size());
        if (lastSelectedObject != selectedObject)
        {
            lastSelectedObject = selectedObject;
        }

        if (ImGui::Button("Delete"))
        {
            BaseObjectType* object = objects[selectedObject];
            objects.erase(objects.begin() + selectedObject);
            delete object;

            UpdateObjectNames();
            selectedObject = selectedObject >= (int)objectNames.size() ? std::max(0, (int)objectNames.size() - 1) : selectedObject;
        }
    }
    
    ImGui::End();
}