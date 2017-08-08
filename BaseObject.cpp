#include "BaseObject.h"

BaseObject::BaseObject(std::string name)
    : name(name)
{
}


BaseObject::~BaseObject()
{
}

std::string BaseObject::GetName()
{
    return name;
}