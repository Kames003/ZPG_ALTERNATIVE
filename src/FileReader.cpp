#include "FileReader.h"
#include <cstdlib>

FileReader& FileReader::getInstance()
{
    static FileReader* f = new FileReader();
    return *f;
}

std::string FileReader::readFile(const char* filename)
{
    // open súboru
    std::ifstream file(filename, std::ios::binary);

    if (file)
    {
        std::string content;
        
        // Zistenie veľkosti súboru
        file.seekg(0, std::ios::end);
        content.resize(file.tellg());
        
        // Návrat na začiatok a načítanie celého obsahu
        file.seekg(0, std::ios::beg);
        file.read(&content[0], content.size());
        file.close();

        return content;
    }
    else 
    {
        // mision failed
        printf("Cannot open a file: %s\n", filename);
        exit(EXIT_FAILURE);
    }
}