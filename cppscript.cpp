
#include <iostream>
#include <cstdlib>
#include <filesystem>
#include <fstream>

const int SUCCESS = 0;
const int MAX_ARGUMENTS = 2;

const std::string importTag = "@import";
const std::string compilerString = "clang++ --std=c++17 -Wall -Werror";

int main(int argCount, char *argValues[])
{
    if (argCount != MAX_ARGUMENTS)
    {
        std::cout << "Usage: cppscript programInput.cpp" << std::endl;
        return 0;
    }

    // Process command-line arguments.
    std::string programInput = argValues[1];
    const std::string programOutput = 
        programInput.substr(0,programInput.find_last_of('.')) + ".run";
    const std::string toExecuteString = "./" + programOutput;

    // Process @imports.
    std::ifstream programFile(programInput);
    if (programFile.is_open())
    {
        std::string line;
        while (std::getline(programFile, line))
        {
            // std::cout << "Looking through line: " << line << std::endl;
            size_t index = line.find(importTag);
            if (index != std::string::npos)
            {
                programInput += " " + line.substr(
                    index + importTag.length() + 1);
                std::cout << "index " << index
                    << " programInput " << programInput << std::endl;
            }
        }
    }
    else
    {
        std::cout << "Could not open file." << std::endl;
    }

    // Compile.
    const std::string toCompileString =
        compilerString + " " + programInput + " -o " + programOutput;
    std::cout << "Compiling " << programInput << std::endl;
    std::cout << toCompileString << std::endl;
    int result = std::system(toCompileString.c_str());
    std::cout << "Finished compiling " << programInput << " (" << result << ")"
        << std::endl;

    // Execute.
    if (result == SUCCESS)
    {
        std::cout << "Running " << programOutput << std::endl;
        std::cout << toExecuteString << std::endl;
        result = std::system(toExecuteString.c_str());
        std::cout << "Finished executing " << programOutput << " (" << result << ")"
            << std::endl;
    }
    else
    {
        std::cout << "Since there was an error, program was not run." << std::endl;
    }

    return 0;
}
