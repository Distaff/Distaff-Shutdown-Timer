#include <iostream>
#include <fstream>
#include <string>

void WriteToFile(char* filename, char* val)
{
    std::ofstream file( filename );
    file << val;
    file.close();
}

char* ReadFromFile(char* filename)
{
    std::ifstream file(filename);
    std::string line;
    std::getline(file, line);
    char *val = &line[0u];
    file.close();
    return val;
}
