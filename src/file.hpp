#pragma once

const std::vector<uint8_t> FileToU8Vec(const std::string inFile);
const std::vector<uint16_t> FileToU16Vec(const std::string inFile);
const std::string FileToString(const std::string inFile);
void U16vecToFile(const std::vector<uint16_t> &outVec, const std::string outFile);
void U8vecToFile(const std::vector<uint8_t> &outVec, const std::string outFile);
