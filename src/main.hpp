#pragma once


void Decomp(uint16_t a);
void DecompFromROM(uint16_t a);
void MapTree(uint16_t a);
const std::tuple<std::vector<uint16_t>, uint16_t> CompressText(const std::string file);

const uint8_t Read8(uint32_t offset);
const uint16_t Read16(uint32_t offset);


std::vector<uint8_t> rom;
std::vector<uint16_t> compData; //array of bits

uint16_t _06D6 = 0;
std::array<uint8_t, 2048> result{};  //A9DE
std::array<uint8_t, 2048> result2{}; //A7BC
uint16_t _1E = 0;
uint16_t _20 = 0;

std::vector<uint8_t> resultVec;  //A9DE
std::vector<uint8_t> result2Vec; //A7BC

const std::vector<std::string> charMap
{
    "\0", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
    " ", "!", "?", "Ä", "Â", "", "", "\'", "Ê", "", "", "Ö", "", "-", "", "/",
    "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "Ü", "Û", "Ù", "", "", "",
    ":", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O",
    "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", ":", "%", "&", "s", "",
};

struct compSeq
{
    uint16_t key;
    uint8_t length;
    std::vector<bool> bits;
};

std::map<uint8_t, compSeq> valKey;

const std::map<std::string, uint8_t> charValues
{
    {"\r\n", 0x00},
    {" ", 0x20}, {"!", 0x21}, {"?", 0x22}, {"Ä", 0x23}, {"Â", 0x24}, {"'", 0x27},
    {"Ê", 0x28}, {"Ö", 0x2B}, {"-", 0x2D}, {"/", 0x2F},
    {"0", 0x30}, {"1", 0x31}, {"2", 0x32}, {"3", 0x33}, {"4", 0x34}, {"5", 0x35}, {"6", 0x36}, {"7", 0x37},
    {"8", 0x38}, {"9", 0x39},
    {":", 0x40}, {"A", 0x41}, {"B", 0x42}, {"C", 0x43}, {"D", 0x44}, {"E", 0x45}, {"F", 0x46}, {"G", 0x47},
    {"H", 0x48}, {"I", 0x49}, {"J", 0x4A}, {"K", 0x4B}, {"L", 0x4C}, {"M", 0x4D}, {"N", 0x4E}, {"O", 0x4F},
    {"P", 0x50}, {"Q", 0x51}, {"R", 0x52}, {"S", 0x53}, {"T", 0x54}, {"U", 0x55}, {"V", 0x56}, {"W", 0x57},
    {"X", 0x58}, {"Y", 0x59}, {"Z", 0x5A}, {":", 0x5B}, {"%", 0x5C}, {"&", 0x5D}, {"s", 0x5E},
};
