#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

template <typename  T>
using DynamicArray = std::vector<T>;
using String = std::string;


class Loader
{
public:
    virtual const DynamicArray<String>& Load() const = 0;

    const DynamicArray<String>& GetLastLoadedData() const {return CachedData; }
protected:
    mutable DynamicArray<String> CachedData;
};

inline std::ostream& operator<<(std::ostream& output,const DynamicArray<String>& data)
{
    for(unsigned int i = 0; i < data.size(); i++)
    {
        output << data[i];
        output << ',' << std::endl;
    }
    return output;
}

class ConsoleLoader : public Loader
{
public:
    String DataCheck(String data) const;
    virtual const DynamicArray<String>& Load() const override
    {
        CachedData.clear();
        String Data = "";
        while (true)
        {
            std::cout << "Enter matrices one by one" << std::endl;
            std::getline(std::cin, Data);
            std::cin >> Data;
            if(Data.empty()) 
                break;
            CachedData.push_back(DataCheck(Data));
        }
        return CachedData;
    }
};


class FileLoader : public Loader
{
public:
    FileLoader(const String file_name)
        : FilePath(file_name){}
    virtual const DynamicArray<String>& Load() const override
    {
        CachedData.clear();
        std::ifstream FileData(FilePath);
        if (FilePath.empty())
        {
            std::cout << "FilePath is empty." << std::endl;
            return CachedData;
        }
        if(FileData.is_open())
        {
            for(String line; std::getline(FileData, line);)
            {
                CachedData.push_back(line);
            }
        }
        else
        {
            std::cout << "Error opening file." << std::endl;
            return CachedData;
        }
        FileData.close();
        return CachedData;
    }
private:
    String FilePath = "";
};