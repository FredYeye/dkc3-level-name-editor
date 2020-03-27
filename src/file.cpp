#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <cstring>

#include "file.hpp"


const std::vector<uint8_t> FileToU8Vec(const std::string inFile)
{
    std::ifstream iFile(inFile.c_str(), std::ios::in | std::ios::binary);
    if(iFile.is_open() == false)
    {
        std::cout << "File " << inFile << " not found" << std::endl;
        std::cin.ignore();
        exit(0);
    }

    std::ostringstream contents;
    contents << iFile.rdbuf();
    iFile.close();

    const std::string contentStr = contents.str();

    return std::vector<uint8_t>(contentStr.begin(), contentStr.end());
}


const std::vector<uint16_t> FileToU16Vec(const std::string inFile)
{
    std::ifstream iFile(inFile.c_str(), std::ios::in | std::ios::binary);
    if(iFile.is_open() == false)
    {
        std::cout << "File " << inFile << " not found" << std::endl;
        std::cin.ignore();
        exit(0);
    }

    std::ostringstream contents;
    contents << iFile.rdbuf();
    iFile.close();

    const std::string contentStr = contents.str();

    const std::vector<uint8_t> temp(contentStr.begin(), contentStr.end());
    std::vector<uint16_t> temp2(temp.size() / 2);

    std::memcpy(temp2.data(), temp.data(), temp.size());

    return temp2;
}


const std::string FileToString(const std::string inFile)
{
    std::ifstream iFile(inFile.c_str(), std::ios::in | std::ios::binary);
    if(iFile.is_open() == false)
    {
        std::cout << "File " << inFile << " not found" << std::endl;
        std::cin.ignore();
        exit(0);
    }

    std::ostringstream contents;
    contents << iFile.rdbuf();
    iFile.close();

    return contents.str();
}


void U16vecToFile(const std::vector<uint16_t> &outVec, const std::string outFile)
{
	std::ofstream result(outFile.c_str(), std::ios::out | std::ios::binary);
	result.write((char*)outVec.data(), outVec.size() * 2);
	result.close();
	std::cout << "Created " << outFile << std::endl;
}


void U8vecToFile(const std::vector<uint8_t> &outVec, const std::string outFile)
{
	std::ofstream result(outFile.c_str(), std::ios::out | std::ios::binary);
	result.write((char*)outVec.data(), outVec.size());
	result.close();
}
