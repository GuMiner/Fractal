#include <cstring>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "../Telemetry/Logger.h"
#include "StringExtensions.h"

const char* StringExtensions::Comment = "#";

bool StringExtensions::LoadConfigurationFile(const std::string& filename, std::vector<std::string>& lines, std::map<int, std::string>& commentLines)
{
    std::string entireFile;
    if (!StringExtensions::LoadStringFromFile(filename, entireFile))
    {
        Logger::Log("Unable to load the config file!");
        return false;
    }

    int currentLine = 0;
    StringExtensions::Split(entireFile, Newline, false, lines);
    for (unsigned int i = 0; i < lines.size(); i++)
    {
        std::string commentString = std::string(Comment);
        if (StringExtensions::StartsWith(lines[i], commentString))
        {
            commentLines[currentLine] = std::string(lines[i]);

            lines.erase(lines.begin() + i);
            i--;
        }
        else if (StringExtensions::IsWhitespaceOrEmpty(lines[i]))
        {
            commentLines[currentLine] = std::string(lines[i]);

            lines.erase(lines.begin() + i);
            i--;
        }

        ++currentLine;
    }

    return true;
}

bool StringExtensions::LoadStringFromFile(const std::string& filename, std::string& result)
{
    std::ifstream file(filename.c_str());
    if (!file)
    {
        return false;
    }

    std::stringstream stream;
    stream << file.rdbuf();
    result = stream.str();

    file.close();
    return true;
}

// Writes a really big string to a file; returns true on success.
bool StringExtensions::WriteStringToFile(const std::string& filename, std::string& data)
{
    std::ofstream file(filename.c_str());
    if (!file)
    {
        return false;
    }

    file << data.c_str();
    file.close();

    return true;
}

// Splits a string into substrings, optionally including empty tokens if present.
void StringExtensions::Split(const std::string& stringToSplit, char delimator, bool excludeEmpty, std::vector<std::string>& splitStrings)
{
    splitStrings.clear();
    std::stringstream stringStream(stringToSplit);

    std::string item;
    while (std::getline(stringStream, item, delimator))
    {
        if (!excludeEmpty || (excludeEmpty && !item.empty()))
        {
            splitStrings.push_back(item);
        }
    }
}

// Removes comment lines from the string.
void StringExtensions::RemoveCommentLines(std::vector<std::string>& lines)
{
    for (unsigned int i = 0; i < lines.size(); i++)
    {
        std::string commentString = std::string(Comment);
        if (StringExtensions::StartsWith(lines[i], commentString))
        {
            lines.erase(lines.begin() + i);
            i--;
        }
    }
}

// Splits a line into two parts and grabs the secondary part.
bool StringExtensions::SplitAndGrabSecondary(const std::string& line, std::string& secondary)
{
    std::vector<std::string> stringParts;
    Split(line, Space, true, stringParts);

    if (stringParts.size() != 2)
    {
        return false;
    }

    secondary = stringParts[1];

    return true;
}

// Determines if the given string starts with the provided sub string.
bool StringExtensions::StartsWith(const std::string& givenString, const std::string& subString)
{
    if (givenString.length() < subString.length())
    {
        return false;
    }

    return (strncmp(givenString.substr(0, subString.length()).c_str(), subString.c_str(), subString.length()) == 0);
}

// Determines if the given string is completely empty or whitespace.
bool StringExtensions::IsWhitespaceOrEmpty(const std::string& givenString)
{
    if (givenString.length() == 0)
    {
        return true;
    }

    for (char character : givenString)
    {
        if (character != ' ' && character != '\r' && character != '\n' && character != '\t')
        {
            return false;
        }
    }
    return true;
}

// Attempts to parse a boolean from a string.
bool StringExtensions::ParseBoolFromString(const std::string& stringToParse, bool& value)
{
    std::istringstream inputStream(stringToParse);
    return inputStream >> std::boolalpha >> value ? true : false;
}

// Attempts to parse an integer from a string.
bool StringExtensions::ParseIntFromString(const std::string& stringToParse, int& value)
{
    std::istringstream inputStream(stringToParse);
    return inputStream >> value ? true : false;
}

// Attempts to parse a double from a string.
bool StringExtensions::ParseFloatFromString(const std::string& stringToParse, float& value)
{
    value = (float)atof(stringToParse.c_str());
    return true;
}

StringExtensions::StringExtensions()
{
}

bool StringExtensions::Test()
{
    // Test some well-known byte amounts
    std::string result = GetNiceByteValue(0);
    if (std::string("0 bytes").compare(result) != 0)
    {
        Logger::LogError("0 bytes test failure, got: ", result);
        return false;
    }

    result = GetNiceByteValue(1);
    if (std::string("1 bytes").compare(result) != 0)
    {
        Logger::LogError("0 bytes test failure, got: ", result);
        return false;
    }

    result = GetNiceByteValue(1024);
    if (std::string("1 kB").compare(result) != 0)
    {
        Logger::LogError("1 kB test failure, got: ", result);
        return false;
    }

    result = GetNiceByteValue(1025);
    if (std::string("1 kB + 1 bytes").compare(result) != 0)
    {
        Logger::LogError("1kB + 1 bytes test failure, got: ", result);
        return false;
    }

    result = GetNiceByteValue(1048576);
    if (std::string("1 MB").compare(result) != 0)
    {
        Logger::LogError("1 MB test failure, got: ", result);
        return false;
    }

    result = GetNiceByteValue(1048577);
    if (std::string("1 MB + 1 bytes").compare(result) != 0)
    {
        Logger::LogError("1 MB + 1 bytes test failure, got: ", result);
        return false;
    }

    result = GetNiceByteValue(1099511627776);
    if (std::string("1 TB").compare(result) != 0)
    {
        Logger::LogError("1 TB test failure, got: ", result);
        return false;
    }

    result = GetNiceByteValue(1125899906842625);
    if (std::string("1 PB + 1 bytes").compare(result) != 0)
    {
        Logger::LogError("1 PB + 1 bytes test failure, got: ", result);
        return false;
    }

    result = GetNiceByteValue(1152921504606846976);
    if (std::string("1024 PB").compare(result) != 0)
    {
        Logger::LogError("1024 PB test failure, got: ", result);
        return false;
    }

    return true;
}

std::string StringExtensions::GetNiceByteValue(long long byteAmount)
{
    // Decompose our byte amount into subamounts.
    const char* scales[] =
    {
        "bytes",
        "kB",
        "MB",
        "GB",
        "TB",
        "PB"
    };

    long long amounts[] =
    {
        0, 0, 0, 0, 0, 0 // bytes to Pb.
    };

    char currentScale = 0;
    while (byteAmount != 0 && currentScale < 5)
    {
        amounts[currentScale] = byteAmount % 1024;
        byteAmount /= 1024;
        ++currentScale;
    }

    amounts[currentScale] = byteAmount;

    // Output those amounts, skipping zeros unless the only result is a zero.
    bool isFirst = true;
    bool outputAny = false;
    std::stringstream textStream;
    for (int i = 5; i >= 0; i--)
    {
        if (amounts[i] != 0 || (!outputAny && i == 0))
        {
            textStream << (!isFirst ? " + " : "") << amounts[i] << " " << scales[i];
            outputAny = true;
            isFirst = false;
        }
    }

    return textStream.str();
}
