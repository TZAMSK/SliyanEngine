#include "util/readFile.hpp"

#include <fstream>
#include <string>

std::string loadShaderAsString(const std::string &path)
{
    std::string result = "";
    std::string line = "";
    std::ifstream file(path);

    if (file.is_open())
    {
        while (std::getline(file, line))
        {
            result += line + '\n';
        }
        file.close();
    }

    return result;
}
