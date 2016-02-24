/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <Latchezar Tzvetkoff> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Latchezar Tzvetkoff
 * ----------------------------------------------------------------------------
 */

/* unpack_command.hpp */

#ifndef __giffler_unpack_command__
#define __giffler_unpack_command__

#include <iostream>

namespace giffler
{

class UnpackCommand
{

private:
    std::string _in;
    std::string _out;

public:
    UnpackCommand(int, char *const *);
    int execute();

};

}

#endif /* !defined(__giffler_unpack_command__) */
