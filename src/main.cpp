#include <fstream>
#include <sstream>
#include <iostream>
#include <array>
#include <iomanip>
#include <vector>
#include <cstring>
#include <map>

#include "main.hpp"
#include "file.hpp"


main(int argc, char *argv[])
{
    if(argc > 1)
    {
        rom = FileToU8Vec(argv[1]);
    }
    else
    {
        rom = FileToU8Vec("Donkey Kong Country 3 - Dixie Kong's Double Trouble! (USA) (En,Fr).sfc");
    }

    compData.resize(0x4EE);
    std::memcpy(compData.data(), rom.data() + 0x3A1ECA, 0x4EE);

    MapTree(0x000E);

    // Decomp(0x000E);
    // DecompFromROM(0x000E);

    const auto [compressed, decompLength] = CompressText("levelnames.txt");

    std::vector<uint8_t> rom2 = rom;
    std::memcpy(rom2.data() + 0x3A1ECA, compressed.data(), compressed.size() * 2);
    std::memcpy(rom2.data() + 0x379E29, &decompLength, 2);

    U8vecToFile(rom2, "modified.sfc");

    if(int16_t diff = compressed.size() - 0x4EE / 2; diff > 0)
    {
        std::cout << "Warning: new name list is bigger than the original by " << diff << " bytes.\nWhatever's after has been overwritten in the modified rom!\n\nPress any key to continue.\n";
        std::cin.ignore();
    }
    else
    {
        std::cout << "Modified name list successfully!\n";
        std::cin.ignore();
    }

    //print decompressed chars
    // for(const auto &v : resultVec)
    // {
    //     std::cout << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << +v << " ";
    //     if(!v)
    //     {
    //         std::cout << "\n";
    //     }
    // }

    //print readable strings
    // for(const auto &v : resultVec)
    // {
    //     if(!v)
    //     {
    //         std::cout << "\n";
    //     }
    //     else if(charMap[v] == "")
    //     {
    //         std::cout << "\n  missing char: " << std::hex << +v << "\n";
    //     }
    //     else
    //     {
    //         std::cout << charMap[v];
    //     }
    // }

    // print compressed bit strings
    // for(const auto &v : valKey)
    // {
    //     std::cout << std::hex << +v.first << " ";
    //     for(const auto &w : v.second.bits)
    //     {
    //         std::cout << w;
    //     }
    //     std::cout << "\n";
    // }
}


void Decomp(uint16_t a)
{
    uint16_t decompLength = Read16(0x379DF5 + Read8(0x379DF2 + _06D6) + ((a - 1) << 2)); //_24
    const uint16_t _46 = Read16(0x379DE6 + (_06D6 << 1));
    // uint16_t _22 = 0;
    uint16_t offset = Read16(0x379EE5 + _46); //initial offset

    uint16_t compIndex = 0; //_42
    uint16_t currentData = compData[0]; //_1C | load a chunk of bits from bitfield of compressed data

    int8_t y = 15;
    while(true)
    {
        const bool carry = ~currentData & 0x8000;

        if(const uint16_t temp3 = Read16(0x379EE7 + offset + carry * 2 + _46); temp3)
        {
            currentData <<= 1; //next bit from bitfield of compressed data
            offset = temp3;
            
            if(--y < 0)
            {
                currentData = compData[++compIndex]; //reload chunk with next set of bits
                y = 15;
            }
        }
        else //data at offset is 0. arrived at decompressed value
        {
            resultVec.push_back(Read8(0x379EE7 + offset + _46 - 1)); //RAM:A9DE
            offset = Read16(0x379EE5 + _46);

            // if(!_22)
            // {
            //     _22 = (resultVec.back() < 5) ? result.back() : 1;
            // }
            
            // if(!result.back() && !--_22)
            // {
            //     result2Vec.push_back(resultVec.size()); //RAM:A7BC, offsets to where current decomp'ed string ends
            // }

            if(!--decompLength)
            {
                return;
            }
        }
    }
}


