#pragma once

#include <string>
#include <fstream>
#include <iostream>

// Singleton trieda pre čítanie súborov
// Používa sa najmä pre čítanie shader súborov, ale aj pre iné účely
class FileReader
{
private:
    // Singleton pattern - private konštruktor
    FileReader() = default;
    
    // Zakázané kopírovanie
    FileReader(const FileReader&) = delete;
    FileReader& operator=(const FileReader&) = delete;
    
    // Zakázané presúvanie
    FileReader(FileReader&&) = delete;
    FileReader& operator=(FileReader&&) = delete;

public:
    // Získanie jedinej inštancie
    static FileReader& getInstance();
    
    // Prečítanie celého súboru do stringu
    std::string readFile(const char* filename);
};