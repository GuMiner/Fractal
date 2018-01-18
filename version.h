#pragma once
#include <sstream>
#include <string>

// Used for autoversioning with the IncrementBuildVersion.py script.
namespace AutoVersion
{
    static const long MAJOR_VERSION = 0;
    static const long MINOR_VERSION = 289;

    static std::string GetVersionString(std::string name)
    {
        std::stringstream versionString;
        versionString << name << " " << MAJOR_VERSION << "." << MINOR_VERSION;
        return versionString.str();
    }
}
