#include "FileLoader.h"
#include <string>

String ConsoleLoader::DataCheck(String data) const
{
    unsigned int j =0;
    for(unsigned int i = 0; i < data.size();i++)
    {
        if (data[0] != '[')
        {
            std::cout << "The wrong format of string massive. You forgot this '[': \n" << ' ' << data;
            break;
        }
        else if (data[data.size() - 1] != ']')
        {
            std::cout << "The wrong format of string massive. You forgot this ']': \n" << ' ' << data;
            break;
        }
        else if (data[i] == ' ')
        {}
        else if (data[i] == ',')
        {}
        else if (data[i] == ';')
        {   
            j += i;
            if (i != j)
            {
                std::cout << "The amount of variables is incorrect. Add/Delete some.: \n" << ' ' << data;
                break;
            }           
        }
        else
            i++;
    }
    return data;
}