void DecompFromROM(uint16_t a)
{
    uint16_t _24 = (a - 1) << 2;
    uint16_t temp = Read8(0x379DF2 + _06D6) + _24;
    _24 = Read16(0x379DF5 + temp);
    uint16_t _42 = Read16(0x379DF7 + temp); //_42
    // uint16_t _44 = 0x003A;
    const uint16_t _46 = Read16(0x379DE6 + (_06D6 << 1));
    _42 += Read16(0x379DEC + (_06D6 << 1));
    uint16_t _22 = 0;
    uint16_t _1A = Read16(0x379EE5 + _46);

    uint32_t _1C = Read16(0x3A0000 | _42); //uses _44=0x003A as bank select

    int8_t y = 15;
    while(true)
    {
        _1C <<= 1;
        const uint8_t temp = (_1C & 0x10000) ? 0 : 2;

        if(const uint16_t temp2 = Read16(0x379EE7 + _1A + temp + _46); temp2)
        {
            _1A = temp2;
            if(--y < 0)
            {
                _42 += 2;
                _1C = Read16(0x3A0000 | _42);
                y = 15;
            }
        }
        else
        {
            result[_20] = Read8(0x379EE7 + _1A + _46 - 1); //RAM:A9DE
            if(!_22)
            {
                _22 = (result[_20] < 5) ? result[_20] : 1;
            }
            
            if(!result[_20] && !--_22)
            {
                result2[_1E] = _20; //RAM:A7BC
                _1E += 2;
            }

            _1A = Read16(0x379EE5 + _46);

            if(!--_24)
            {
                return;
            }

            ++_20;
            _1C >>= 1;
        }
    }
}


void MapTree(uint16_t a)
{
    uint16_t offset = Read16(0x379EE5);

    // uint16_t compIndex = 0;
    uint16_t currentData = 0;
    uint32_t counter = 0;
    uint8_t length = 0;

    int8_t y = 15;
    while(counter <= 0xFFFF)
    {
        const bool carry = ~currentData & 1;

        if(const uint16_t temp3 = Read16(0x379EE7 + offset + carry * 2); temp3)
        {
            currentData >>= 1;
            offset = temp3;
            ++length;
        }
        else
        {
            const uint8_t val = Read8(0x379EE7 + offset - 1);
            offset = Read16(0x379EE5);
            valKey.insert({val, {uint16_t(counter), length}});

            currentData = ++counter;
            length = 0;
        }
    }

    for(auto &v : valKey)
    {
        for(int x = 0; x < v.second.length; ++x)
        {
            v.second.bits.push_back((v.second.key >> x) & 1);
        }
    }
}


const std::tuple<std::vector<uint16_t>, uint16_t> CompressText(const std::string file)
{
    const std::string levelNames = FileToString(file);

    std::vector<bool> compText;
    uint16_t decompLength = 0;

    for(auto x = 0; x < levelNames.size(); ++x)
    {
        std::string pa = levelNames.substr(x, 1);

        if(!charValues.count(pa))
        {
            if(charValues.count(levelNames.substr(x, 2)))
            {
                pa = levelNames.substr(x++, 2);
            }
            else
            {
                std::cout << "warning: unrecognized character: " << pa << "\n";
            }
            
        }

        const uint8_t val = charValues.at(pa);
        for(const auto &v : valKey.at(val).bits)
        {
            compText.push_back(v);
        }

        ++decompLength;
    }

    while(compText.size() & 0b1111)
    {
        compText.push_back(0);
    }
    std::vector<uint16_t> res;
    for(int y = 0; y < compText.size() / 16; ++y)
    {
        uint16_t w = 0;
        for(int x = 0; x < 16; ++x)
        {
            w |= compText[x + y * 16] << (15 - x);
        }
        res.push_back(w);
    }
    res.push_back(0); //? not sure what the point of this is

    return std::make_tuple(res, decompLength);
}


const uint8_t Read8(uint32_t offset)
{
    return rom[offset & 0x00FFFFFF];
}


const uint16_t Read16(uint32_t offset)
{
    offset &= 0x00FFFFFF;
    return rom[offset] | (rom[offset + 1] << 8);
}
