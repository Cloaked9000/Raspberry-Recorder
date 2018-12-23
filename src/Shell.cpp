//
// Created by fred on 23/12/18.
//

#include "Shell.h"

std::string Shell::execute(const std::string &script_data)
{
    std::string ret;
    FILE *fp;
    char buff[4096];

    /* Open the command for reading. */
    fp = popen(std::string("/bin/bash -c '" + script_data + "'").c_str(), "r");
    if (fp == nullptr)
    {
        throw std::runtime_error("Failed to open pipe");
    }

    try
    {
        size_t read_size = 0;
        do
        {
            read_size = fread(buff, sizeof(char), sizeof(buff), fp);
            if(!ferror(fp))
            {
                ret.append(buff, read_size);
            }

        } while(read_size == sizeof(buff));
    }
    catch(const std::exception &e)
    {
        pclose(fp);
        throw;
    }

    pclose(fp);

    if(!ret.empty() && ret.back() == '\n')
        ret.pop_back();

    return ret;
}
