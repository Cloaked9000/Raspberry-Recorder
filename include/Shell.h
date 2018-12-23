//
// Created by fred on 23/12/18.
//

#ifndef RASPBERRYRECORDER_SHELL_H
#define RASPBERRYRECORDER_SHELL_H


#include <string>
#include <stdexcept>

class Shell
{
public:

    /*!
     * Executes a shell script/command
     *
     * @param script_data Script/command to execute. Should be escaped.
     * @return The script/command output
     */
    static std::string execute(const std::string &script_data);
};


#endif //RASPBERRYRECORDER_SHELL_H
